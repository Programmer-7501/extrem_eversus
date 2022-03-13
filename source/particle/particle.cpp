
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
	// �p�[�e�B�N���X�V
	UpdateParticle();

	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Draw m_SceneRenderManager��nullptr");
		return;
	}

	Renderer& renderer = Renderer::GetInstance();

	CameraComponent* camera = m_SceneRenderManager->GetMainCameraComponent();

	// null�`�F�b�N
	if (m_DeviceContext == nullptr || m_VertexBuffer.Get() == nullptr || m_IndexBuffer == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InputLayout == nullptr || m_ShaderResourceView == nullptr || m_SceneRenderManager == nullptr || camera == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Draw nullptr");
		return;
	}

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f viewMatrix = camera->GetViewMatrix();

	//�r���[�s��̋t�s��łȂ������s�ړ�������0
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


	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �C���f�b�N�X�o�b�t�@��ݒ�
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	m_DeviceContext->VSSetShaderResources(SRVData::k_ParticleDataShaderResourceViewIndex, 1, m_ParticleSRV.GetAddressOf());
	m_DeviceContext->VSSetShaderResources(SRVData::k_AlliveParticleIndexShaderResourceViewIndex, 1, m_AlliveParticleIndexSRV.GetAddressOf());

	//�e�N�X�`���ݒ�
	m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &m_ShaderResourceView);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���߉��Z�����ɂ���
	renderer.SetBlendState(Renderer::BLENDSTATE_INVISIBLEADD);

	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_NOWRITE);

	// �`��
	m_DeviceContext->DrawIndexedInstancedIndirect(m_DrawArgs.Get(), 0);

	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_STANDARD);

	// �f�t�H���g�ɂ���
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

	// �p�[�e�B�N���f�[�^��o�^
	// ���݂̔�\���J�E���^�[��ێ�
	UINT init[] = { static_cast<UINT>(-1), static_cast<UINT>(-1) };
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDATA_INDEX, ARRAYSIZE(uavs), uavs, init);

	int dispatch = (spawnNum / PARTICLE_EMITTER_THREDNUM) + 1;
	// ���s
	m_DeviceContext->Dispatch(dispatch, 1, 1);

	ZeroMemory(uavs, sizeof(uavs));
	//�o�^����s
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDATA_INDEX, ARRAYSIZE(uavs), uavs, init);
}

