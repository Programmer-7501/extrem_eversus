
#include"../../../system/main.h"
#include "aabb_collision_component.h"
#include"../../actor/actor.h"
#include"../mesh_component/debug/aabb_debug_view_component.h"

AABBCollisionComponent::AABBCollisionComponent(Actor * owner)
	:CollisionComponent(owner)
	, m_OffsetPosition(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f))
	, m_OffsetScale(Conv_XM::Vector3f(1.0f, 1.0f, 1.0f))
	, m_AABB{}
	, m_AABBDebugViewComponent(nullptr)
{

}

AABBCollisionComponent::~AABBCollisionComponent()
{
}

void AABBCollisionComponent::Load()
{

}

void AABBCollisionComponent::Init()
{
	//! AABBのデバッグビュー取得
	m_AABBDebugViewComponent = m_Owner->GetComponent<AABBDebugViewComponent>();
}

void AABBCollisionComponent::Update()
{
	if (m_IsUseFlag == false)
	{
		m_AABBDebugViewComponent->SetVisible(false);
	}
	else
	{
		m_AABBDebugViewComponent->SetVisible(true);
	}
}

void AABBCollisionComponent::OnUpdateWorldTransform()
{
	//構造体に値入力
	m_AABB.Min = m_Owner->GetPosition() + m_OffsetPosition - (m_OffsetScale / 2.0f);
	m_AABB.Max = m_Owner->GetPosition() + m_OffsetPosition + (m_OffsetScale / 2.0f);

	// デバッグビュー入力
	if (m_AABBDebugViewComponent)
	{
		m_AABBDebugViewComponent->SetTransform(m_Owner->GetPosition() + m_OffsetPosition, m_OffsetScale);
	}
}
