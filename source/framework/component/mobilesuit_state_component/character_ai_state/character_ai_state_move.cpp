
#include"../../../../system/main.h"
#include "character_ai_state_move.h"
#include"../mobilesuit_ai_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../mobilesuit_state/mobilesuit_state.h"
#include"../../mobilesuit_component/boost_component.h"

namespace CharcterAIStateMoveData
{
	//! �o�H���ĒT�����鎞��
	static constexpr int k_RePathfindingTime = 60 * 4;

	//! ���̒T���łǂꂾ���i�ނ�
	static constexpr float k_MoveLength = 100.0f;

	//! �U���ֈڂ�J�E���g
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
		Logger::GetInstance().SetLog("CharcterAIStateMove::Init GetData�����s");
		return;
	}
}

void CharcterAIStateMove::ProcessInput()
{
	if (m_OwnerComponent == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter m_OwnerComponent��nullptr");
		return;
	}

	// �o�H�T���ς݂��ǂ���
	if (m_OwnerComponent->GetIsPathfinding() == false || m_PathfindingTime > CharcterAIStateMoveData::k_RePathfindingTime)
	{
		if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
		{
			Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter nullptr");
			return;
		}

		// �ǂ�����_���Ă��邩�擾
		int targetNum = m_MobileSuit->GetTargetNumber();
		Conv_XM::Vector3f targetPos,myPos, enemyPos, myPosToEnemyUnitVec;
		// �����̈���擾
		myPos = m_MobileSuit->GetPosition();
		if (targetNum == 0)
		{
			enemyPos = m_EnemyMobileSuit001->GetPosition();
		}
		else
		{
			enemyPos = m_EnemyMobileSuit002->GetPosition();
		}

		// �����̏ꏊ����G�̏ꏊ�܂ł̒P�ʃx�N�g��
		myPosToEnemyUnitVec = enemyPos - myPos;
		myPosToEnemyUnitVec = DirectX::XMVector3Normalize(myPosToEnemyUnitVec);

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateMoveData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// �o�H�T���p�J�E���g��0�ɂ���
		m_PathfindingTime = 0;
	}

	// ���r���X�[�c�̏�Ԏ擾
	MobileSuitState* currentMobileSuitState = m_OwnerComponent->GetCurrentMobileSuitState();

	// ���r���X�[�c�̖��O�擾
	std::string currentMobileSuitStateName = currentMobileSuitState->GetName();

	// �ړ��������擾
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
		// �u�[�X�g�̎c�ʂ��擾
		float ratio = m_BoostComponent->GetBoostRatio();

		if (ratio > 0.5f)
		{
			m_OwnerComponent->SetIsPressInput(MobileSuitStateComponent::IN_JUMP, true);
		}

	}

	// ���b�N�I����ԂȂ�
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
	// �J�E���g�A�b�v����
	m_PathfindingTime++;
	m_FrameCount++;
}

void CharcterAIStateMove::OnEnter()
{
	if (m_OwnerComponent == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter m_OwnerComponent��nullptr");
		return;
	}

	// �o�H�T���ς݂��ǂ���
	if (m_OwnerComponent->GetIsPathfinding() == false)
	{
		if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
		{
			Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter nullptr");
			return;
		}

		// �ǂ�����_���Ă��邩�擾
		int targetNum = m_MobileSuit->GetTargetNumber();
		Conv_XM::Vector3f targetPos, myPos, enemyPos, myPosToEnemyUnitVec;
		// �����̈���擾
		myPos = m_MobileSuit->GetPosition();
		if (targetNum == 0)
		{
			enemyPos = m_EnemyMobileSuit001->GetPosition();
		}
		else
		{
			enemyPos = m_EnemyMobileSuit002->GetPosition();
		}

		// �����̏ꏊ����G�̏ꏊ�܂ł̒P�ʃx�N�g��
		myPosToEnemyUnitVec = enemyPos - myPos;
		myPosToEnemyUnitVec = DirectX::XMVector3Normalize(myPosToEnemyUnitVec);

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateMoveData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// �o�H�T���p�J�E���g��0�ɂ���
		m_PathfindingTime = 0;
	}

	m_FrameCount = 0;
}

void CharcterAIStateMove::OnExit()
{
}
