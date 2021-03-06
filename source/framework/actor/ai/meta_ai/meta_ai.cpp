
#include"../../../../system/main.h"
#include "meta_ai.h"
#include"../../../component/mesh_component/model_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../../directx/resource/model_manager.h"

namespace MetaAIData
{
	static constexpr float k_PlayerDistance = 300.0f;
}

MetaAI::MetaAI()
{
}

MetaAI::~MetaAI()
{
}

void MetaAI::LoadActor()
{
}

void MetaAI::InitActor()
{
}

void MetaAI::UpdateActor()
{
	// プレイヤーがどっちを狙っているか
	int playerTargetNumber = m_MobileSuits[PLAYER]->GetTargetNumber();

	// 0番を狙っているなら
	if (playerTargetNumber == 0)
	{
		// 1番がダウンしてるなら
		if (m_MobileSuits[ENEMY002]->GetDownFlag() == true)
		{
			// 0番を狙う
			m_MobileSuits[ALLY]->SetTargetNumber(0);
		}
		else
		{
			// 1番を狙う
			m_MobileSuits[ALLY]->SetTargetNumber(1);
		}

	}
	else
	{
		// 0番がダウンしてるなら
		if (m_MobileSuits[ENEMY001]->GetDownFlag() == true)
		{
			// 1番を狙う
			m_MobileSuits[ALLY]->SetTargetNumber(1);
		}
		else
		{
			// 0番を狙う
			m_MobileSuits[ALLY]->SetTargetNumber(0);
		}
	}

	// プレイヤーがダウンしているなら
	if (m_MobileSuits[PLAYER]->GetDownFlag() == true)
	{
		m_MobileSuits[ENEMY001]->SetTargetNumber(1);
		m_MobileSuits[ENEMY002]->SetTargetNumber(1);
	}
	else
	{
		m_MobileSuits[ENEMY001]->SetTargetNumber(1);
		m_MobileSuits[ENEMY002]->SetTargetNumber(0);
	}
}

void MetaAI::DecideRoute()
{


}
