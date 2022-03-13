
#include"../../../system/main.h"
#include "life_component.h"

namespace LifeData
{
	static constexpr float k_DefaultLife = 600.0f;
}

LifeComponent::LifeComponent(Actor * owner)
	: Component(owner)
	, m_MaxLife(LifeData::k_DefaultLife)
	, m_CurrentLife(LifeData::k_DefaultLife)
{
}

LifeComponent::~LifeComponent()
{
}

void LifeComponent::PlusLife(float life)
{
	float tmpLife = m_CurrentLife + life;
	// クランプする
	m_CurrentLife = Math::Clamp(tmpLife, 0.0f, m_MaxLife);
}

void LifeComponent::SetLife(float life)
{	// クランプする
	m_CurrentLife = Math::Clamp(life, 0.0f, m_MaxLife);
}
