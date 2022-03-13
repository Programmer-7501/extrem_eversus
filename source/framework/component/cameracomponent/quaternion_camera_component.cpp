
#include"../../../system/main.h"
#include "quaternion_camera_component.h"
#include"../../../directx/renderer.h"

QuaternionCameraComponent::QuaternionCameraComponent(Actor * owner)
	: CameraComponent(owner)
	, m_Quaternion(DirectX::XMQuaternionIdentity())
	, m_Forward(Conv_XM::Vector3f::WorldForward)
{
}

QuaternionCameraComponent::~QuaternionCameraComponent()
{
}

void QuaternionCameraComponent::SetEuler(float x, float y, float z)
{
	float rad_X, rad_Y, rad_Z;
	rad_X = DirectX::XMConvertToRadians(x);
	rad_Y = DirectX::XMConvertToRadians(y);
	rad_Z = DirectX::XMConvertToRadians(z);
	m_Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rad_X, rad_Y, rad_Z);

	m_Forward = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldForward, m_Quaternion);
	m_Forward = DirectX::XMVector3Normalize(m_Forward);
	m_Up = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldUp, m_Quaternion);
	m_Up = DirectX::XMVector3Normalize(m_Up);
}

void QuaternionCameraComponent::SetCameraData()
{
	// �����_���[�擾
	Renderer& renderer = Renderer::GetInstance();

	// ������0�Ȃ�
	if (Conv_XM::Vector3f::LengthSq(m_Forward) < Math::k_Epsilon)
	{
		return;
	}

	// �J�����|�W�V�����Z�b�g
	m_CameraData.CameraPosition = m_Position;
	//�r���[�}�g���N�X����
	m_CameraData.View = DirectX::XMMatrixLookToLH(m_Position, m_Forward, m_Up);
	//�v���W�F�N�V�����}�g���N�X�ݒ�
	m_CameraData.Projection = DirectX::XMMatrixPerspectiveFovLH(CameraData::k_Fov, SystemData::k_AspectRate, CameraData::k_NearZ, CameraData::k_FarZ);
	// �r���[�v���W�F�N�V�����s��
	m_CameraData.CameraViewProjectionInverse = DirectX::XMMatrixInverse(nullptr, (DirectX::XMMATRIX)m_CameraData.View * (DirectX::XMMATRIX)m_CameraData.Projection);
	// �R���X�^���g�o�b�t�@�ɃZ�b�g
	renderer.SetDrawData(m_CameraData);
}