bool Particle::Init()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("Particle::Init GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("Particle::Init GetShader�����s");
		return false;
	}

	// �f�o�C�X�̎擾
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// null�`�F�b�N
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init device��nullptr");
		return false;
	}

	HRESULT hr;

	//---------------------------------------------------------------------------------------------
	//      ���_�o�b�t�@�̍쐬
	//---------------------------------------------------------------------------------------------

	// ���_�ݒ�
	VERTEX_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f, 0.5f,0.0f}, {0.0f,0.0f} };
	vertex[1] = { { 0.5f, 0.5f,0.0f}, {1.0f,0.0f} };
	vertex[2] = { {-0.5f, -0.5f ,0.0f}, {0.0f,1.0f} };
	vertex[3] = { { 0.5f, -0.5f ,0.0f}, {1.0f,1.0f} };


	// ���_�o�b�t�@����
	// ������������
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_TEXCOOD) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = vertex;

	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer���s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      �C���f�b�N�X�o�b�t�@�̍쐬
	//---------------------------------------------------------------------------------------------

	// �l�p�`�̃C���f�b�N�X���`
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// �o�b�t�@���쐬
	D3D11_BUFFER_DESC indexBufferDesc = {};
	// �C���f�b�N�X���̒ǉ�
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = index;

	// �C���f�b�N�X�o�b�t�@���쐬
	hr = device->CreateBuffer(&indexBufferDesc, &indexInitData, m_IndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer���s");
		return false;
	}


	//---------------------------------------------------------------------------------------------
	//      �C���X�^���X���̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer�Ɏ��s");
		return false;
	}



	//---------------------------------------------------------------------------------------------
	//      �C���X�^���X���UAV�̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateUnorderedAccessView�Ɏ��s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      �p�[�e�B�N�����̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer�Ɏ��s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      �p�[�e�B�N�����UAV�̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceView�Ɏ��s");
		return false;
	}


	//---------------------------------------------------------------------------------------------
	//      �p�[�e�B�N�����SRV�̍쐬
	//---------------------------------------------------------------------------------------------

	D3D11_SHADER_RESOURCE_VIEW_DESC particleBufferSRVDesc = {};
	particleBufferSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	particleBufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	particleBufferSRVDesc.Buffer.FirstElement = 0;
	particleBufferSRVDesc.Buffer.NumElements = m_MaxParticleNum;

	hr = device->CreateShaderResourceView(m_ParticleBuffer.Get(), &particleBufferSRVDesc, m_ParticleSRV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceView�Ɏ��s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      ���S�p�[�e�B�N���C���f�b�N�X�o�b�t�@�̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer�Ɏ��s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      ���S�p�[�e�B�N��UAV�̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateUnorderedAccessView�Ɏ��s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      �����p�[�e�B�N���̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer�Ɏ��s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      �����p�[�e�B�N��UAV�̍쐬
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
		Logger::GetInstance().SetLog("Particle::Init CreateUnorderedAccessView�Ɏ��s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      �����p�[�e�B�N�����SRV�̍쐬
	//---------------------------------------------------------------------------------------------

	D3D11_SHADER_RESOURCE_VIEW_DESC alliveParticleIndexSRVDesc = {};
	alliveParticleIndexSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	alliveParticleIndexSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	alliveParticleIndexSRVDesc.Buffer.FirstElement = 0;
	alliveParticleIndexSRVDesc.Buffer.NumElements = m_MaxParticleNum;

	hr = device->CreateShaderResourceView(m_AlliveParticleIndexBuffer.Get(), &alliveParticleIndexSRVDesc, m_AlliveParticleIndexSRV.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Particle::Init CreateShaderResourceView�Ɏ��s");
		return false;
	}


	//�@�V�[�������_�[�}�l�[�W���[�擾
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init currentScene��nullptr");
		return false;
	}

	m_SceneRenderManager = currentScene->GetSceneRenderManager();
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init m_SceneRenderManager��nullptr");
		return false;
	}

	// �p�[�e�B�N���f�[�^�̏�����
	if (InitParticleData() == false)
	{
		Logger::GetInstance().SetLog("Particle::Init InitParticleData�����s");
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

	// �V�F�[�_�[�Z�b�g
	m_DeviceContext->CSSetShader(m_InitComputeShader, nullptr, 0);

	// ���S�C���f�b�N�X�Z�b�g
	UINT init[] = { static_cast<UINT>(0) };
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::DEADPARTICLE_INDEX, 1, m_DeadParticleIndexAppendConsumeUAV.GetAddressOf(), init);

	// ���s
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

	// �V�F�[�_�[�Z�b�g
	m_DeviceContext->CSSetShader(m_UpdateComputeShader, nullptr, 0);

	// ���݂̔�\���J�E���^�[��ێ�
	ID3D11UnorderedAccessView* uavs[] = { m_DrawArgsUAV.Get(), m_ParticleUAV.Get(), m_DeadParticleIndexAppendConsumeUAV.Get(), m_AlliveParticleIndexUAV.Get() };
	UINT init[] = { static_cast<UINT>(-1), static_cast<UINT>(-1),static_cast<UINT>(-1),static_cast<UINT>(0) };
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDRAWARGS_INDEX, ARRAYSIZE(uavs), uavs, init);

	// ���s
	m_DeviceContext->Dispatch(m_Dispatch, 1, 1);

	// ���݂̔�\���J�E���^�[��ێ�
	ZeroMemory(uavs, sizeof(uavs));
	m_DeviceContext->CSSetUnorderedAccessViews(UAVData::PARTICLEDRAWARGS_INDEX, ARRAYSIZE(uavs), uavs, init);
}

bool Particle::GetDeviceContext()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_DeviceContext��nullptr");
		return false;
	}

	return true;
}

bool Particle::GetShader()
{
	// �V�F�[�_�[�}�l�[�W���[�擾
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_PARTICLE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_PARTICLE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_PARTICLE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputLayout��nullptr");
		return false;
	}

	// �������R���s���[�g�V�F�[�_�[�̎擾
	m_InitComputeShader = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_INIT);
	if (m_InitComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_ComputeShader��nullptr");
		return false;
	}

	// �X�V�R���s���[�g�V�F�[�_�[�̎擾
	m_UpdateComputeShader = shaderManager.GetComputeShader(ShaderManager::CS_NAME_PARTICLE_UPDATE);
	if (m_UpdateComputeShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_ComputeShader��nullptr");
		return false;
	}

	return true;
}