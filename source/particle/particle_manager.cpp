
#include"../system/main.h"
#include"particle_manager.h"
#include"particle.h"
#include"../directx/renderer.h"
#include"../directx/render_data/srv_data.h"
#include"../directx/resource/shader_manager.h"

void ParticleManager::Init()
{
	// 変数の初期化
	m_EmitterIndexCount = 0;

	for (int i = 0; i < EMITTER_MAXNUM; i++)
	{
		m_Emitters[i] = {};
	}

	m_EmitterArrayIsUpdate = true;

	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_DeviceContextがnullptr");
		return;
	}

	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init deviceがnullptr");
		return;
	}

	//---------------------------------------------------------------------------------------------
	//      スフィアエミッターコンピュートシェーダーの取得
	//---------------------------------------------------------------------------------------------
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	m_EmitterComputeShader[CBufferData::ET_SPHERE] = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_CIRCLE_EMITTER);
	if (m_EmitterComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_ComputeShaderがnullptr");
		return;
	}

	//---------------------------------------------------------------------------------------------
	//      レクトエミッターコンピュートシェーダーの取得
	//---------------------------------------------------------------------------------------------
	m_EmitterComputeShader[CBufferData::ET_RECT] = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_RECT_EMITTER);
	if (m_EmitterComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_ComputeShaderがnullptr");
		return;
	}


	//---------------------------------------------------------------------------------------------
	//      レクトエミッターコンピュートシェーダーの取得
	//---------------------------------------------------------------------------------------------
	m_EmitterComputeShader[CBufferData::ET_DEMO] = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_DEMO_EMITTER);
	if (m_EmitterComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_ComputeShaderがnullptr");
		return;
	}

	HRESULT hr;
	//---------------------------------------------------------------------------------------------
	//      パーティクルエミッター配列のコンスタントバッファ
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC particleEmitterArrayBufferDesc = {};

	particleEmitterArrayBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	particleEmitterArrayBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	particleEmitterArrayBufferDesc.CPUAccessFlags = 0;
	particleEmitterArrayBufferDesc.ByteWidth = sizeof(CBufferData::ParticleEmitter) * EMITTER_MAXNUM;

	// リソースの設定
	D3D11_SUBRESOURCE_DATA particleEmitterArrayInitData;
	particleEmitterArrayInitData.pSysMem = m_Emitters;

	hr = device->CreateBuffer(&particleEmitterArrayBufferDesc, &particleEmitterArrayInitData, m_EmitterArrayConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ParticleManager::Init CreateBufferが失敗");
		return;
	}

	// コンピュートシェーダーにパーティクルエミッター配列バッファを登録
	m_DeviceContext->CSSetConstantBuffers(CBufferData::PARTICLEEMITTERARRAY_INDEX, 1, m_EmitterArrayConstantBuffer.GetAddressOf());

	//---------------------------------------------------------------------------------------------
	//      死亡パーティクル数コンスタントバッファの作成
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC deadParticleNumBufferDesc = {};

	deadParticleNumBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	deadParticleNumBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	deadParticleNumBufferDesc.CPUAccessFlags = 0;
	deadParticleNumBufferDesc.ByteWidth = 4 * sizeof(UINT);
	device->CreateBuffer(&deadParticleNumBufferDesc, nullptr, DeadParticleNumConstantBuffer.GetAddressOf());
	// 死亡数登録
	m_DeviceContext->CSSetConstantBuffers(CBufferData::DEADPARTICLENUMBER_INDEX, 1, DeadParticleNumConstantBuffer.GetAddressOf());

	//---------------------------------------------------------------------------------------------
	//      パーティクルエミッターのコンスタントバッファ
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC particleEmitterBufferDesc = {};

	particleEmitterBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	particleEmitterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	particleEmitterBufferDesc.CPUAccessFlags = 0;
	particleEmitterBufferDesc.ByteWidth = sizeof(CBufferData::ParticleEmitter);

	hr = device->CreateBuffer(&particleEmitterBufferDesc, nullptr, m_EmitterConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ParticleManager::Init CreateBufferが失敗");
		return;
	}

	// コンピュートシェーダーにパーティクルエミッターバッファを登録
	m_DeviceContext->CSSetConstantBuffers(CBufferData::USEPARTICLEEMITTER_INDEX, 1, m_EmitterConstantBuffer.GetAddressOf());


	//---------------------------------------------------------------------------------------------
	//      乱数テクスチャの作成
	//---------------------------------------------------------------------------------------------

	D3D11_TEXTURE2D_DESC randomTextureDesc = {};
	randomTextureDesc.Width = 1024;
	randomTextureDesc.Height = 1024;
	randomTextureDesc.ArraySize = 1;
	randomTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	randomTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	randomTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	randomTextureDesc.MipLevels = 1;
	randomTextureDesc.SampleDesc.Count = 1;
	randomTextureDesc.SampleDesc.Quality = 0;

	float* values = new float[randomTextureDesc.Width * randomTextureDesc.Height * 4];
	float* ptr = values;
	for (UINT i = 0; i < randomTextureDesc.Width * randomTextureDesc.Height; i++)
	{
		ptr[0] = Math::GenerateZeroToOneRand();
		ptr[1] = Math::GenerateZeroToOneRand();
		ptr[2] = Math::GenerateZeroToOneRand();
		ptr[3] = Math::GenerateZeroToOneRand();
		ptr += 4;
	}

	D3D11_SUBRESOURCE_DATA randomTextureData;
	randomTextureData.pSysMem = values;
	randomTextureData.SysMemPitch = randomTextureDesc.Width * 16;
	randomTextureData.SysMemSlicePitch = 0;

	hr = device->CreateTexture2D(&randomTextureDesc, &randomTextureData, m_RandomTexture.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateTexture2Dが失敗");
		return;
	}

	delete[] values;


	//---------------------------------------------------------------------------------------------
	//      乱数テクスチャSRVの作成
	//---------------------------------------------------------------------------------------------

	D3D11_SHADER_RESOURCE_VIEW_DESC randomTextureSRV;
	randomTextureSRV.Format = randomTextureDesc.Format;
	randomTextureSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	randomTextureSRV.Texture2D.MipLevels = 1;
	randomTextureSRV.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(m_RandomTexture.Get(), &randomTextureSRV, m_RandomTextureSRV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceViewが失敗");
		return;
	}

	// シェーダーリソースビューをセット
	m_DeviceContext->CSSetShaderResources(SRVData::k_RandomTextureShaderResourceViewIndex, 1, m_RandomTextureSRV.GetAddressOf());


	//---------------------------------------------------------------------------------------------
	//      デバッグ用バッファの作成
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC debugBufferDesc = {};
	debugBufferDesc.Usage = D3D11_USAGE_STAGING;
	debugBufferDesc.BindFlags = 0;
	debugBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	debugBufferDesc.ByteWidth = sizeof(UINT);
	hr = device->CreateBuffer(&debugBufferDesc, nullptr, &m_DebugCounterBuffer);
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBufferが失敗");
		return;
	}
}

