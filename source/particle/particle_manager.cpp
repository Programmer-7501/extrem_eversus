
#include"../system/main.h"
#include"particle_manager.h"
#include"particle.h"
#include"../directx/renderer.h"
#include"../directx/render_data/srv_data.h"
#include"../directx/resource/shader_manager.h"

void ParticleManager::Init()
{
	// �ϐ��̏�����
	m_EmitterIndexCount = 0;

	for (int i = 0; i < EMITTER_MAXNUM; i++)
	{
		m_Emitters[i] = {};
	}

	m_EmitterArrayIsUpdate = true;

	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_DeviceContext��nullptr");
		return;
	}

	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// null�`�F�b�N
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init device��nullptr");
		return;
	}

	//---------------------------------------------------------------------------------------------
	//      �X�t�B�A�G�~�b�^�[�R���s���[�g�V�F�[�_�[�̎擾
	//---------------------------------------------------------------------------------------------
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	m_EmitterComputeShader[CBufferData::ET_SPHERE] = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_CIRCLE_EMITTER);
	if (m_EmitterComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_ComputeShader��nullptr");
		return;
	}

	//---------------------------------------------------------------------------------------------
	//      ���N�g�G�~�b�^�[�R���s���[�g�V�F�[�_�[�̎擾
	//---------------------------------------------------------------------------------------------
	m_EmitterComputeShader[CBufferData::ET_RECT] = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_RECT_EMITTER);
	if (m_EmitterComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_ComputeShader��nullptr");
		return;
	}


	//---------------------------------------------------------------------------------------------
	//      ���N�g�G�~�b�^�[�R���s���[�g�V�F�[�_�[�̎擾
	//---------------------------------------------------------------------------------------------
	m_EmitterComputeShader[CBufferData::ET_DEMO] = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_DEMO_EMITTER);
	if (m_EmitterComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::Init m_ComputeShader��nullptr");
		return;
	}

	HRESULT hr;
	//---------------------------------------------------------------------------------------------
	//      �p�[�e�B�N���G�~�b�^�[�z��̃R���X�^���g�o�b�t�@
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC particleEmitterArrayBufferDesc = {};

	particleEmitterArrayBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	particleEmitterArrayBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	particleEmitterArrayBufferDesc.CPUAccessFlags = 0;
	particleEmitterArrayBufferDesc.ByteWidth = sizeof(CBufferData::ParticleEmitter) * EMITTER_MAXNUM;

	// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA particleEmitterArrayInitData;
	particleEmitterArrayInitData.pSysMem = m_Emitters;

	hr = device->CreateBuffer(&particleEmitterArrayBufferDesc, &particleEmitterArrayInitData, m_EmitterArrayConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ParticleManager::Init CreateBuffer�����s");
		return;
	}

	// �R���s���[�g�V�F�[�_�[�Ƀp�[�e�B�N���G�~�b�^�[�z��o�b�t�@��o�^
	m_DeviceContext->CSSetConstantBuffers(CBufferData::PARTICLEEMITTERARRAY_INDEX, 1, m_EmitterArrayConstantBuffer.GetAddressOf());

	//---------------------------------------------------------------------------------------------
	//      ���S�p�[�e�B�N�����R���X�^���g�o�b�t�@�̍쐬
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC deadParticleNumBufferDesc = {};

	deadParticleNumBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	deadParticleNumBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	deadParticleNumBufferDesc.CPUAccessFlags = 0;
	deadParticleNumBufferDesc.ByteWidth = 4 * sizeof(UINT);
	device->CreateBuffer(&deadParticleNumBufferDesc, nullptr, DeadParticleNumConstantBuffer.GetAddressOf());
	// ���S���o�^
	m_DeviceContext->CSSetConstantBuffers(CBufferData::DEADPARTICLENUMBER_INDEX, 1, DeadParticleNumConstantBuffer.GetAddressOf());

	//---------------------------------------------------------------------------------------------
	//      �p�[�e�B�N���G�~�b�^�[�̃R���X�^���g�o�b�t�@
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC particleEmitterBufferDesc = {};

	particleEmitterBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	particleEmitterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	particleEmitterBufferDesc.CPUAccessFlags = 0;
	particleEmitterBufferDesc.ByteWidth = sizeof(CBufferData::ParticleEmitter);

	hr = device->CreateBuffer(&particleEmitterBufferDesc, nullptr, m_EmitterConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ParticleManager::Init CreateBuffer�����s");
		return;
	}

	// �R���s���[�g�V�F�[�_�[�Ƀp�[�e�B�N���G�~�b�^�[�o�b�t�@��o�^
	m_DeviceContext->CSSetConstantBuffers(CBufferData::USEPARTICLEEMITTER_INDEX, 1, m_EmitterConstantBuffer.GetAddressOf());


	//---------------------------------------------------------------------------------------------
	//      �����e�N�X�`���̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateTexture2D�����s");
		return;
	}

	delete[] values;


	//---------------------------------------------------------------------------------------------
	//      �����e�N�X�`��SRV�̍쐬
	//---------------------------------------------------------------------------------------------

	D3D11_SHADER_RESOURCE_VIEW_DESC randomTextureSRV;
	randomTextureSRV.Format = randomTextureDesc.Format;
	randomTextureSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	randomTextureSRV.Texture2D.MipLevels = 1;
	randomTextureSRV.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(m_RandomTexture.Get(), &randomTextureSRV, m_RandomTextureSRV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceView�����s");
		return;
	}

	// �V�F�[�_�[���\�[�X�r���[���Z�b�g
	m_DeviceContext->CSSetShaderResources(SRVData::k_RandomTextureShaderResourceViewIndex, 1, m_RandomTextureSRV.GetAddressOf());


	//---------------------------------------------------------------------------------------------
	//      �f�o�b�O�p�o�b�t�@�̍쐬
	//---------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC debugBufferDesc = {};
	debugBufferDesc.Usage = D3D11_USAGE_STAGING;
	debugBufferDesc.BindFlags = 0;
	debugBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	debugBufferDesc.ByteWidth = sizeof(UINT);
	hr = device->CreateBuffer(&debugBufferDesc, nullptr, &m_DebugCounterBuffer);
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer�����s");
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
	// �ϐ��̏�����
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

	// �p�[�e�B�N���G�~�b�^�[�̃R���X�^���g�o�b�t�@�Z�b�g
	int spawnNum = SetParticleEmitterConstantBuffer(emitterIndex);

	auto&& iter = m_ParticleMap.find(particleName);

	if (iter == m_ParticleMap.end())
	{
		Logger::GetInstance().SetLog("ParticleManager::SpawnParticle �}�b�v�ɓo�^�Ȃ�");
		return;
	}
	if (iter->second)
	{
		ID3D11UnorderedAccessView* deadParticleIndexUAV = iter->second->GetDeadParticleIndexAppendConsumeUAV();

		// ���S�����擾
		m_DeviceContext->CopyStructureCount(DeadParticleNumConstantBuffer.Get(), 0, deadParticleIndexUAV);

		// �G�~�b�^�[���w��
		if (m_Emitters[emitterIndex].EmitterType < 0 || m_Emitters[emitterIndex].EmitterType > CBufferData::ET_MAX)
		{
			Logger::GetInstance().SetLog("ParticleManager::SpawnParticle �G�~�b�^�[�̃^�C�v���������w�肳��Ă��Ȃ�");
			return;
		}
		// �V�F�[�_�[�Z�b�g
		m_DeviceContext->CSSetShader(m_EmitterComputeShader[m_Emitters[emitterIndex].EmitterType], nullptr, 0);

		iter->second->Emitter(spawnNum);
	}
}

