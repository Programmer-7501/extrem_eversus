
#include"../system/main.h"
#include "particle.h"
#include"../directx/renderer.h"
#include"../directx/resource/shader_manager.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene/scene.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../directx/render_data/srv_data.h"
#include"../directx/render_data/uav_data.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene/scene.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/component/cameracomponent/camera_component.h"
#include"../shader/shader_data.h"

namespace ParticleInfo
{
	static constexpr float k_Size = 1.0f;
	static constexpr int k_Amout = 1024;
}

Particle::Particle(int maxParticleNum)
	: m_DrawArgs(nullptr)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_DrawArgsUAV(nullptr)
	, m_ParticleBuffer(nullptr)
	, m_ParticleUAV(nullptr)
	, m_ParticleSRV(nullptr)
	, m_DeadParticleIndexBuffer(nullptr)
	, m_DeadParticleIndexAppendConsumeUAV(nullptr)
	, m_AlliveParticleIndexBuffer(nullptr)
	, m_AlliveParticleIndexUAV(nullptr)
	, m_AlliveParticleIndexSRV(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_ShaderResourceView(nullptr)
	, m_InitComputeShader(nullptr)
	, m_UpdateComputeShader(nullptr)
	, m_SceneRenderManager(nullptr)
	, m_MaxParticleNum(maxParticleNum)
	, m_Dispatch(0)
{
	m_Dispatch = m_MaxParticleNum / PARTICLE_UPDATE_THREDNUM;
}


Particle::~Particle()
{
}

void Particle::Update()
{

}

void Particle::Draw() const
{
	// パーティクル更新
	UpdateParticle();

	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Draw m_SceneRenderManagerがnullptr");
		return;
	}

	Renderer& renderer = Renderer::GetInstance();

	CameraComponent* camera = m_SceneRenderManager->GetMainCameraComponent();

	// nullチェック
	if (m_DeviceContext == nullptr || m_VertexBuffer.Get() == nullptr || m_IndexBuffer == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InputLayout == nullptr || m_ShaderResourceView == nullptr || m_SceneRenderManager == nullptr || camera == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Draw nullptr");
		return;
	}

	//マトリクス設定
	Conv_XM::Matrix4x4f viewMatrix = camera->GetViewMatrix();

	//ビュー行列の逆行列でなおかつ平行移動成分が0
	Conv_XM::Matrix4x4f invView = DirectX::XMMatrixInverse(nullptr, viewMatrix);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;
	invView._44 = 1.0f;

	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(0.0f, 5.0f, 0.0f);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)invView * (DirectX::XMMATRIX)mtxTranslate;
	renderer.SetWorldMatrix(mtxWorld);


	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// インデックスバッファを設定
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	m_DeviceContext->VSSetShaderResources(SRVData::k_ParticleDataShaderResourceViewIndex, 1, m_ParticleSRV.GetAddressOf());
	m_DeviceContext->VSSetShaderResources(SRVData::k_AlliveParticleIndexShaderResourceViewIndex, 1, m_AlliveParticleIndexSRV.GetAddressOf());

	//テクスチャ設定
	m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &m_ShaderResourceView);

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 透過加算合成にする
	renderer.SetBlendState(Renderer::BLENDSTATE_INVISIBLEADD);

	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_NOWRITE);

	// 描画
	m_DeviceContext->DrawIndexedInstancedIndirect(m_DrawArgs.Get(), 0);

	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_STANDARD);

	// デフォルトにする
	renderer.SetBlendState(Renderer::BLENDSTATE_DEFAULT);
}

void Particle::Emitter(int spawnNum)
{
	if (m_DeadParticleIndexAppendConsumeUAV == nullptr ||
		m_ParticleUAV == nullptr
		)
	{
		Logger::GetInstance().SetLog("Particle::Emitter nullptr");
		return;
	}

	ID3D11UnorderedAccessView* uavs[] = { m_ParticleUAV.Get(),m_DeadParticleIndexAppendConsumeUAV.Get() };

	// パーティクルデータを登録
	// 現在の非表示カウンターを保持
	UINT init[] = { static_cast<UINT>(-1), static_cast<UINT>(-1) };
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDATA_INDEX, ARRAYSIZE(uavs), uavs, init);

	int dispatch = (spawnNum / PARTICLE_EMITTER_THREDNUM) + 1;
	// 実行
	m_DeviceContext->Dispatch(dispatch, 1, 1);

	ZeroMemory(uavs, sizeof(uavs));
	//登録解除s
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDATA_INDEX, ARRAYSIZE(uavs), uavs, init);
}

