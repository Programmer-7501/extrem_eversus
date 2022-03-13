
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
	// ƒvƒŒƒCƒ„[‚ª‚Ç‚Á‚¿‚ğ‘_‚Á‚Ä‚¢‚é‚©
	int playerTargetNumber = m_MobileSuits[PLAYER]->GetTargetNumber();

	// 0”Ô‚ğ‘_‚Á‚Ä‚¢‚é‚È‚ç
	if (playerTargetNumber == 0)
	{
		// 1”Ô‚ªƒ_ƒEƒ“‚µ‚Ä‚é‚È‚ç
		if (m_MobileSuits[ENEMY002]->GetDownFlag() == true)
		{
			// 0”Ô‚ğ‘_‚¤
			m_MobileSuits[ALLY]->SetTargetNumber(0);
		}
		else
		{
			// 1”Ô‚ğ‘_‚¤
			m_MobileSuits[ALLY]->SetTargetNumber(1);
		}

	}
	else
	{
		// 0”Ô‚ªƒ_ƒEƒ“‚µ‚Ä‚é‚È‚ç
		if (m_MobileSuits[ENEMY001]->GetDownFlag() == true)
		{
			// 1”Ô‚ğ‘_‚¤
			m_MobileSuits[ALLY]->SetTargetNumber(1);
		}
		else
		{
			// 0”Ô‚ğ‘_‚¤
			m_MobileSuits[ALLY]->SetTargetNumber(0);
		}
	}

	// ƒvƒŒƒCƒ„[‚ªƒ_ƒEƒ“‚µ‚Ä‚¢‚é‚È‚ç
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