void ParticleManager::Update()
{
	// �G�~�b�^�[�z��̍X�V
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
		// �G�~�b�^�[�Z�b�g
		m_DeviceContext->UpdateSubresource(m_EmitterArrayConstantBuffer.Get(), 0, NULL, m_Emitters, 0, 0);

		m_EmitterArrayIsUpdate = false;
	}
}

void ParticleManager::CreateParticle(const std::string & particleName,int maxParticleNum, ID3D11ShaderResourceView* srv)
{
	// �p�[�e�B�N���쐬
	Particle* particle = new Particle(maxParticleNum);
	// null�`�F�b�N
	if (particle == nullptr)
	{
		Logger::GetInstance().SetLog("ParticleManager::CreateParticle particle��nullptr");
		return;
	}

	// �e�N�X�`���ݒ�
	particle->SetTexture(srv);
	// ����������
	particle->Init();
	// �o�^
	m_ParticleMap.emplace(particleName, particle);

}

void ParticleManager::UpdateParticleEmitterConstantBuffer(int emitterIndex, const CBufferData::ParticleEmitter & particleEmitter)
{
	if (emitterIndex < 0 || emitterIndex >= EMITTER_MAXNUM)
	{

		Logger::GetInstance().SetLog("ParticleManager::GetParticleEmitter �擾�����C���f�b�N�X���ő吔�𒴂��Ă�");
		return;
	}

	// ����
	m_Emitters[emitterIndex] = particleEmitter;
	m_Emitters[emitterIndex].MyIndex = emitterIndex;

	// �X�V�t���O�𗧂Ă�
	m_EmitterArrayIsUpdate = true;
}

int ParticleManager::SetParticleEmitterConstantBuffer(int emitterIndex)
{
	if (emitterIndex < 0 || emitterIndex >= EMITTER_MAXNUM)
	{
		Logger::GetInstance().SetLog("ParticleManager::SetParticleEmitterConstantBuffer �擾�����C���f�b�N�X���ő吔�𒴂��Ă�");
		return 0;
	}

	CBufferData::ParticleEmitter emitter = m_Emitters[emitterIndex];
	// �G�~�b�^�[�Z�b�g
	m_DeviceContext->UpdateSubresource(m_EmitterConstantBuffer.Get(), 0, NULL, &emitter, 0, 0);

	// �X�V�t���O�𗧂Ă�
	m_EmitterArrayIsUpdate = true;

	return emitter.MaxSpawnParticlesThisFrame;
}

int ParticleManager::ReadCounter(ID3D11UnorderedAccessView * uav)
{
	int count = 0;

	// UAV�̃J�E���^�[���f�o�b�O�p�o�b�t�@�ɃR�s�[����
	m_DeviceContext->CopyStructureCount(m_DebugCounterBuffer, 0, uav);

	// �f�o�b�O�p�o�b�t�@�̒��g�����o��
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	m_DeviceContext->Map(m_DebugCounterBuffer, 0, D3D11_MAP_READ, 0, &MappedResource);

	// �f�[�^��ǂݍ���
	count = *(int*)MappedResource.pData;

	m_DeviceContext->Unmap(m_DebugCounterBuffer, 0);

	return count;
}
