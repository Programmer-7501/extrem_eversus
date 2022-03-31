#include"../../../../system/main.h"
#include "mobilesuit_state_turn_around_beamrifle_shot.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../cameracomponent/lockon_camera_component.h"

namespace MobileSuitStateTurnAroundBeamRifleShotData
{
	//! �u�[�X�g�_�b�V�����͎�t����
	constexpr int k_BoostDashInputTime = 12;

	//! �u�[�X�g�_�b�V������
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! �u�[�X�g�_�b�V�����Ɏg�p������
	constexpr float k_BoostDashUseBoostNum = 20.0f;

	//! �󒆔w�ʂ����̏ꍇ�̈�t���[���ɂ�����u�[�X�g�g�p��
	constexpr float k_TurnAroundAirUseBoostNum = 2.0f;
}

MobileSuitStateTurnAroundBeamRifleShot::MobileSuitStateTurnAroundBeamRifleShot(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_BoostDashInitialSpeed(MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInitialSpeed)
	, m_BoostDashCount(MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInputTime + 1)
	, m_FrameCount(0)
{
}

void MobileSuitStateTurnAroundBeamRifleShot::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetData�����s");
		return;
	}

	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::Init m_MobileSuit��nullptr");
		return;
	}
}

void MobileSuitStateTurnAroundBeamRifleShot::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateTurnAroundBeamRifleShot::ProcessInput nullptr");
		return;
	}


	bool isBeamRifleShot = m_MobileSuit->GetIsBeamRifleShot();
	// �r�[�����C�t���������I������
	if (isBeamRifleShot == false)
	{
		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}
	}


	//! AI�̏ꍇ
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		// �u�[�X�g�_�b�V�����͑ҋ@��ԂōĂуu�[�X�g�_�b�V�����͂����ꂽ��
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount <= MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInputTime)
		{
			// �u�[�X�g�_�b�V���ɃL�����Z��
			// �J�����̑O�x�N�g�����擾
			Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
			forward.y = 0.0f;
			forward = DirectX::XMVector3Normalize(forward);

			// y���ŉE��90�x��]�������N�H�[�^�j�I�����쐬
			Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
			Conv_XM::Vector3f right = DirectX::XMVector3Rotate(forward, q1);

			// ���r���X�[�c�A�N�^�[�̑O�x�N�g�����擾
			Conv_XM::Vector3f myForward = m_MobileSuit->GetForward();
			myForward.y = 0.0f;
			myForward = DirectX::XMVector3Normalize(myForward);

			RotationActor(forward, myForward, m_Owner->GetInputDirection());
			Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
			m_RigidBodyComponent->PlusVelocity(dir * m_BoostDashInitialSpeed);

			m_MobileSuit->SetIsBoostDash(true);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
			m_BoostComponent->UseBoost(MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashUseBoostNum);
		}

		// �u�[�X�g�_�b�V�����͂��Ȃ���ԂŃW�����v�{�^���������ꂽ��
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount > MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInputTime)
		{
			// �J�E���g���[���ɂ���
			m_BoostDashCount = 0;
		}
	}

	// �󒆂ɂ���ꍇ�u�[�X�g���g�p����
	if (m_RigidBodyComponent->GetIsGround() == false)
	{
		m_BoostComponent->UseBoost(MobileSuitStateTurnAroundBeamRifleShotData::k_TurnAroundAirUseBoostNum);
	}
}

void MobileSuitStateTurnAroundBeamRifleShot::Update()
{
	// �J�E���g�A�b�v
	m_FrameCount++;
	m_BoostDashCount++;
}

void MobileSuitStateTurnAroundBeamRifleShot::OnEnter()
{
	if (m_RigidBodyComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateTurnAroundBeamRifleShot::OnEnter m_RigidBodyComponent��nullptr");
		return;
	}

	// ���x��0�ɂ���
	m_RigidBodyComponent->SetDirection(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	m_RigidBodyComponent->SetIsUseGravity(false);

	// �G�̕����Ɍ�����
	RotateToEnemy();

	// �������Ȃ�
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("beamrifle", 20);

	m_BoostDashCount = MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInputTime + 1;
	m_FrameCount = 0;
}

void MobileSuitStateTurnAroundBeamRifleShot::OnExit()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateTurnAroundBeamRifleShot::OnEnter m_RigidBodyComponent��nullptr");
		return;
	}
	m_RigidBodyComponent->SetIsUseGravity(true);
}
