
#include"../../../system/main.h"
#include "model_component.h"
#include"../../../directx/resource/model.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

ModelComponent::ModelComponent(Actor * owner)
	: MeshComponent(owner)
	, m_Model(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_OffsetScale(1.0f,1.0f,1.0f)
	, m_OffsetQuaternion(DirectX::XMQuaternionIdentity())
{
	// �e�`�������
	m_IsShadowInput = true;
}

void ModelComponent::Draw() const
{
	// null�`�F�b�N
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false || m_Visible == false)
	{
		return;
	}

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_OffsetQuaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	Renderer& renderer = Renderer::GetInstance();
	renderer.SetWorldMatrix(mtxWorld);
	renderer.SetMaterial(m_Material);

	// ���f���`��
	m_Model->Draw();
}

void ModelComponent::ShadowDraw() const
{
	// null�`�F�b�N
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::Draw nullptr");
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
	m_DeviceContext->PSSetShader(NULL, NULL, 0);

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_OffsetQuaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	Renderer::GetInstance().SetWorldMatrix(mtxWorld);

	// ���f���`��
	m_Model->Draw();
}

bool ModelComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetShader�����s");
		return false;
	}

	return true;
}

bool ModelComponent::GetDeviceContext()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_DeviceContext��nullptr");
		return false;
	}

	return true;
}

bool ModelComponent::GetShader()
{
	// �V�F�[�_�[�}�l�[�W���[�̎擾
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_GBUFFERINPUT);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetShader m_VertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_GBUFFERINPUT);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_PixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_GBUFFERINPUT);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}
