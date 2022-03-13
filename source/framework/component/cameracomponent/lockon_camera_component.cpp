
#include"../../../system/main.h"
#include "lockon_camera_component.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"

namespace LockOnCameraData
{
	//! ���L�A�N�^�[����ǂꂾ������邩
	constexpr float k_OwnerDistance = 5.0f;

	//! �I�t�Z�b�g�|�W�V����
	const Conv_XM::Vector3f k_OffsetPosition = Conv_XM::Vector3f(0.0f, 2.0f, 0.0f);
}

LockOnCameraComponent::LockOnCameraComponent(Actor * owner)
	: CameraComponent(owner)
	, m_TargetActor(nullptr)
	, m_Quaternion(DirectX::XMQuaternionIdentity())
	, m_Forward(Conv_XM::Vector3f::WorldForward)
	, m_Right(Conv_XM::Vector3f::WorldRight)
	, m_IsNotTargetMoveFlag(false)
	, m_StoreTargetPos(0.0f,0.0f,0.0f)
{
}

LockOnCameraComponent::~LockOnCameraComponent()
{
}

void LockOnCameraComponent::Update()
{
	if (m_TargetActor)
	{
		Conv_XM::Vector3f distance;

		// �^�[�Q�b�g�𓮂������ǂ���
		if (m_IsNotTargetMoveFlag == true)
		{
			distance = m_StoreTargetPos - m_Owner->GetPosition();
		}
		else
		{
			distance = m_TargetActor->GetPosition() - m_Owner->GetPosition();
		}
		
		Conv_XM::Vector3f distancenormal = DirectX::XMVector3Normalize(distance);

		// ��]�������߂�
		Conv_XM::Vector3f Axis;
		Conv_XM::Vector3f::Cross( Conv_XM::Vector3f::WorldForward, distancenormal, &Axis);

		// �Ȃ��p�����߂�(�ǂ�������K���ς݂̂��ߒ�����1)
		float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, distancenormal);
		float rad = acosf(dot);

		// Z������Ƃ����N�H�[�^�j�I�������߂�
		m_Quaternion = DirectX::XMQuaternionRotationAxis(Axis, rad);

		// �O�x�N�g�����擾����
		m_Forward = distancenormal;


		// �E�x�N�g�����擾����
		m_Right = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldRight, m_Quaternion);

		// ��x�N�g���擾
		m_Up = Conv_XM::Vector3f::WorldUp;

		// �J�����̃|�W�V���������
		m_Position = Conv_XM::Vector3f
		(
			m_Owner->GetPosition().x - m_Forward.x * LockOnCameraData::k_OwnerDistance + LockOnCameraData::k_OffsetPosition.x,
			m_Owner->GetPosition().y + LockOnCameraData::k_OffsetPosition.y,
			m_Owner->GetPosition().z - m_Forward.z * LockOnCameraData::k_OwnerDistance + LockOnCameraData::k_OffsetPosition.z
			);

		// �^�[�Q�b�g�̃|�W�V���������
		m_Target = m_Owner->GetPosition() + distancenormal * LockOnCameraData::k_OwnerDistance;
	}
}

void LockOnCameraComponent::SetNotMoveFlag(bool flag)
{
	m_IsNotTargetMoveFlag = flag;

	if (m_IsNotTargetMoveFlag == true)
	{
		m_StoreTargetPos = m_TargetActor->GetPosition();
	}
}
