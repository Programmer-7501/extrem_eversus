
#include"../../../../system/main.h"
#include "mobilesuit_state_jump_standby.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"

namespace MobileSuitStateJumpStandbyData
{
	//! �u�[�X�g�_�b�V�����͎�t����
	static constexpr int k_BoostDashInputTime = 20;

	//! �W�����v�̑��x
	static constexpr float k_DefaultJumpSpeed = 0.1f;

	//! �u�[�X�g�W�����v�Ŏg�p�����u�[�X�g��
	static constexpr float k_DefaultBoostJumpUseBoostNum = 3.5f;

	//! �u�[�X�g�_�b�V�����x
	static constexpr float k_DefaultBoostDashSpeed = 2.5f;

	//! �u�[�X�g�_�b�V���Ŏg�p�����u�[�X�g��
	static constexpr float k_DefaultBoostDashUseBoostNum = 20.0f;
}

MobileSuitStateJumpStandby::MobileSuitStateJumpStandby(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
	, m_IsPressJumpButton(true)
	, m_BoostJumpSpeed(MobileSuitStateJumpStandbyData::k_DefaultJumpSpeed)
	, m_BoostJumpUseBoostNum(MobileSuitStateJumpStandbyData::k_DefaultBoostJumpUseBoostNum)
	, m_BoostDashSpeed(MobileSuitStateJumpStandbyData::k_DefaultBoostDashSpeed)
	, m_BoostDashUseBoostNum(MobileSuitStateJumpStandbyData::k_DefaultBoostDashUseBoostNum)
{
}

void MobileSuitStateJumpStandby::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetData�����s");
		return;
	}
}

void MobileSuitStateJumpStandby::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_BoostComponent == nullptr || m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateJumpStandby::ProcessInput nullptr");
		return;
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

		}
		else
		{
			m_IsPressJumpButton = false;
		}

		if (m_CountFrame > MobileSuitStateJumpStandbyData::k_BoostDashInputTime && m_IsPressJumpButton == false)
		{
			Conv_XM::Vector3f jumpSpeed = m_BoostJumpSpeed * Conv_XM::Vector3f::WorldUp;
			m_RigidBodyComponent->PlusVelocity(jumpSpeed);
			m_BoostComponent->UseBoost(m_BoostJumpUseBoostNum);
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
			m_RigidBodyComponent->PlusVelocity(Conv_XM::Vector3f(0.0f, 0.1f, 0.0f));
			m_RigidBodyComponent->SetIsGround(false);
		}

		// ���Ԉȓ��ɃW�����v�{�^���������ꂽ��
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_CountFrame <= MobileSuitStateJumpStandbyData::k_BoostDashInputTime)
		{
			RotationActor(forward, myForward, m_Owner->GetInputDirection());
			Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
			m_RigidBodyComponent->PlusVelocity(dir * m_BoostDashSpeed);

			m_MobileSuit->SetIsBoostDash(true);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
			m_BoostComponent->UseBoost(m_BoostDashUseBoostNum);
		}

		// �����ƃW�����v�{�^���������Ă�����
		if (m_IsPressJumpButton == true && m_CountFrame > MobileSuitStateJumpStandbyData::k_BoostDashInputTime)
		{
			Conv_XM::Vector3f jumpSpeed = m_BoostJumpSpeed * Conv_XM::Vector3f::WorldUp;
			m_RigidBodyComponent->PlusVelocity(jumpSpeed);
			m_BoostComponent->UseBoost(m_BoostJumpUseBoostNum);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostJump");
		}
	}
}

void MobileSuitStateJumpStandby::Update()
{

	m_CountFrame++;
}

void MobileSuitStateJumpStandby::OnEnter()
{

	if (m_RigidBodyComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBoostJump::OnEnter nullptr");
		return;
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("landing", 5);
	m_CountFrame = 0;
	m_IsPressJumpButton = true;
}

void MobileSuitStateJumpStandby::OnExit()
{
}