void ParticleManager::Uninit()
{
	for (auto&& particle : m_ParticleMap)
	{
		if (particle.second)
		{
			delete particle.second;
			particle.second = nullptr;
		}
	}
	m_ParticleMap.clear();
}

void ParticleManager::Reset()
{
	// 変数の初期化
	m_EmitterIndexCount = 0;

	for (int i = 0; i < EMITTER_MAXNUM; i++)
	{
		m_Emitters[i] = {};
	}

	m_EmitterArrayIsUpdate = true;

	for (auto&& particle : m_ParticleMap)
	{
		if (particle.second)
		{
			delete particle.second;
			particle.second = nullptr;
		}
	}
	m_ParticleMap.clear();
}

void ParticleManager::SpawnParticle(const std::string & particleName, int emitterIndex)
{
	Renderer& renderer = Renderer::GetInstance();
	renderer.SetRandomNumber(Math::GenerateZeroToOneRand());

	// パーティクルエミッターのコンスタントバッファセット
	int spawnNum = SetParticleEmitterConstantBuffer(emitterIndex);

	auto&& iter = m_ParticleMap.find(particleName);

	if (iter == m_ParticleMap.end())
	{
		Logger::GetInstance().SetLog("ParticleManager::SpawnParticle マップに登録なし");
		return;
	}
	if (iter->second)
	{
		ID3D11UnorderedAccessView* deadParticleIndexUAV = iter->second->GetDeadParticleIndexAppendConsumeUAV();

		// 死亡数を取得
		m_DeviceContext->CopyStructureCount(DeadParticleNumConstantBuffer.Get(), 0, deadParticleIndexUAV);

		// エミッターを指定
		if (m_Emitters[emitterIndex].EmitterType < 0 || m_Emitters[emitterIndex].EmitterType > CBufferData::ET_MAX)
		{
			Logger::GetInstance().SetLog("ParticleManager::SpawnParticle エミッターのタイプが正しく指定されていない");
			return;
		}
		// シェーダーセット
		m_DeviceContext->CSSetShader(m_EmitterComputeShader[m_Emitters[emitterIndex].EmitterType], nullptr, 0);

		iter->second->Emitter(spawnNum);
	}
}

