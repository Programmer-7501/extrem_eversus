
#include"../../../../system/main.h"
#include "mobilesuit_state_bend_backward.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../move_component/rigidbody_component.h"

namespace MobileSuitStateBendBackWardData
{
	//! デフォルトののけ反り時間
	static constexpr int k_DefaultBendBackWardTime = 60;
}

MobileSuitStateBendBackWard::MobileSuitStateBendBackWard(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_CountFrame(0)
{
}

void MobileSuitStateBendBackWard::Init()
{
	// データ取得取得
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateBendBackWard::ProcessInput()
{
}

void MobileSuitStateBendBackWard::Update()
{
	// カウントアップ
	m_CountFrame++;

	//　硬直時間を過ぎたら
	if (m_CountFrame > MobileSuitStateBendBackWardData::k_DefaultBendBackWardTime)
	{
		if (m_Owner == nullptr || m_RigidBodyComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateLandingRecovery::Update nullptr");
			return;
		}

		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}

	}
}

void MobileSuitStateBendBackWard::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter m_AnimationComponentがnullptr");
		return;
	}

	m_AnimationComponent->ResetAnimation("bendbackward");

	// カウント初期化
	m_CountFrame = 0;

	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateLandingRecovery::OnEnter nullptr");
		return;
	}

	m_RigidBodyComponent->SetDirection(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	m_RigidBodyComponent->SetIsUseGravity(false);
}

void MobileSuitStateBendBackWard::OnExit()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateLandingRecovery::OnEnter nullptr");
		return;
	}
	m_RigidBodyComponent->SetIsUseGravity(true);
}
