
#include"../../../system/main.h"
#include "boost_component.h"

namespace BoostData
{
	constexpr float k_DefaultBoost = 600.0f;
}

BoostComponent::BoostComponent(Actor * owner)
	: Component(owner)
	, m_MaxBoost(BoostData::k_DefaultBoost)
	, m_CurrentBoost(BoostData::k_DefaultBoost)
{
}

BoostComponent::~BoostComponent()
{
}

bool BoostComponent::GetIsOverHeat() const
{
	if (m_CurrentBoost < Math::k_Epsilon)
	{
		return true;
	}

	return false;
}