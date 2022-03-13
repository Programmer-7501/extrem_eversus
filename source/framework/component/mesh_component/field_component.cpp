
#include"../../../system/main.h"
#include "field_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../actor/actor.h"

FieldComponent::FieldComponent(Actor * owner)
	:MeshComponent(owner)
{
}

FieldComponent::~FieldComponent()
{
}

void FieldComponent::Draw() const
{
	// 見えないならリターン
	if (m_Visible == false)
	{
		return;
	}

	// nullチェック
	if (m_Owner == nullptr || m_DeviceContext == nullptr || m_VertexBuffer == nullptr || m_IndexBuffer == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InputLayout == nullptr || m_ShaderResourceView == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false)
	{
		return;
	}

	Renderer& renderer = Renderer::GetInstance();

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	Conv_XM::Vector3f position = m_Owner->GetPosition();
	Conv_XM::Vector3f scale = m_Owner->GetScale();
	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)mtxTranslate;
	renderer.SetWorldMatrix(mtxWorld);

	// マテリアルセット
	renderer.SetMaterial(m_Material);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// インデックスバッファを設定
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//テクスチャ設定
	m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &m_ShaderResourceView);

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画
	m_DeviceContext->DrawIndexed(6, 0, 0);
}

bool FieldComponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetShaderが失敗");
		return false;
	}

	// メッシュ作成
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init CreateMeshが失敗");
		return false;
	}

	return true;
}

bool FieldComponent::CreateMesh()
{
	// デバイスの取得
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init deviceがnullptr");
		return false;
	}

	HRESULT hr;

	// 頂点設定
	VERTEX_NORMAL_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f, 0.0f, 0.5f},{0.0f,1.0f,0.0f}, {0.0f,0.0f} };
	vertex[1] = { { 0.5f, 0.0f, 0.5f},{0.0f,1.0f,0.0f}, {1.0f,0.0f} };
	vertex[2] = { {-0.5f, 0.0f, -0.5f}, {0.0f,1.0f,0.0f},{0.0f,1.0f} };
	vertex[3] = { { 0.5f, 0.0f, -0.5f}, {0.0f,1.0f,0.0f},{1.0f,1.0f} };


	// 頂点バッファ生成
	// 書き換え無し
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_NORMAL_TEXCOOD) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// リソースの設定
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = vertex;

	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init CreateBuffer失敗");
		return false;
	}

	// 四角形のインデックスを定義
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// バッファを作成
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	// インデックス情報の追加
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	// リソースの設定
	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = index;

	// インデックスバッファを作成
	hr = device->CreateBuffer(&indexBufferDesc, &indexInitData, m_IndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init CreateBuffer失敗");
		return false;
	}

	return true;
}

bool FieldComponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool FieldComponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_GBUFFERINPUT);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_GBUFFERINPUT);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_GBUFFERINPUT);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
