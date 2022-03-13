
#include"../../../system/main.h"
#include "arousal_component.h"

namespace ArousalComponentData
{
	//! �f�t�H���g�̍ő�o����
	static constexpr float k_DefaultMaxArousal = 100.0f;
}

ArousalComponent::ArousalComponent(Actor * owner)
	:Component(owner)
	, m_MaxArousal(ArousalComponentData::k_DefaultMaxArousal)
	, m_CurrentArousal(0)
{
}

ArousalComponent::~ArousalComponent()
{
}
