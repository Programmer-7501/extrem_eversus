
#include"../../../../system/main.h"
#include "mobilesuit_state_boost_jump.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"

namespace StateBoostJumpData
{
	//! �f�t�H���g�̃W�����v���x
	constexpr float k_BoostJumpSpeed = 0.08f;

	//! �u�[�X�g�����
	constexpr float k_DefaultBoostJumpUseBoostNum = 3.5f;

	//! �u�[�X�g�_�b�V�����͎�t����
	constexpr int k_BoostDashInputTime = 16;

	//! �u�[�X�g�_�b�V�����x
	constexpr float k_BoostDashSpeed = 1.5f;

	//! �u�[�X�g�_�b�V�����̃u�[�X�g�g�p��
	constexpr float k_DefaultBoostDashUseBoostNum = 20.0f;

	//! �f�t�H���g�̉�]���(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;

	//! �f�t�H���g�̏㏸���x����l
	constexpr float k_DefaultRiseSpeedLimit = 0.4f;
}

MobileSuitStateBoostJump::MobileSuitStateBoostJump(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_BoostJumpSpeed(StateBoostJumpData::k_BoostJumpSpeed)
	, m_CountFrame(0)
	, m_BoostDashSpeed(StateBoostJumpData::k_BoostDashSpeed)
	, m_BoostDashUseBoostNum(StateBoostJumpData::k_DefaultBoostDashUseBoostNum)
	, m_IsPressJumpButton(true)
{
}

void MobileSuitStateBoostJump::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetData�����s");
		return;
	}
}

void MobileSuitStateBoostJump::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_BoostComponent == nullptr || m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBoostJump::ProcessInput nullptr");
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

	// y���ŉE��90�x��]�������N�H�[�^�j�I�����쐬
	Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
	Conv_XM::Vector3f right = DirectX::XMVector3Rotate(forward, q1);

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
		// �W�����v�{�^�������͂��ꑱ������㏸
		if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_JUMP))
		{
			// ���x��^����
			m_RigidBodyComponent->PlusVelocity(Conv_XM::Vector3f(0.0f, m_BoostJumpSpeed, 0.0f));
			if (m_RigidBodyComponent->GetVelocity().y > StateBoostJumpData::k_DefaultRiseSpeedLimit)
			{
				m_RigidBodyComponent->SetVelocityY(StateBoostJumpData::k_DefaultRiseSpeedLimit);
			}

			m_BoostComponent->UseBoost(StateBoostJumpData::k_DefaultBoostJumpUseBoostNum);

			// ��������]������
			RotationActor(forward, myForward, m_Owner->GetInputDirection(), StateBoostJumpData::k_DefaultRotationLimit);
		}
		else
		{
			m_IsPressJumpButton = false;
		}
		if (m_CountFrame > StateBoostJumpData::k_BoostDashInputTime && m_IsPressJumpButton == false)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}

		// ���Ԉȓ��ɃW�����v�{�^���������ꂽ��
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_CountFrame < StateBoostJumpData::k_BoostDashInputTime)
		{
			RotationActor(forward, myForward, m_Owner->GetInputDirection());
			Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
			m_RigidBodyComponent->PlusVelocity(dir * m_BoostDashSpeed);

			m_MobileSuit->SetIsBoostDash(true);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
			m_BoostComponent->UseBoost(m_BoostDashUseBoostNum);
		}

		// ���C�����͂���������
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))
		{
			m_MobileSuit->BeamRifleShot();
		}
	}
}

void MobileSuitStateBoostJump::Update()
{
	// �J�E���g�A�b�v
	m_CountFrame++;
}

void MobileSuitStateBoostJump::OnEnter()
{
	// �J�E���g������
	m_CountFrame = 0;

	if (m_RigidBodyComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBoostJump::OnEnter nullptr");
		return;
	}
	// �ݒu���������
	m_RigidBodyComponent->PlusVelocity(Conv_XM::Vector3f(0.0f, 0.1f, 0.0f));
	m_RigidBodyComponent->SetIsGround(false);

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("idle", 5);

	// �{�^���t���O���Z�b�g
	m_IsPressJumpButton = true;
}

void MobileSuitStateBoostJump::OnExit()
{

}
