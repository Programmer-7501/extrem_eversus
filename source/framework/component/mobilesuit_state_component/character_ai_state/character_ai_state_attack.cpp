
#include"../../../../system/main.h"
#include "character_ai_state_attack.h"
#include"../mobilesuit_ai_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../mobilesuit_state/mobilesuit_state.h"
#include"../../mobilesuit_component/boost_component.h"

namespace CharcterAIStateAttackData
{
	//! �o�H���ĒT�����鎞��
	static constexpr int k_RePathfindingTime = 60 * 4;

	//! ���̒T���łǂꂾ���i�ނ�
	static constexpr float k_MoveLength = 100.0f;

	//! �u�[�X�g�_�b�V�������Ă��玟�Ƀu�[�X�g�_�b�V��������܂ł̊�
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
		Logger::GetInstance().SetLog("CharcterAIStateAttack::Init GetData�����s");
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

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateAttackData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// �o�H�T���p�J�E���g��0�ɂ���
		m_PathfindingTime = 0;
	}

	// �ړ��������擾
	Conv_XM::Vector3f myPos = m_MobileSuit->GetPosition();
	WeightedGraphNode* nextNode = m_OwnerComponent->GetNextNode();
	Conv_XM::Vector2f moveDir = Conv_XM::Vector2f(nextNode->Position.x - myPos.x, nextNode->Position.z - myPos.z);
	moveDir = DirectX::XMVector3Normalize(moveDir);
	m_OwnerComponent->SetInputDirection(moveDir);

	// ���r���X�[�c�̏�Ԏ擾
	MobileSuitState* currentMobileSuitState = m_OwnerComponent->GetCurrentMobileSuitState();

	// ���r���X�[�c�̖��O�擾
	std::string currentMobileSuitStateName = currentMobileSuitState->GetName();

	// ���b�N�I����ԂȂ�
	if (m_MobileSuit->GetIsLockOn() == true)
	{
		// �r�[�����C�t����łĂ��ԂȂ�
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
		else// �r�[�����C�t����łĂȂ���ԂȂ�
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

			// ��U��߂�
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

		targetPos = myPos + (myPosToEnemyUnitVec * CharcterAIStateAttackData::k_MoveLength);

		m_OwnerComponent->Pathfinding(targetPos);

		// �o�H�T���p�J�E���g��0�ɂ���
		m_PathfindingTime = 0;
	}

	m_FrameCount = 0;
	m_BoostDashCoolTime = 0;
	m_BeamRifleShotNum = 0;
}

void CharcterAIStateAttack::OnExit()
{
}
