
#include"../../../system/main.h"
#include"sprite_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../../framework/scene/scene_manager.h"
#include"../../../framework/scene/scene.h"
#include"../../../framework/scene_function/scene_render_manager.h"
#include"../../../directx/render_data/srv_data.h"

SpriteComponent::SpriteComponent(Actor * owner)
	:Component(owner)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_DrawOrder(DM_DEFAULT)
	, m_Visible(true)
	, m_SpriteTextureConfig{ Conv_XM::Vector2f(0.0f,0.0f),Conv_XM::Vector2f(1.0f,1.0f)}
	, m_Position(Conv_XM::Vector2f(0.0f,0.0f))
	, m_Size(Conv_XM::Vector2f(1.0f,1.0f))
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_ShaderResourceView(nullptr)
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Init()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init GetDeviceInterfaceが失敗");
		return;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init GetShaderが失敗");
		return;
	}

	// デバイスの取得
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init deviceがnullptr");
		return;
	}


	HRESULT hr;

	// 頂点設定
	VERTEX_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f, -0.5f,0.0f}, {m_SpriteTextureConfig.TexCut.x,m_SpriteTextureConfig.TexCut.y} };
	vertex[1] = { { 0.5f, -0.5f,0.0f}, {m_SpriteTextureConfig.TexCut.x + m_SpriteTextureConfig.TexCutSize.x,m_SpriteTextureConfig.TexCut.y} };
	vertex[2] = { {-0.5f, 0.5f,0.0f}, {m_SpriteTextureConfig.TexCut.x,m_SpriteTextureConfig.TexCut.y + m_SpriteTextureConfig.TexCutSize.y} };
	vertex[3] = { { 0.5f, 0.5f,0.0f}, {m_SpriteTextureConfig.TexCut.x + m_SpriteTextureConfig.TexCutSize.x,m_SpriteTextureConfig.TexCut.y + m_SpriteTextureConfig.TexCutSize.y} };


	// 頂点バッファ生成
	// 書き換え無し
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_TEXCOOD) * 4;
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
		return;
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
		return;
	}

	// シーンレンダーマネージャーに登録
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init currentSceneがnullptr");
		return;
	}
	SceneRenderManager* sceneRenderManager = currentScene->GetSceneRenderManager();
	if (sceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init sceneRenderManagerがnullptr");
		return;
	}
	sceneRenderManager->AddSpriteComponent(this);
}

void SpriteComponent::Uninit()
{
}

void SpriteComponent::Draw() const
{
	// 見えないならリターン
	if (m_Visible == false)
	{
		return;
	}

	// nullチェック
	if (m_DeviceContext == nullptr || m_VertexBuffer == nullptr || m_IndexBuffer == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InputLayout == nullptr || m_ShaderResourceView == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Draw nullptr");
		return;
	}

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, 0.0f);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)mtxTranslate;
	Renderer::GetInstance().SetWorldMatrix(mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_TEXCOOD);
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

bool SpriteComponent::GetDeviceContext()
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

bool SpriteComponent::GetShader()
{
	// 頂点シェーダーの取得
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_UNLITTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_UNLITTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_UNLITTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
