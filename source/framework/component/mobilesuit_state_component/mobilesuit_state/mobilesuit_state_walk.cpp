
#include"../../../../system/main.h"
#include "mobilesuit_state_walk.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mesh_component/animation_model_component.h"


namespace StateWalkData
{
	//! �f�t�H���g�̕�������
	constexpr float k_DefaultWalkSpeed = 0.02f;

	//! �f�t�H���g�W�����v���x
	constexpr float k_DefaultJumpSpeed = 2.0f;

	//! �f�t�H���g�̉�]���(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;

	//! �X�e�b�v���͑ҋ@����
	constexpr int k_StepInputTime = 16;
}

MobileSuitStateWalk::MobileSuitStateWalk(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_WalkSpeed(StateWalkData::k_DefaultWalkSpeed)
	, m_JumpSpeed(StateWalkData::k_DefaultJumpSpeed)
	, m_FrameCount(0)
{
}

void MobileSuitStateWalk::Init()
{
	// �f�[�^�擾�擾
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetData�����s");
		return;
	}
}

void MobileSuitStateWalk::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_LockOnCameraComponent == nullptr || m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}

	//! AI�̏ꍇ
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		// �J�����̑O�x�N�g�����擾
		Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
		forward.y = 0.0f;
		forward = DirectX::XMVector3Normalize(forward);


		// y���ŉE��90�x��]�������N�H�[�^�j�I�����쐬
		Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
		Conv_XM::Vector3f right = DirectX::XMVector3Rotate(forward, q1);

		// ���r���X�[�c�A�N�^�[�̑O�x�N�g�����擾
		Conv_XM::Vector3f myForward = m_MobileSuit->GetForward();


		// ����ˌ�����
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) &&
			m_CannonBulletManager->GetCurrentBullet() > 0)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP))// �u�[�X�g�W�����v��ԂɈڍs
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateJumpStandby");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))// �ˌ�����
		{
			m_MobileSuit->BeamRifleShot();
		}
		else if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK))
		{
			RotationActor(forward, myForward, m_Owner->GetInputDirection(), StateWalkData::k_DefaultRotationLimit);
			// �O�x�N�g���ƖڕW�x�N�g���̂Ȃ��p���Z�o
			float cos = Conv_XM::Vector2f::Dot(Conv_XM::Vector2f::Forward, m_Owner->GetInputDirection());
			float rad;
			if (m_Owner->GetInputDirection().x > 0)
			{
				rad = acosf(cos);
			}
			else
			{
				rad = -acosf(cos);
			}

			q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, rad);

			//���K������
			Conv_XM::Vector3f nextVec = DirectX::XMVector3Rotate(forward, q1);
			nextVec = DirectX::XMVector3Normalize(nextVec);

			Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
			m_RigidBodyComponent->PlusVelocity(nextVec * m_WalkSpeed);
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
	}
}

void MobileSuitStateWalk::Update()
{
	// �J�E���g�A�b�v
	m_FrameCount++;
}

void MobileSuitStateWalk::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter m_AnimationComponent��nullptr");
		return;
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("walk", 20);
	// �t���[���J�E���g��������
	m_FrameCount = 0;
}

void MobileSuitStateWalk::OnExit()
{
}