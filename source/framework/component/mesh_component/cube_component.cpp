
#include"../../../system/main.h"
#include "cube_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../../directx/resource/texture_manager.h"
#include"../../actor/actor.h"

CubeComponent::CubeComponent(Actor * owner)
	:MeshComponent(owner)
{
	m_IsShadowInput = true;
}

CubeComponent::~CubeComponent()
{
}

void CubeComponent::Draw() const
{
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//CBufferData::Material material = {};
	//material.Metaric = 0.0f;
	//material.Smoothness = 0.0f;
	//Renderer::GetInstance().SetMaterial(material);

	Conv_XM::Matrix4x4f mat = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);
	//マトリクス設定
	Renderer::GetInstance().SetWorldMatrix(m_Owner->GetWorldMatrix());

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//インデックスバッファ設定
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//テクスチャ設定
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_ShaderResourceView);

	// 使用するプリミティブタイプを設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer::GetInstance().GetDeviceContext()->DrawIndexed(36, 0, 0);
}

void CubeComponent::ShadowDraw() const
{
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(NULL, NULL, 0);

	Conv_XM::Matrix4x4f mat = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);
	//マトリクス設定
	Renderer::GetInstance().SetWorldMatrix(m_Owner->GetWorldMatrix());

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//インデックスバッファ設定
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//テクスチャ設定
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_ShaderResourceView);

	// 使用するプリミティブタイプを設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer::GetInstance().GetDeviceContext()->DrawIndexed(36, 0, 0);
}

bool CubeComponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::LoadData GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::LoadData GetShaderが失敗");
		return false;
	}

	m_ShaderResourceView = TextureManager::GetInstance().Load("asset/texture/purple.png");

	VERTEX_NORMAL_TEXCOOD vertex[24];

	//前
	{
		vertex[0].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
		vertex[0].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[0].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[1].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
		vertex[1].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[1].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[2].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
		vertex[2].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[2].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[3].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
		vertex[3].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[3].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//右
	{
		vertex[4].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
		vertex[4].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[4].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[5].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
		vertex[5].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[5].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[6].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
		vertex[6].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[6].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[7].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
		vertex[7].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[7].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//後ろ
	{
		vertex[8].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
		vertex[8].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[8].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[9].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
		vertex[9].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[9].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[10].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
		vertex[10].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[10].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[11].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
		vertex[11].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[11].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//上
	{
		vertex[12].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
		vertex[12].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[12].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[13].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
		vertex[13].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[13].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[14].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
		vertex[14].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[14].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[15].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
		vertex[15].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[15].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//左
	{
		vertex[16].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
		vertex[16].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[16].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[17].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
		vertex[17].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[17].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[18].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
		vertex[18].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[18].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[19].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
		vertex[19].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[19].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//下
	{
		vertex[20].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
		vertex[20].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[20].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[21].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
		vertex[21].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[21].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[22].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
		vertex[22].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[22].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[23].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
		vertex[23].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[23].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_NORMAL_TEXCOOD) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, m_VertexBuffer.GetAddressOf());


	WORD index[]{
	 0,  1,  2,     3,  2,  1,
	 4,  5,  6,     7,  6,  5,
	 8,  9, 10,    11, 10,  9,
	12, 13, 14,    15, 14, 13,
	16, 17, 18,    19, 18, 17,
	20, 21, 22,    23, 22, 21,
	};

	WORD indexcount = ARRAYSIZE(index);

	// インデックスバッファとして作成するための情報設定
	D3D11_BUFFER_DESC index_buffer_desc;
	index_buffer_desc.ByteWidth = sizeof(UWORD) * indexcount;            // バッファのサイズ(頂点数)
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;              // 使用方法
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;      // バッファの種類(インデックスバッファ)
	index_buffer_desc.CPUAccessFlags = 0;                       // CPUアクセス設定
	index_buffer_desc.MiscFlags = 0;                            // その他のオプション
	index_buffer_desc.StructureByteStride = 0;                  // 構造体ではないので0

	 // インデックスバッファの初期値指定
	D3D11_SUBRESOURCE_DATA index_init_data;
	// バッファの初期値設定
	index_init_data.pSysMem = index;
	// インデックスバッファでは使用しない
	index_init_data.SysMemPitch = 0;
	index_init_data.SysMemSlicePitch = 0;

	// インデックスバッファ作成
	Renderer::GetInstance().GetDevice()->CreateBuffer(&index_buffer_desc, &index_init_data, m_IndexBuffer.GetAddressOf());

	return true;
}

bool CubeComponent::GetDeviceContext()
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

bool CubeComponent::GetShader()
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
