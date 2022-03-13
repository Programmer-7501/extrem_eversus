
#include"../../../../system/main.h"
#include "mobilesuit_state_air_idle.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../../actor_manager/bullet_manager.h"
namespace MobileSuitStateAirIdleData
{
	//! �f�t�H���g�̃W�����v���x
	static constexpr float k_DefaultJumpSpeed = 0.1f;
}

MobileSuitStateAirIdle::MobileSuitStateAirIdle(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_JumpSpeed(MobileSuitStateAirIdleData::k_DefaultJumpSpeed)
{
}

void MobileSuitStateAirIdle::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateAirIdle::Init GetData�����s");
		return;
	}
}

void MobileSuitStateAirIdle::ProcessInput()
{
	if (m_Owner == nullptr || m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr)
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
		// ����ˌ�����
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) &&
			m_CannonBulletManager->GetCurrentBullet() > 0)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP))// �W�����v���͑ҋ@��ԂɈڍs
		{
			m_RigidBodyComponent->PlusVelocity(Conv_XM::Vector3f(0.0f, m_JumpSpeed, 0.0f));
			m_RigidBodyComponent->SetIsGround(false);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostJump");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))// �ˌ�����
		{
			m_MobileSuit->BeamRifleShot();
		}
	}

	// �ڒn���Ă�����
	if (m_RigidBodyComponent->GetIsGround() == true)
	{
		m_Owner->ChangeMobileSuitState("MobileSuitStateLandingRecovery");
	}
}

void MobileSuitStateAirIdle::Update()
{
}

void MobileSuitStateAirIdle::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateAirIdle::OnEnter m_AnimationComponent��nullptr");
		return;
	}
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("idle", 20);
}

void MobileSuitStateAirIdle::OnExit()
{
}