void ParticleManager::Update()
{
	// エミッター配列の更新
	UpdateParticleEmitterArrayConstantBuffer();

	if (m_IsDebugCountView == false)
	{
		return;
	}

	auto&& iter = m_ParticleMap.find(ParticleManagerData::k_BeamParticleName);

	ID3D11UnorderedAccessView* uav = iter->second->GetDeadParticleIndexAppendConsumeUAV();
	int deadParticleNum = ReadCounter(uav);
	int maxParticleNum = iter->second->GetMaxParticleNumber();
	int alliveParticleNum = maxParticleNum - deadParticleNum;
	ImGui::Begin("SceneInfo");
	ImGui::Text("Particle AlliveParticleNum %d", alliveParticleNum);
	ImGui::Text("Particle DeadParticleNum %d", deadParticleNum);
	ImGui::End();
}

void ParticleManager::Draw() const
{
	for (auto&& particle : m_ParticleMap)
	{
		if (particle.second)
		{
			particle.second->Draw();
		}
	}
}

void ParticleManager::UpdateParticleEmitterArrayConstantBuffer()
{
	if (m_EmitterArrayIsUpdate)
	{
		// エミッターセット
		m_DeviceContext->UpdateSubresource(m_EmitterArrayConstantBuffer.Get(), 0, NULL, m_Emitters, 0, 0);

		m_EmitterArrayIsUpdate = false;
	}
}

void ParticleManager::CreateParticle(const std::string & particleName,int maxParticleNum, ID3D11ShaderResourceView* srv)
{
	// パーティクル作成
	Particle* particle = new Particle(maxParticleNum);
	// nullチェック
	if (particle == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::CreateParticle particleがnullptr");
		return;
	}

	// テクスチャ設定
	particle->SetTexture(srv);
	// 初期化処理
	particle->Init();
	// 登録
	m_ParticleMap.emplace(particleName, particle);

}

void ParticleManager::UpdateParticleEmitterConstantBuffer(int emitterIndex, const CBufferData::ParticleEmitter & particleEmitter)
{
	if (emitterIndex < 0 || emitterIndex >= EMITTER_MAXNUM)
	{

		Logger::GetInstance().SetLog("ParticleManager::GetParticleEmitter 取得したインデックスが最大数を超えてる");
		return;
	}

	// 入力
	m_Emitters[emitterIndex] = particleEmitter;
	m_Emitters[emitterIndex].MyIndex = emitterIndex;

	// 更新フラグを立てる
	m_EmitterArrayIsUpdate = true;
}

int ParticleManager::SetParticleEmitterConstantBuffer(int emitterIndex)
{
	if (emitterIndex < 0 || emitterIndex >= EMITTER_MAXNUM)
	{
		Logger::GetInstance().SetLog("ParticleManager::SetParticleEmitterConstantBuffer 取得したインデックスが最大数を超えてる");
		return 0;
	}

	CBufferData::ParticleEmitter emitter = m_Emitters[emitterIndex];
	// エミッターセット
	m_DeviceContext->UpdateSubresource(m_EmitterConstantBuffer.Get(), 0, NULL, &emitter, 0, 0);

	// 更新フラグを立てる
	m_EmitterArrayIsUpdate = true;

	return emitter.MaxSpawnParticlesThisFrame;
}

int ParticleManager::ReadCounter(ID3D11UnorderedAccessView * uav)
{
	int count = 0;

	// UAVのカウンターをデバッグ用バッファにコピーする
	m_DeviceContext->CopyStructureCount(m_DebugCounterBuffer, 0, uav);

	// デバッグ用バッファの中身を取り出す
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	m_DeviceContext->Map(m_DebugCounterBuffer, 0, D3D11_MAP_READ, 0, &MappedResource);

	// データを読み込む
	count = *(int*)MappedResource.pData;

	m_DeviceContext->Unmap(m_DebugCounterBuffer, 0);

	return count;
}
