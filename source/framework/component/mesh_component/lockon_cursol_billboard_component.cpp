
#include"../../../system/main.h"
#include "lockon_cursol_billboard_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../cameracomponent/camera_component.h"

LockonCursolBillboardComponent::LockonCursolBillboardComponent(Actor * owner)
	:MeshComponent(owner)
	, m_DeviceContext(nullptr)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_Position(0.0f,0.0f,0.0f)
	, m_Size(1.0f,1.0f)
	, m_SceneRenderManager(nullptr)
	, m_LockOnState(LOCKONSTATE_GREEN)
{
	m_RenderingWay = RENDERING_FORWARD;
}

void LockonCursolBillboardComponent::Draw() const
{
	if (m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_VertexBuffer == nullptr || m_IndexBuffer == nullptr || m_ShaderResourceView == nullptr)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::Draw nullptr");
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

	// カメラコンポーネント取得
	class CameraComponent* cameraComponent = m_SceneRenderManager->GetMainCameraComponent();
	if (cameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::Draw cameraComponentがnullptr");
		return;
	}
	//マトリクス設定
	Conv_XM::Matrix4x4f matView = cameraComponent->GetViewMatrix();

	//ビュー行列の逆行列でなおかつ平行移動成分が0
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	matView._44 = 1.0f;

	DirectX::XMVECTOR determinant;
	Conv_XM::Matrix4x4f matInvView = DirectX::XMMatrixInverse(&determinant, matView);

	Conv_XM::Matrix4x4f mtxWorld, mtxScaling, mtxTranslate;
	mtxScaling = DirectX::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f);
	mtxTranslate = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	mtxWorld = (DirectX::XMMATRIX)mtxScaling * (DirectX::XMMATRIX)matInvView * (DirectX::XMMATRIX)mtxTranslate;

	renderer.SetWorldMatrix(mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// 表示するインデックスバッファを選択
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//テクスチャ設定
	m_DeviceContext->PSSetShaderResources(0, 1, &m_ShaderResourceView[m_LockOnState]);

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 深度値を無効にする
	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_NOWRITE_REFERENCE);

	//描画
	m_DeviceContext->DrawIndexed(6, 0, 0);

	// 深度値を戻す
	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_STANDARD);
}

bool LockonCursolBillboardComponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::LoadData GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::LoadData GetShaderが失敗");
		return false;
	}

	// メッシュの制作
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::LoadData CreateMeshが失敗");
		return false;
	}

	return true;
}

bool LockonCursolBillboardComponent::CreateMesh()
{
	// デバイスの取得
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init deviceがnullptr");
		return false;
	}

	HRESULT hr;

	//---------------------------------------------------------------------------------------------
	//      頂点バッファの作成
	//---------------------------------------------------------------------------------------------

	// 頂点設定
	VERTEX_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f, 0.5f,0.0f}, {0.0f,0.0f} };
	vertex[1] = { { 0.5f, 0.5f,0.0f}, {1.0f,0.0f} };
	vertex[2] = { {-0.5f, -0.5f ,0.0f}, {0.0f,1.0f} };
	vertex[3] = { { 0.5f, -0.5f ,0.0f}, {1.0f,1.0f} };


	// 頂点バッファ生成
	// 書き換え無し
	D3D11_BUFFER_DESC vertexBufferDesc = {};
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      インデックスバッファの作成
	//---------------------------------------------------------------------------------------------

	// 四角形のインデックスを定義
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// バッファを作成
	D3D11_BUFFER_DESC indexBufferDesc = {};
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer失敗");
		return false;
	}

	return true;
}

bool LockonCursolBillboardComponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool LockonCursolBillboardComponent::GetShader()
{
	// シェーダーマネージャー取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_UNLITTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_UNLITTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_UNLITTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	//　シーンレンダーマネージャー取得
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init currentSceneがnullptr");
		return false;
	}

	m_SceneRenderManager = currentScene->GetSceneRenderManager();
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init m_SceneRenderManagerがnullptr");
		return false;
	}

	return true;
}
