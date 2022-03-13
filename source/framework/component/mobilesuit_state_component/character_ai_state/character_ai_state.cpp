
#include"../../../../system/main.h"
#include "character_ai_state.h"
#include"../mobilesuit_ai_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"

bool CharcterAIState::GetData()
{

	if (m_OwnerComponent == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_Owner��nullptr");
		return false;
	}

	// ���r���X�[�c�A�N�^�[�擾
	m_MobileSuit = dynamic_cast<MobileSuit*>(m_OwnerComponent->GetOwner());
	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_MobileSuit��nullptr");
		return false;
	}

	// �u�[�X�g�R���|�[�l���g�擾
	m_BoostComponent = m_OwnerComponent->GetBoostComponent();
	if (m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BoostComponent��nullptr");
		return false;
	}

	// �G�l�~�[�����
	m_EnemyMobileSuit001 = m_MobileSuit->GetEnemyMobileSuit001();
	if (m_EnemyMobileSuit001 == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_EnemyMobileSuit001��nullptr");
		return false;
	}

	m_EnemyMobileSuit002 = m_MobileSuit->GetEnemyMobileSuit002();
	if (m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_EnemyMobileSuit002��nullptr");
		return false;
	}

	return true;
}
