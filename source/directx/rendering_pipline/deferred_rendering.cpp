
#include"../../system/main.h"
#include "deferred_rendering.h"
#include"../renderer.h"
#include"../resource/shader_manager.h"
#include"../../system/myimgui.h"

namespace DeferredRenderingData
{
	enum SRVIndexInPixelShader
	{
		ALBEDCOLOR_SRVINDEX,
		NORMAL_SRVINDEX,
		SPECULAR_SRVINDEX,
		DEPTHSTENCIL_SRVINDEX,
	};
}

DeferredRendering::DeferredRendering(
	ID3D11ShaderResourceView * albedColorSRV,
	ID3D11ShaderResourceView * normalSRV,
	ID3D11ShaderResourceView * specularSRV,
	ID3D11ShaderResourceView* depthstencilSRV,
	float clearColor_R,
	float clearColor_B,
	float clearColor_G,
	float clearColor_A
)
	: m_AlbedColorSRV(albedColorSRV)
	, m_NormalSRV(normalSRV)
	, m_SpecularSRV(specularSRV)
	, m_DepthStancilSRV(depthstencilSRV)
	, m_Color{ clearColor_R , clearColor_B , clearColor_G ,clearColor_A }
{
}

DeferredRendering::~DeferredRendering()
{
}

bool DeferredRendering::Init()
{
	Renderer& renderer = Renderer::GetInstance();

	m_DeviceContext = renderer.GetDeviceContext();

	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::Init m_DeviceContextがnullptr");
		return false;
	}

	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("DeferredRendering::Init GetShaderが失敗");
		return false;
	}

	if (CreatePolygon() == false)
	{
		Logger::GetInstance().SetLog("DeferredRendering::Init CreatePolygonが失敗");
		return false;
	}

	return true;
}

void DeferredRendering::Draw() const
{
	Renderer& renderer = Renderer::GetInstance();

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	renderer.SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// 表示するインデックスバッファを選択
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//テクスチャ設定

	// アルベドカラーセット
	m_DeviceContext->PSSetShaderResources(DeferredRenderingData::ALBEDCOLOR_SRVINDEX, 1, &m_AlbedColorSRV);
	// 法線セット
	m_DeviceContext->PSSetShaderResources(DeferredRenderingData::NORMAL_SRVINDEX, 1, &m_NormalSRV);
	// スぺキュラセット
	m_DeviceContext->PSSetShaderResources(DeferredRenderingData::SPECULAR_SRVINDEX, 1, &m_SpecularSRV);
	// デプスステンシルセット
	m_DeviceContext->PSSetShaderResources(DeferredRenderingData::DEPTHSTENCIL_SRVINDEX, 1, &m_DepthStancilSRV);

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画
	m_DeviceContext->DrawIndexed(6, 0, 0);

	ImGui::Begin(MyImGuiData::k_DebugWindowName);

	ImGui::BeginChild(MyImGuiData::k_RenderingResultChildWindowName, MyImGuiData::k_RenderingResultChildWindowSize, true);
	ImGui::Text("AlbedColor");
	ImGui::Image((void*)m_AlbedColorSRV, MyImGuiData::k_RenderingResultTextureSize);
	ImGui::Text("Normal");
	ImGui::Image((void*)m_NormalSRV, MyImGuiData::k_RenderingResultTextureSize);
	ImGui::Text("Metaric&Smoothness");
	ImGui::Image((void*)m_SpecularSRV, MyImGuiData::k_RenderingResultTextureSize);
	ImGui::Text("Depth");
	ImGui::Image((void*)m_DepthStancilSRV, MyImGuiData::k_RenderingResultTextureSize);
	ImGui::EndChild();

	ImGui::End();
}

bool DeferredRendering::GetShader()
{
	// 頂点シェーダーの取得
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_DEFERREDRENDERING);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_DEFERREDRENDERING);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_DEFERREDRENDERING);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}

bool DeferredRendering::CreatePolygon()
{
	Renderer& renderer = Renderer::GetInstance();
	HRESULT hr;

	// デバイス取得
	ID3D11Device* device = renderer.GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::CreatePolygon deviceがnullptr");
		return false;
	}

	VERTEX_TEXCOOD vertex[4];

	float width = static_cast<float>(SystemData::k_ScreenWidth);
	float height = static_cast<float>(SystemData::k_ScreenHeight);
	vertex[0] = { {0.0f		, 0.0f	, 0.0f},{0.0f, 0.0f} };
	vertex[1] = { {width	, 0.0f	, 0.0f},{1.0f, 0.0f} };
	vertex[2] = { {0.0f		, height, 0.0f},{0.0f, 1.0f} };
	vertex[3] = { {width	, height, 0.0f},{1.0f, 1.0f} };

	//頂点バッファ生成
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_TEXCOOD) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = vertex;

	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("DeferredRendering::CreatePolygon CreateBufferが失敗");
		return false;
	}

	// 四角形のインデックスを定義
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	//// バッファを作成
	D3D11_BUFFER_DESC indexBufferDesc = {};
	// インデックス情報の追加
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;        
	indexBufferDesc.ByteWidth = sizeof(WORD) * 6;       
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;                 

		//// リソースの設定
	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = index;

	// インデックスバッファを作成
	hr = device->CreateBuffer(&indexBufferDesc, &indexInitData, m_IndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("DeferredRendering::CreatePolygon CreateBufferが失敗");
		return false;
	}

	return true;
}