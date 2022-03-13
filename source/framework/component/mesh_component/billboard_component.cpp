#include"../../../system/main.h"
#include "billboard_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../actor/actor.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../component/cameracomponent/camera_component.h"

BillBoardComponent::BillBoardComponent(Actor * owner)
	:MeshComponent(owner)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_ShaderResourceView(nullptr)
	, m_SceneRenderManager(nullptr)
	, m_Position(0.0f,0.0f,0.0f)
	, m_Size(1.0f,1.0f)
	, m_BillBoardTextureConfig{ Conv_XM::Vector2f(0.0f,0.0f),Conv_XM::Vector2f(1.0f,1.0f) }
	, m_ShaftRock{}
{
	// ライトの影響を受けないためフォワードレンダリング
	m_RenderingWay = RENDERING_FORWARD;
}

BillBoardComponent::~BillBoardComponent()
{
}

void BillBoardComponent::Draw() const
{
	// 見えないならリターン
	if (m_Visible == false)
	{
		return;
	}

	// nullチェック
	if (m_Owner == nullptr || m_DeviceContext == nullptr || m_VertexBuffer == nullptr 
		|| m_IndexBuffer == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr 
		|| m_InputLayout == nullptr || m_ShaderResourceView == nullptr || m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Draw nullptr");
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
	CameraComponent* camera = m_SceneRenderManager->GetMainCameraComponent();
	// ビューマトリクス取得
	Conv_XM::Matrix4x4f matView = camera->GetViewMatrix();

	//ビュー行列の逆行列でなおかつ平行移動成分が0
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	matView._44 = 1.0f;

	DirectX::XMVECTOR determinant;
	Conv_XM::Matrix4x4f invView = XMMatrixInverse(&determinant, matView);

	// X軸回転情報の打ち消し
	if (m_ShaftRock[0] == true)
	{
		invView._11 = 1.0f;
		invView._12 = 0.0f;
		invView._13 = 0.0f;
	}
	if(m_ShaftRock[1] == true)
	{

		// Y軸回転情報の打ち消し
		invView._21 = 0.0f;
		invView._22 = 1.0f;
		invView._23 = 0.0f;
	}
	if (m_ShaftRock[2] == true)
	{
		// Z軸回転情報の打ち消し
		invView._31 = 0.0f;
		invView._32 = 0.0f;
		invView._33 = 1.0f;
	}

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)invView * (DirectX::XMMATRIX)mtxTranslate;
	renderer.SetWorldMatrix(mtxWorld);

	// マテリアルセット
	renderer.SetMaterial(m_Material);

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

bool BillBoardComponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init GetShaderが失敗");
		return false;
	}

	// メッシュ作成
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init CreateMeshが失敗");
		return false;
	}

	//　シーンレンダーマネージャー取得
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init currentSceneがnullptr");
		return false;
	}

	m_SceneRenderManager = currentScene->GetSceneRenderManager();
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init m_SceneRenderManagerがnullptr");
		return false;
	}

	return true;
}

bool BillBoardComponent::CreateMesh()
{
	// デバイスの取得
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init deviceがnullptr");
		return false;
	}

	HRESULT hr;

	// 頂点設定
	VERTEX_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f,  0.5f , 0.0f}, {m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[1] = { { 0.5f,  0.5f , 0.0f}, {m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[2] = { {-0.5f, -0.5f, 0.0f},  {m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
	vertex[3] = { { 0.5f, -0.5f, 0.0f},  {m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };


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
		Logger::GetInstance().SetLog("BillBoardComponent::Init CreateBuffer失敗");
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
		Logger::GetInstance().SetLog("BillBoardComponent::Init CreateBuffer失敗");
		return false;
	}

	return true;
}

bool BillBoardComponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool BillBoardComponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_UNLITTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_UNLITTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_UNLITTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
