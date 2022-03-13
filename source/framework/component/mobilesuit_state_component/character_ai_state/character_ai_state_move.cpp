
#include"../../../../system/main.h"
#include "character_ai_state_move.h"
#include"../mobilesuit_ai_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../mobilesuit_state/mobilesuit_state.h"
#include"../../mobilesuit_component/boost_component.h"

namespace CharcterAIStateMoveData
{
	//! 経路を再探索する時間
	static constexpr int k_RePathfindingTime = 60 * 4;

	//! 一回の探索でどれだけ進むか
	static constexpr float k_MoveLength = 100.0f;

	//! 攻撃へ移るカウント
	static constexpr int k_AttackCount = 240;
}

CharcterAIStateMove::CharcterAIStateMove(MobileSuitAIStateComponent * ownerComponent)
	:CharcterAIState(ownerComponent)
	, m_FrameCount(0)
	, m_PathfindingTime(0)
{

}

void CharcterAIStateMove::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("CharcterAIStateMove::Init GetDataが失敗");
		return;
	}
}

void CharcterAIStateMove::ProcessInput()
{
	if (m_OwnerComponent == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter m_OwnerComponentがnullptr");
		return;
	}

	// 経路探索済みかどうか
	if (m_OwnerComponent->GetIsPathfinding() == false || m_PathfindingTime > CharcterAIStateMoveData::k_RePathfindingTime)
	{
		if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
		{
			Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter nullptr");
			return;
		}

		// どっちを狙っているか取得
		int targetNum = m_MobileSuit->GetTargetNumber();
		Conv_XM::Vector3f targetPos,myPos, enemyPos, myPosToEnemyUnitVec;
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

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateMoveData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// 経路探索用カウントを0にする
		m_PathfindingTime = 0;
	}

	// モビルスーツの状態取得
	MobileSuitState* currentMobileSuitState = m_OwnerComponent->GetCurrentMobileSuitState();

	// モビルスーツの名前取得
	std::string currentMobileSuitStateName = currentMobileSuitState->GetName();

	// 移動方向を取得
	Conv_XM::Vector3f myPos = m_MobileSuit->GetPosition();
	WeightedGraphNode* nextNode = m_OwnerComponent->GetNextNode();
	Conv_XM::Vector2f moveDir = Conv_XM::Vector2f(nextNode->Position.x - myPos.x, nextNode->Position.z - myPos.z);
	moveDir = DirectX::XMVector3Normalize(moveDir);
	m_OwnerComponent->SetInputDirection(moveDir);

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
		// ブーストの残量を取得
		float ratio = m_BoostComponent->GetBoostRatio();

		if (ratio > 0.5f)
		{
			m_OwnerComponent->SetIsPressInput(MobileSuitStateComponent::IN_JUMP, true);
		}

	}

	// ロックオン状態なら
	if (m_MobileSuit->GetIsLockOn() == true)
	{
		if (m_FrameCount > CharcterAIStateMoveData::k_AttackCount)
		{
			m_OwnerComponent->ChangeCharacterAIState("CharcterAIStateAttack");
		}
	}

}

void CharcterAIStateMove::Update()
{
	// カウントアップする
	m_PathfindingTime++;
	m_FrameCount++;
}

void CharcterAIStateMove::OnEnter()
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

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateMoveData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// 経路探索用カウントを0にする
		m_PathfindingTime = 0;
	}

	m_FrameCount = 0;
}

void CharcterAIStateMove::OnExit()
{
}
