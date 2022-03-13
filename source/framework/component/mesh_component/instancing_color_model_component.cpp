
#include"../../../system/main.h"
#include "instancing_color_model_component.h"
#include"../../../directx/resource/model.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

namespace InstancingColorModelComponentData
{
	static constexpr int k_DefaultInstancingNum = 1024;
}

InstancingColorModelComponent::InstancingColorModelComponent(Actor * owner)
	:MeshComponent(owner)
	, m_Model(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_MaxInstanceNum(InstancingColorModelComponentData::k_DefaultInstancingNum)
	, m_InstanceMatrixBuffer(nullptr)
{
}

void InstancingColorModelComponent::Update()
{
	if (m_IsUpdate == true)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		Renderer& renderer = Renderer::GetInstance();
		renderer.GetDeviceContext()->Map(m_InstanceMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

		InstanceColorData* instanceData = (InstanceColorData*)(map.pData);

		for (int i = 0; i < m_InstanceColorData.size(); i++)
		{
			instanceData[i] = m_InstanceColorData[i];
		}

		renderer.GetDeviceContext()->Unmap(m_InstanceMatrixBuffer.Get(), 0);

		m_IsUpdate = false;
	}
}

void InstancingColorModelComponent::Draw() const
{
	// null�`�F�b�N
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InstanceMatrixBuffer == nullptr || m_MaxInstanceNum <= 0)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false)
	{
		return;
	}

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	Renderer& renderer = Renderer::GetInstance();
	renderer.SetMaterial(m_Material);

	// ���f���`��
	UINT stride[2] = { sizeof(VERTEX_NORMAL_TEXCOOD), sizeof(InstanceColorData) };
	UINT offset[2] = { 0, 0 };
	m_Model->InstancingColorDraw(m_InstanceMatrixBuffer.Get(), static_cast<int>(m_InstanceColorData.size()), stride, offset);
}

bool InstancingColorModelComponent::CreateInstancingData()
{
	Renderer& renderer = Renderer::GetInstance();
	ID3D11Device* device = renderer.GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::CreateInstancingData device��nullptr");
		return false;
	}

	HRESULT hr;

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;//���_���������p
	bd.ByteWidth = sizeof(InstanceColorData) * m_MaxInstanceNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//���_���������p
	bd.MiscFlags = 0;

	hr = device->CreateBuffer(&bd, nullptr, m_InstanceMatrixBuffer.GetAddressOf());

	if (FAILED(hr) == true)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::CreateInstancingData CreateBuffer�����s");
		return false;
	}

	return true;
}

bool InstancingColorModelComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Init GetDeviceInterface�����s");
		return false;
	}

	if (CreateInstancingData() == false)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Init CreateInstancingData�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Init GetShader�����s");
		return false;
	}

	return true;
}

bool InstancingColorModelComponent::GetDeviceContext()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::GetInterface m_DeviceContext��nullptr");
		return false;
	}

	return true;
}

bool InstancingColorModelComponent::GetShader()
{
	// �V�F�[�_�[�}�l�[�W���[�̎擾
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_INSTANCING_COLOR);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::GetShader m_VertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_INSTANCING_COLOR);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::GetInterface m_PixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_INSTANCING_COLOR);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}
