
#include"../../../../system/main.h"
#include "mobilesuit_state_boost_dash.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mesh_component/animation_model_component.h"

namespace StateBoostDashData
{
	//! �f�t�H���g�̃u�[�X�g�_�b�V���X�s�[�h
	constexpr float k_DefaultBoostDashSpeed = 0.06f;

	//! �f�t�H���g�̃u�[�X�g�_�b�V������
	constexpr int k_DefaultBoostDashTime = 24;

	//! �f�t�H���g�̉�]���(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;

	//! �f�t�H���g�̃u�[�X�g�g�p��
	constexpr float k_DefaultBoostDashUseBoostNum = 2.0f;
}

MobileSuitStateBoostDash::MobileSuitStateBoostDash(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_BoostDashSpeed(StateBoostDashData::k_DefaultBoostDashSpeed)
	, m_BoostTime(StateBoostDashData::k_DefaultBoostDashTime)
	, m_FrameCount(0)
	, m_IsPressJump(true)
	, m_IsPressMove(true)
{

}

void MobileSuitStateBoostDash::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetData�����s");
		return;
	}
}

void MobileSuitStateBoostDash::ProcessInput()
{
	// null�`�F�b�N
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_MobileSuit == nullptr || m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}

	// �u�[�X�g���Ȃ��Ȃ�
	if (m_BoostComponent->GetIsOverHeat() == true)
	{
		m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
	}

	// �J�����̑O�x�N�g�����擾
	Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
	forward.y = 0.0f;
	forward = DirectX::XMVector3Normalize(forward);

	// �J�����̉E�x�N�g�����擾
	Conv_XM::Vector3f right = m_LockOnCameraComponent->GetRight();
	right.y = 0.0f;
	right = DirectX::XMVector3Normalize(right);

	// ���r���X�[�c�A�N�^�[�̑O�x�N�g�����擾
	Conv_XM::Vector3f myForward = m_MobileSuit->GetForward();
	myForward.y = 0.0f;
	myForward = DirectX::XMVector3Normalize(myForward);

	//! AI�̏ꍇ
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_JUMP) == false)
		{
			// �W�����v�{�^�������������Ȃ�����
			m_IsPressJump = false;
		}

		if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK) == false)
		{
			// �ړ��{�^�������������Ȃ�����
			m_IsPressMove = false;
		}

		bool isContinueBoostDash = m_IsPressJump || m_IsPressMove;

		if (isContinueBoostDash == true)// �u�[�X�g�_�b�V���𑱂���
		{
			// �����Ă���Ԃ̓J�E���g���Z�b�g
			m_FrameCount = 0;
			// ���W�b�h�{�f�B�ɑ��x��^����
			m_RigidBodyComponent->PlusVelocity(myForward * StateBoostDashData::k_DefaultBoostDashSpeed);
			m_RigidBodyComponent->SetVelocityY(0.0f);

			// �u�[�X�g���g�p����
			m_BoostComponent->UseBoost(StateBoostDashData::k_DefaultBoostDashUseBoostNum);

			// �u�[�X�g�W�����v��ԂɈڍs
			if (m_IsPressJump == false && m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_JUMP) == true)
			{
				m_Owner->ChangeMobileSuitState("MobileSuitStateBoostJump");
			}
		}
		else// �u�[�X�g�_�b�V������߂�
		{
			// �u�[�X�g�W�����v��ԂɈڍs
			if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP))
			{
				m_Owner->ChangeMobileSuitState("MobileSuitStateBoostJump");
			}
			// �u�[�X�g�_�b�V�����Ԃ𒴂����ꍇ
			if (m_FrameCount > StateBoostDashData::k_DefaultBoostDashTime)
			{
				m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
			}
			else //�@�u�[�X�g�_�b�V�����Ԓ��͑��x��������
			{
				// ���W�b�h�{�f�B�ɑ��x��^����
				m_RigidBodyComponent->PlusVelocity(myForward * StateBoostDashData::k_DefaultBoostDashSpeed);
				m_RigidBodyComponent->SetVelocityY(0.0f);

				// �u�[�X�g���g�p����
				m_BoostComponent->UseBoost(StateBoostDashData::k_DefaultBoostDashUseBoostNum);
			}
		}

		RotationActor(forward, myForward, m_Owner->GetInputDirection(), StateBoostDashData::k_DefaultRotationLimit);

		// ����ˌ�����
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) &&
			m_CannonBulletManager->GetCurrentBullet() > 0)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))// ���C�����͂���������
		{
			m_MobileSuit->BeamRifleShot();
		}
	}
}

void MobileSuitStateBoostDash::Update()
{
	// �t���[���J�E���g�A�b�v
	m_FrameCount++;
}

void MobileSuitStateBoostDash::OnEnter()
{
	if (m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter m_RigidBodyComponent��nullptr");
		return;
	}

	// �u�[�X�g�_�b�V���t���O��true��
	m_MobileSuit->SetIsBoostDash(true);
	m_MobileSuit->ResetBeamRifleCoolTime();

	// �d�͂��I�t�ɂ���
	m_RigidBodyComponent->SetIsUseGravity(false);

	// �ڒn��Ԃ���������
	m_RigidBodyComponent->SetIsGround(false);

	// �t���[���J�E���g��������
	m_FrameCount = 0;

	// �{�^���t���O��������
	m_IsPressJump = true;
	m_IsPressMove = true;

	// �������Ȃ�
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("boostdash", 20);
}

void MobileSuitStateBoostDash::OnExit()
{
	if (m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnExit m_RigidBodyComponent��nullptr");
		return;
	}

	// �u�[�X�g�_�b�V���t���O��false��
	m_MobileSuit->SetIsBoostDash(false);

	// �d�͂��I���ɂ���
	m_RigidBodyComponent->SetIsUseGravity(true);
}

