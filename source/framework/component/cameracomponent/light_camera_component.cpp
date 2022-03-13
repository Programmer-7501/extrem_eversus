
#include"../../../system/main.h"
#include "light_camera_component.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"

/**
* @namespace LightCameraData
* @brief ���C�g�J�����f�[�^
*/
namespace LightCameraData
{
	//! �t�H�u�f�[�^
	static constexpr float k_Fov = DirectX::XMConvertToRadians(60.0f);
	//! �؂��蕝
	static constexpr float k_Width = 64.0f;
	//! �؂��荂��
	static constexpr float k_Height = 64.0f;
	//! ��A�[Z
	static constexpr float k_NearZ = 0.1f;
	//! �t�@�[Z
	static constexpr float k_FarZ = 100.0f;

	//! �n�ʂ���ǂꂾ�����ꂽ�ʒu�ɂ��邩
	static constexpr float k_GroundHeight = 20.0f;
}

LightCameraComponent::LightCameraComponent(Actor * owner)
	: QuaternionCameraComponent(owner)
	, m_ChaseActor(nullptr)
{
}

LightCameraComponent::~LightCameraComponent()
{
}

void LightCameraComponent::Update()
{
	if (m_ChaseActor == nullptr)
	{
		return;
	}
	m_Position = m_ChaseActor->GetPosition();
	m_Position.y += LightCameraData::k_GroundHeight;
}

void LightCameraComponent::SetCameraData()
{
	// �����_���[�擾
	Renderer& renderer = Renderer::GetInstance();

	Conv_XM::Matrix4x4f view, projection, viewProjection;

	//�r���[�}�g���N�X����
	view = DirectX::XMMatrixLookToLH(m_Position, m_Forward, m_Up);
	//�v���W�F�N�V�����}�g���N�X�ݒ�
	projection = DirectX::XMMatrixOrthographicLH(LightCameraData::k_Width * 2, LightCameraData::k_Height * 2, LightCameraData::k_NearZ, LightCameraData::k_FarZ);
	// �r���[�v���W�F�N�V�����s��
	viewProjection = (DirectX::XMMATRIX)view * (DirectX::XMMATRIX)projection;

	// �R���X�^���g�o�b�t�@�ɃZ�b�g
	renderer.SetViewProjectionMatrix(view, projection);
	renderer.SetLightViewProjection(viewProjection);
}
