
#include"../../../../system/main.h"
#include "mobilesuit_state_down.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../move_component/rigidbody_component.h"


namespace MobileSuitAIStateDownData
{
	//! デフォルトののけ反り時間
	static constexpr int k_DefaultBendBackWardTime = 70;
}


MobileSuitStateDown::MobileSuitStateDown(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
{
}

void MobileSuitStateDown::Init()
{
	// データ取得取得
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateDown::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateDown::ProcessInput()
{
}

void MobileSuitStateDown::Update()
{
	// カウントアップ
	m_CountFrame++;

	//　硬直時間を過ぎたら
	if (m_CountFrame > MobileSuitAIStateDownData::k_DefaultBendBackWardTime)
	{
		if (m_Owner == nullptr || m_RigidBodyComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateDown::Update nullptr");
			return;
		}

		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateReturn");
		}
	}
}

void MobileSuitStateDown::OnEnter()
{
	if (m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr || m_BoostComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter nullptr");
		return;
	}

	// カウント初期化
	m_CountFrame = 0;

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("down", 5);

	m_RigidBodyComponent->SetIsUseGravity(true);
}

void MobileSuitStateDown::OnExit()
{
}
