
#include"../../../../system/main.h"
#include "character_ai_state_attack.h"
#include"../mobilesuit_ai_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../mobilesuit_state/mobilesuit_state.h"
#include"../../mobilesuit_component/boost_component.h"

namespace CharcterAIStateAttackData
{
	//! 経路を再探索する時間
	static constexpr int k_RePathfindingTime = 60 * 4;

	//! 一回の探索でどれだけ進むか
	static constexpr float k_MoveLength = 100.0f;

	//! ブーストダッシュをしてから次にブーストダッシュをするまでの間
	static constexpr int k_BoostDashCoolTime = 120;
}

CharcterAIStateAttack::CharcterAIStateAttack(MobileSuitAIStateComponent * ownerComponent)
	:CharcterAIState(ownerComponent)
	, m_FrameCount(0)
	, m_PathfindingTime(0)
	, m_BoostDashCoolTime(0)
	, m_BeamRifleShotNum(0)
{
}

void CharcterAIStateAttack::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("CharcterAIStateAttack::Init GetDataが失敗");
		return;
	}
}

void CharcterAIStateAttack::ProcessInput()
{
	if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIStateAttack::OnEnter nullptr");
		return;
	}

	// 経路探索済みかどうか
	if (m_OwnerComponent->GetIsPathfinding() == false)
	{
		if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
		{
			Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter nullptr");
			return;
		}

		// どっちを狙っているか取得
		int targetNum = m_MobileSuit->GetTargetNumber();
		Conv_XM::Vector3f targetPos, myPos, enemyPos, myPosToEnemyUnitVec;
		// 自分の一を取得
		myPos = m_MobileSuit->GetPosition();
		if (targetNum == 0)
		{
			enemyPos = m_EnemyMobileSuit001->GetPosition();
		}
		else
		{
			enemyPos = m_EnemyMobileSuit002->GetPosition();
		}

		// 自分の場所から敵の場所までの単位ベクトル
		myPosToEnemyUnitVec = enemyPos - myPos;
		myPosToEnemyUnitVec = DirectX::XMVector3Normalize(myPosToEnemyUnitVec);

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateAttackData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// 経路探索用カウントを0にする
		m_PathfindingTime = 0;
	}

	// 移動方向を取得
	Conv_XM::Vector3f myPos = m_MobileSuit->GetPosition();
	WeightedGraphNode* nextNode = m_OwnerComponent->GetNextNode();
	Conv_XM::Vector2f moveDir = Conv_XM::Vector2f(nextNode->Position.x - myPos.x, nextNode->Position.z - myPos.z);
	moveDir = DirectX::XMVector3Normalize(moveDir);
	m_OwnerComponent->SetInputDirection(moveDir);

	// モビルスーツの状態取得
	MobileSuitState* currentMobileSuitState = m_OwnerComponent->GetCurrentMobileSuitState();

	// モビルスーツの名前取得
	std::string currentMobileSuitStateName = currentMobileSuitState->GetName();

	// ロックオン状態なら
	if (m_MobileSuit->GetIsLockOn() == true)
	{
		// ビームライフルを打てる状態なら
		if (m_MobileSuit->GetIsBeamRifleShot() == false)
		{
			if (currentMobileSuitStateName == "MobileSuitStateIdle")
			{
				m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
			}
			if (currentMobileSuitStateName == "MobileSuitStateJumpStandby")
			{
				m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
			}

			if (currentMobileSuitStateName == "MobileSuitStateBoostDash")
			{
				m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI, true);
				m_OwnerComponent->SetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK, true);
				m_BeamRifleShotNum++;
			}
		}
		else// ビームライフルを打てない状態なら
		{
			if (currentMobileSuitStateName == "MobileSuitStateIdle")
			{
				m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
			}
			if (currentMobileSuitStateName == "MobileSuitStateJumpStandby")
			{
				m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
			}

			if (m_BoostDashCoolTime > CharcterAIStateAttackData::k_BoostDashCoolTime)
			{
				if (currentMobileSuitStateName == "MobileSuitStateBoostDash")
				{
					m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
				}
				if (currentMobileSuitStateName == "MobileSuitStateBoostJump")
				{
					m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
					m_BoostDashCoolTime = 0;
				}
			}
			else
			{
				//if (currentMobileSuitStateName == "MobileSuitStateBoostJump")
				//{
				//	m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
				//}
				if (currentMobileSuitStateName == "MobileSuitStateBoostDash")
				{
					m_OwnerComponent->SetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK, true);
				}
			}

			// 一旦やめる
			if (m_BeamRifleShotNum >= 4)
			{
				m_OwnerComponent->ChangeCharacterAIState("CharcterAIStateMove");
			}
		}
	}
	else
	{

		if (currentMobileSuitStateName == "MobileSuitStateIdle")
		{
			m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
		}
		if (currentMobileSuitStateName == "MobileSuitStateJumpStandby")
		{
			m_OwnerComponent->SetIsTriggerInput(MobileSuitStateComponent::IN_JUMP, true);
		}
		if (currentMobileSuitStateName == "MobileSuitStateBoostDash")
		{
			m_OwnerComponent->SetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK, true);
		}
	}
}

void CharcterAIStateAttack::Update()
{
	m_BoostDashCoolTime++;
	m_FrameCount++;
}

void CharcterAIStateAttack::OnEnter()
{
	if (m_OwnerComponent == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter m_OwnerComponentがnullptr");
		return;
	}

	// 経路探索済みかどうか
	if (m_OwnerComponent->GetIsPathfinding() == false)
	{
		if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
		{
			Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter nullptr");
			return;
		}

		// どっちを狙っているか取得
		int targetNum = m_MobileSuit->GetTargetNumber();
		Conv_XM::Vector3f targetPos, myPos, enemyPos, myPosToEnemyUnitVec;
		// 自分の一を取得
		myPos = m_MobileSuit->GetPosition();
		if (targetNum == 0)
		{
			enemyPos = m_EnemyMobileSuit001->GetPosition();
		}
		else
		{
			enemyPos = m_EnemyMobileSuit002->GetPosition();
		}

		// 自分の場所から敵の場所までの単位ベクトル
		myPosToEnemyUnitVec = enemyPos - myPos;
		myPosToEnemyUnitVec = DirectX::XMVector3Normalize(myPosToEnemyUnitVec);

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateAttackData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// 経路探索用カウントを0にする
		m_PathfindingTime = 0;
	}

	m_FrameCount = 0;
	m_BoostDashCoolTime = 0;
	m_BeamRifleShotNum = 0;
}

void CharcterAIStateAttack::OnExit()
{
}
