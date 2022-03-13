
#include"../../../system/main.h"
#include "collision_component.h"

CollisionComponent::CollisionComponent(Actor * owner)
	: Component(owner)
	, m_IsUseFlag(true)
{
}

CollisionComponent::~CollisionComponent()
{
}
