
#include"../../../../system/main.h"
#include "mobilesuit_state_return.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../move_component/rigidbody_component.h"

namespace MobileSuitAIStateReturnData
{
	//! デフォルトののけ反り時間
	static constexpr int k_DefaultBendBackWardTime = 50;
}


MobileSuitStateReturn::MobileSuitStateReturn(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
{
}

void MobileSuitStateReturn::Init()
{
	// データ取得取得
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateReturn::ProcessInput()
{
}

void MobileSuitStateReturn::Update()
{
	// カウントアップ
	m_CountFrame++;

	//　硬直時間を過ぎたら
	if (m_CountFrame > MobileSuitAIStateReturnData::k_DefaultBendBackWardTime)
	{
		if (m_Owner == nullptr || m_RigidBodyComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateLandingRecovery::Update nullptr");
			return;
		}

		m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
	}
}

void MobileSuitStateReturn::OnEnter()
{
	if (m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr || m_BoostComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter nullptr");
		return;
	}

	// カウント初期化
	m_CountFrame = 0;

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("return", 5);
}

void MobileSuitStateReturn::OnExit()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter m_AnimationComponentがnullptr");
		return;
	}

	m_AnimationComponent->SetAnimation("idle");
}
