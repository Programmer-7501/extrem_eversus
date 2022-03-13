#include "component.h"

Component::Component(Actor * owner, int updateOrder)
	: m_Owner(owner)
	, m_UpdateOrder(updateOrder)
{
}

Component::~Component()
{
}

void Component::Load()
{
}

void Component::Init()
{
}

void Component::Uninit()
{
}

void Component::ProcessInput()
{
}

void Component::Update()
{
}

void Component::OnUpdateWorldTransform()
{
}
