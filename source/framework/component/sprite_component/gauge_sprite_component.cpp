
#include"../../../system/main.h"
#include "gauge_sprite_component.h"

GaugeSpriteComponent::GaugeSpriteComponent(Actor * owner)
	:ChangeableSpriteComponent(owner)
	, m_Direction(LEFT)
	, m_Ratio(1.0f)
	, m_MaxLength(300.0f)
	, m_StartPosition(0.0f,0.0f)
{
}

GaugeSpriteComponent::~GaugeSpriteComponent()
{
}

void GaugeSpriteComponent::Update()
{
	float length = m_MaxLength * m_Ratio;
	m_Size.x = length;

	m_Position = m_StartPosition;

	if (m_Direction == LEFT)
	{
		m_Position.x -= length / 2.0f;
	}
	else
	{
		m_Position.x += length / 2.0f;
	}

	//Å@åpè≥å≥ÇÃçXêV
	ChangeableSpriteComponent::Update();
}