bool Particle::Init()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("Particle::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("Particle::Init GetShaderが失敗");
		return false;
	}

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


	//---------------------------------------------------------------------------------------------
	//      インスタンス情報の作成
	//---------------------------------------------------------------------------------------------
	UINT initDrawArgs[5];
	initDrawArgs[0] = 6;//IndexCountPerInstance;
	initDrawArgs[1] = 0;//InstanceCount
	initDrawArgs[2] = 0;//StartIndexLocation
	initDrawArgs[3] = 0;//BaseVertexLocation
	initDrawArgs[3] = 0;//StartInstanceLocation

	D3D11_BUFFER_DESC instanceBufferDesc = {};
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	instanceBufferDesc.ByteWidth = 5 * sizeof(UINT);
	instanceBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = initDrawArgs;

	hr = Renderer::GetInstance().GetDevice()->CreateBuffer(&instanceBufferDesc, &data, m_DrawArgs.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBufferに失敗");
		return false;
	}



	//---------------------------------------------------------------------------------------------
	//      インスタンス情報UAVの作成
	//---------------------------------------------------------------------------------------------

	D3D11_UNORDERED_ACCESS_VIEW_DESC instanceUAVDesc = {};

	instanceUAVDesc.Format = DXGI_FORMAT_R32_UINT;
	instanceUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	instanceUAVDesc.Buffer.FirstElement = 0;
	instanceUAVDesc.Buffer.Flags = 0;
	instanceUAVDesc.Buffer.NumElements = 5;


	hr = device->CreateUnorderedAccessView(m_DrawArgs.Get(), &instanceUAVDesc, m_DrawArgsUAV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateUnorderedAccessViewに失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      パーティクル情報の作成
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC particleBufferDesc = {};

	particleBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	particleBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	particleBufferDesc.ByteWidth = sizeof(ParticleData) * m_MaxParticleNum;
	particleBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	particleBufferDesc.StructureByteStride = sizeof(ParticleData);

	hr = device->CreateBuffer(&particleBufferDesc, nullptr, m_ParticleBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBufferに失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      パーティクル情報UAVの作成
	//---------------------------------------------------------------------------------------------
	D3D11_UNORDERED_ACCESS_VIEW_DESC particleBufferUAVDesc = {};

	particleBufferUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	particleBufferUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	particleBufferUAVDesc.Buffer.FirstElement = 0;
	particleBufferUAVDesc.Buffer.Flags = 0;
	particleBufferUAVDesc.Buffer.NumElements = m_MaxParticleNum;

	hr = device->CreateUnorderedAccessView(m_ParticleBuffer.Get(), &particleBufferUAVDesc, m_ParticleUAV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceViewに失敗");
		return false;
	}


	//---------------------------------------------------------------------------------------------
	//      パーティクル情報SRVの作成
	//---------------------------------------------------------------------------------------------

	D3D11_SHADER_RESOURCE_VIEW_DESC particleBufferSRVDesc = {};
	particleBufferSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	particleBufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	particleBufferSRVDesc.Buffer.FirstElement = 0;
	particleBufferSRVDesc.Buffer.NumElements = m_MaxParticleNum;

	hr = device->CreateShaderResourceView(m_ParticleBuffer.Get(), &particleBufferSRVDesc, m_ParticleSRV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceViewに失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      死亡パーティクルインデックスバッファの作成
	//---------------------------------------------------------------------------------------------

	D3D11_BUFFER_DESC deadParticleIndexBufferDesc = {};
	deadParticleIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	deadParticleIndexBufferDesc.ByteWidth = sizeof(UINT) * m_MaxParticleNum;
	deadParticleIndexBufferDesc.StructureByteStride = sizeof(UINT);
	deadParticleIndexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	deadParticleIndexBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;

	hr = Renderer::GetInstance().GetDevice()->CreateBuffer(&deadParticleIndexBufferDesc, nullptr, m_DeadParticleIndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBufferに失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      死亡パーティクルUAVの作成
	//---------------------------------------------------------------------------------------------

	D3D11_UNORDERED_ACCESS_VIEW_DESC deadParticleIndexUavDesc = {};

	deadParticleIndexUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	deadParticleIndexUavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	deadParticleIndexUavDesc.Buffer.FirstElement = 0;
	deadParticleIndexUavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
	deadParticleIndexUavDesc.Buffer.NumElements = m_MaxParticleNum;


	hr = device->CreateUnorderedAccessView(m_DeadParticleIndexBuffer.Get(), &deadParticleIndexUavDesc, m_DeadParticleIndexAppendConsumeUAV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateUnorderedAccessViewに失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      生存パーティクルの作成
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC alliveParticleIndexBufferDesc = {};
	alliveParticleIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	alliveParticleIndexBufferDesc.ByteWidth = sizeof(UINT) * m_MaxParticleNum;
	alliveParticleIndexBufferDesc.StructureByteStride = sizeof(UINT);
	alliveParticleIndexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	alliveParticleIndexBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

	hr = Renderer::GetInstance().GetDevice()->CreateBuffer(&alliveParticleIndexBufferDesc, nullptr, m_AlliveParticleIndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBufferに失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      生存パーティクルUAVの作成
	//---------------------------------------------------------------------------------------------

	D3D11_UNORDERED_ACCESS_VIEW_DESC alliveParticleIndexUavDesc = {};

	alliveParticleIndexUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	alliveParticleIndexUavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	alliveParticleIndexUavDesc.Buffer.FirstElement = 0;
	alliveParticleIndexUavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
	alliveParticleIndexUavDesc.Buffer.NumElements = m_MaxParticleNum;


	hr = device->CreateUnorderedAccessView(m_AlliveParticleIndexBuffer.Get(), &alliveParticleIndexUavDesc, m_AlliveParticleIndexUAV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateUnorderedAccessViewに失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      生存パーティクル情報SRVの作成
	//---------------------------------------------------------------------------------------------

	D3D11_SHADER_RESOURCE_VIEW_DESC alliveParticleIndexSRVDesc = {};
	alliveParticleIndexSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	alliveParticleIndexSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	alliveParticleIndexSRVDesc.Buffer.FirstElement = 0;
	alliveParticleIndexSRVDesc.Buffer.NumElements = m_MaxParticleNum;

	hr = device->CreateShaderResourceView(m_AlliveParticleIndexBuffer.Get(), &alliveParticleIndexSRVDesc, m_AlliveParticleIndexSRV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceViewに失敗");
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

	// パーティクルデータの初期化
	if (InitParticleData() == false)
	{
		Logger::GetInstance().SetLog("Particle::Init InitParticleDataが失敗");
		return false;
	}

	return true;
}

bool Particle::InitParticleData()
{
	if (m_InitComputeShader == nullptr || m_DeadParticleIndexAppendConsumeUAV == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::InitParticleData nullptr");
		return false;
	}

	// シェーダーセット
	m_DeviceContext->CSSetShader(m_InitComputeShader, nullptr, 0);

	// 死亡インデックスセット
	UINT init[] = { static_cast<UINT>(0) };
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::DEADPARTICLE_INDEX, 1, m_DeadParticleIndexAppendConsumeUAV.GetAddressOf(), init);

	// 実行
	m_DeviceContext->Dispatch(m_Dispatch, 1, 1);

	return true;
}

void Particle::UpdateParticle() const
{
	if (m_UpdateComputeShader == nullptr || m_DrawArgsUAV == nullptr || m_DeadParticleIndexAppendConsumeUAV == nullptr || m_AlliveParticleIndexUAV == nullptr || m_ParticleUAV == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::UpdateParticle nullptr");
		return;
	}

	// シェーダーセット
	m_DeviceContext->CSSetShader(m_UpdateComputeShader, nullptr, 0);

	// 現在の非表示カウンターを保持
	ID3D11UnorderedAccessView* uavs[] = { m_DrawArgsUAV.Get(), m_ParticleUAV.Get(), m_DeadParticleIndexAppendConsumeUAV.Get(), m_AlliveParticleIndexUAV.Get() };
	UINT init[] = { static_cast<UINT>(-1), static_cast<UINT>(-1),static_cast<UINT>(-1),static_cast<UINT>(0) };
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDRAWARGS_INDEX, ARRAYSIZE(uavs), uavs, init);

	// 実行
	m_DeviceContext->Dispatch(m_Dispatch, 1, 1);

	// 現在の非表示カウンターを保持
	ZeroMemory(uavs, sizeof(uavs));
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDRAWARGS_INDEX, ARRAYSIZE(uavs), uavs, init);
}

bool Particle::GetDeviceContext()
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

bool Particle::GetShader()
{
	// シェーダーマネージャー取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_PARTICLE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_PARTICLE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_PARTICLE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	// 初期化コンピュートシェーダーの取得
	m_InitComputeShader = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_INIT);
	if (m_InitComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_ComputeShaderがnullptr");
		return false;
	}

	// 更新コンピュートシェーダーの取得
	m_UpdateComputeShader = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_UPDATE);
	if (m_UpdateComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_ComputeShaderがnullptr");
		return false;
	}

	return true;
}