
#include"../../../system/main.h"
#include "changeable_billboard_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../actor/actor.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../component/cameracomponent/camera_component.h"

ChangeableBillBoardComponent::ChangeableBillBoardComponent(Actor * owner)
	:BillBoardComponent(owner)
	, m_Color(1.0f,1.0f,1.0f,1.0f)
	, m_IsUpdate(true)
{
}

ChangeableBillBoardComponent::~ChangeableBillBoardComponent()
{
}

void ChangeableBillBoardComponent::Update()
{	
	// 更新フラグが立っているなら
	if (m_IsUpdate == true)
	{
		// 頂点更新
		UpdateVetexData();
	}
}

void ChangeableBillBoardComponent::Draw() const
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
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Draw nullptr");
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

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)invView * (DirectX::XMMATRIX)mtxTranslate;
	renderer.SetWorldMatrix(mtxWorld);

	// マテリアルセット
	renderer.SetMaterial(m_Material);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_COLOR_TEXCOOD);
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

void ChangeableBillBoardComponent::UpdateVetexData()
{
	//頂点書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	m_DeviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_COLOR_TEXCOOD* vertex = (VERTEX_COLOR_TEXCOOD*)msr.pData;
	vertex[0] = { {-0.5f,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[1] = { { 0.5f,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[2] = { {-0.5f, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
	vertex[3] = { { 0.5f, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
	m_DeviceContext->Unmap(m_VertexBuffer.Get(), 0);

	// 更新フラグを戻す
	m_IsUpdate = false;
}

bool ChangeableBillBoardComponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init GetShaderが失敗");
		return false;
	}

	// メッシュ作成
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init CreateMeshが失敗");
		return false;
	}

	//　シーンレンダーマネージャー取得
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init currentSceneがnullptr");
		return false;
	}

	m_SceneRenderManager = currentScene->GetSceneRenderManager();
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init m_SceneRenderManagerがnullptr");
		return false;
	}

	return true;
}

bool ChangeableBillBoardComponent::CreateMesh()
{
	// デバイスの取得
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::CreateMesh deviceがnullptr");
		return false;
	}

	HRESULT hr;

	//頂点バッファ生成
	//頂点書き換え用
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;//頂点書き換え用
	bd.ByteWidth = sizeof(VERTEX_COLOR_TEXCOOD) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//頂点書き換え用
	bd.MiscFlags = 0;

	hr = device->CreateBuffer(&bd, NULL, m_VertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::CreateMesh CreateBufferが失敗");
		return false;
	}

	// 四角形のインデックスを定義
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	//// バッファを作成
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	// インデックス情報の追加
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;                 // デフォルトアクセス
	bufferDesc.ByteWidth = sizeof(WORD) * 6;                // サイズはインデックスの数 6
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;         // インデックスバッファを使用
	bufferDesc.CPUAccessFlags = 0;                          // CPUのバッファへのアクセス拒否

		//// リソースの設定
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = index;

	// インデックスバッファを作成
	hr = device->CreateBuffer(&bufferDesc, &initData, m_IndexBuffer.GetAddressOf());
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::CreateMesh CreateBufferが失敗");
		return false;
	}

	return true;
}

bool ChangeableBillBoardComponent::GetShader()
{
	// 頂点シェーダーの取得
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_UNLITCOLORTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_UNLITCOLORTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_UNLITCOLORTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
