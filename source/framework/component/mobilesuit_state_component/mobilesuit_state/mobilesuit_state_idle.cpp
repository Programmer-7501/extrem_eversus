
#include"../../../../system/main.h"
#include "mobilesuit_state_idle.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mesh_component/animation_model_component.h"


namespace StateIdleData
{
	//! デフォルトのジャンプ速度
	static constexpr float k_DefaultJumpSpeed = 0.1f;
}

MobileSuitStateIdle::MobileSuitStateIdle(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_JumpSpeed(StateIdleData::k_DefaultJumpSpeed)
{
}

void MobileSuitStateIdle::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateIdle::ProcessInput()
{
	// nullチェック
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
		// インプットマネージャー取得
		InputManager& inputManager = InputManager::GetInstance();


		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) && 
			m_CannonBulletManager->GetCurrentBullet() > 0)// 特殊射撃入力
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK) == true)// 歩き状態に移行
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateWalk");

			m_Owner->SetStepReserve(MobileSuitStateComponent::SD_FORWARD);
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) == true)// ブーストジャンプ状態に移行
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateJumpStandby");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) == true)// 射撃入力
		{
			m_MobileSuit->BeamRifleShot();
		}

		// 格闘入力
		if (inputManager.GetIsTrigger(InputManager::IN_KAKUTOU))
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateNKakutouFirst");
		}
	}
}

void MobileSuitStateIdle::Update()
{

}

void MobileSuitStateIdle::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		//　初期化前に使う可能性があるので
		m_AnimationComponent = m_Owner->GetAnimationModelComponent();

		if (m_AnimationComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateIdle::OnEnter m_AnimationComponentがnullptr");
			m_AnimationComponent->SetAnimation("idle");

			return;
		}
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("idle", 20);
}

void MobileSuitStateIdle::OnExit()
{
}
