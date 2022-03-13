
#include"../../../../system/main.h"
#include "character_ai_state.h"
#include"../mobilesuit_ai_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"

bool CharcterAIState::GetData()
{

	if (m_OwnerComponent == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_Ownerがnullptr");
		return false;
	}

	// モビルスーツアクター取得
	m_MobileSuit = dynamic_cast<MobileSuit*>(m_OwnerComponent->GetOwner());
	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_MobileSuitがnullptr");
		return false;
	}

	// ブーストコンポーネント取得
	m_BoostComponent = m_OwnerComponent->GetBoostComponent();
	if (m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BoostComponentがnullptr");
		return false;
	}

	// エネミーを入力
	m_EnemyMobileSuit001 = m_MobileSuit->GetEnemyMobileSuit001();
	if (m_EnemyMobileSuit001 == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_EnemyMobileSuit001がnullptr");
		return false;
	}

	m_EnemyMobileSuit002 = m_MobileSuit->GetEnemyMobileSuit002();
	if (m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("CharcterAIState::GetData m_EnemyMobileSuit002がnullptr");
		return false;
	}

	return true;
}
