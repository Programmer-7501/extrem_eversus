
#include"../../../../system/main.h"
#include "character_ai_state_start.h"
#include"../mobilesuit_ai_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"

CharcterAIStateStart::CharcterAIStateStart(MobileSuitAIStateComponent * ownerComponent)
	:CharcterAIState(ownerComponent)
	, m_FrameCount(0)
{
}

void CharcterAIStateStart::Init()
{
}

void CharcterAIStateStart::ProcessInput()
{
}

void CharcterAIStateStart::Update()
{
	m_FrameCount++;

	if (m_FrameCount > 0)
	{
		if (m_OwnerComponent == nullptr)
		{
			Logger::GetInstance().SetLog("CharcterAIStateMove::OnEnter m_OwnerComponent‚ªnullptr");
			return;
		}

		m_OwnerComponent->ChangeCharacterAIState("CharcterAIStateAttack");
	}
}

void CharcterAIStateStart::OnEnter()
{
	m_FrameCount = 0;
}

void CharcterAIStateStart::OnExit()
{
}
