
#include"../../../system/main.h"
#include "camera_component.h"
#include"../../../directx/renderer.h"

CameraComponent::CameraComponent(Actor * owner)
	: Component(owner)
	, m_CameraData{}
	, m_Position(CameraData::k_DefaultPosition)
	, m_Up(CameraData::k_DefaultUp)
	, m_Target(CameraData::k_DefaultTarget)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::SetCameraData()
{
	// �����_���[�擾
	Renderer& renderer = Renderer::GetInstance();

	// �J�����|�W�V�����Z�b�g
	m_CameraData.CameraPosition = m_Position;
	//�r���[�}�g���N�X����
	m_CameraData.View = DirectX::XMMatrixLookAtLH(m_Position, m_Target, m_Up);
	//�v���W�F�N�V�����}�g���N�X�ݒ�
	m_CameraData.Projection = DirectX::XMMatrixPerspectiveFovLH(CameraData::k_Fov, SystemData::k_AspectRate, CameraData::k_NearZ, CameraData::k_FarZ);
	// �r���[�v���W�F�N�V�����s��
	m_CameraData.CameraViewProjectionInverse = DirectX::XMMatrixInverse(nullptr, (DirectX::XMMATRIX)m_CameraData.View * (DirectX::XMMATRIX)m_CameraData.Projection);
	// �R���X�^���g�o�b�t�@�ɃZ�b�g
	renderer.SetDrawData(m_CameraData);
}
