
#include"../../../../system/main.h"
#include "mobilesuit_state_landing_recovery.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../move_component/rigidbody_component.h"

MobileSuitStateLandingRecovery::MobileSuitStateLandingRecovery(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
	, m_RecoveryFrame(0)
{
}

void MobileSuitStateLandingRecovery::Init()
{
	// データ取得取得
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateLandingRecovery::ProcessInput()
{
}

void MobileSuitStateLandingRecovery::Update()
{
	//　硬直時間を過ぎたら
	if (m_CountFrame > m_RecoveryFrame)
	{
		if (m_Owner == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateLandingRecovery::Update m_Ownerがnullptr");
			return;
		}
		m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
	}

	// カウントアップ
	m_CountFrame++;
}

void MobileSuitStateLandingRecovery::OnEnter()
{
	if (m_BoostComponent == nullptr || m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateLandingRecovery::OnEnter nullptr");
		return;
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("landing", 5);

	// カウント初期化
	m_CountFrame = 0;

	// ブーストの割合取得
	float boostRatio = m_BoostComponent->GetBoostRatio();

	// 割合によって硬直時間を決定
	if (boostRatio > 0.5f)
	{
		m_RecoveryFrame = 26;
	}
	else if (boostRatio > 0.01f)
	{
		m_RecoveryFrame = 28;
	}
	else
	{
		m_RecoveryFrame = 42;
	}

	m_RigidBodyComponent->SetGroundFriction(0.05f);
}

void MobileSuitStateLandingRecovery::OnExit()
{
	// カウント初期化
	m_CountFrame = 0;
	// ブーストリセット
	m_BoostComponent->ResetBoost();


	m_RigidBodyComponent->SetGroundFriction(0.2f);
}
