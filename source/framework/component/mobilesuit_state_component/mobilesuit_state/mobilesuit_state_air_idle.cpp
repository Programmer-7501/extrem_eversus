
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
	//! デフォルトのジャンプ速度
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
		Logger::GetInstance().SetLog("MobileSuitStateAirIdle::Init GetDataが失敗");
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

	//! AIの場合
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		// 特殊射撃入力
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) &&
			m_CannonBulletManager->GetCurrentBullet() > 0)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP))// ジャンプ入力待機状態に移行
		{
			m_RigidBodyComponent->PlusVelocity(Conv_XM::Vector3f(0.0f, m_JumpSpeed, 0.0f));
			m_RigidBodyComponent->SetIsGround(false);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostJump");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))// 射撃入力
		{
			m_MobileSuit->BeamRifleShot();
		}
	}

	// 接地していたら
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
		Logger::GetInstance().SetLog("MobileSuitStateAirIdle::OnEnter m_AnimationComponentがnullptr");
		return;
	}
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("idle", 20);
}

void MobileSuitStateAirIdle::OnExit()
{
}
