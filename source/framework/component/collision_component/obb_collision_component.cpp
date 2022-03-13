
#include"../../../system/main.h"
#include "obb_collision_component.h"
#include"../../actor/actor.h"
#include"../mesh_component/debug/obb_debug_view_component.h"

OBBCollisionComponent::OBBCollisionComponent(Actor * owner)
	:CollisionComponent(owner)
	, m_OffsetPosition(Conv_XM::Vector3f(0.0f,0.0f,0.0f))
	, m_OffsetScale(Conv_XM::Vector3f(1.0f, 1.0f, 1.0f))
	, m_OffsetQuaternion(DirectX::XMQuaternionIdentity())
	, m_OBB{}
	, m_OBBDebugView(nullptr)
{

}

OBBCollisionComponent::~OBBCollisionComponent()
{
}

void OBBCollisionComponent::Load()
{

}

void OBBCollisionComponent::Init()
{
	//! OBBのデバッグビュー取得
	m_OBBDebugView = m_Owner->GetComponent<OBBDebugViewComponent>();
}

void OBBCollisionComponent::Update()
{
	if (m_IsUseFlag == false)
	{
		m_OBBDebugView->SetVisible(false);
	}
	else
	{
		m_OBBDebugView->SetVisible(true);
	}
}

void OBBCollisionComponent::OnUpdateWorldTransform()
{
	//座標入力
	Conv_XM::Vector4f myquaternion = m_Owner->GetQuaternion();

	Conv_XM::Vector4f multQuaternion = DirectX::XMQuaternionMultiply(m_OffsetQuaternion, myquaternion);

	Conv_XM::Vector3f rotateOffsetPosition = DirectX::XMVector3Rotate(m_OffsetPosition, multQuaternion);

	m_OBB.Position = m_Owner->GetPosition() + rotateOffsetPosition;

	m_OBB.Quaternion = multQuaternion;

	m_OBB.Scale = m_OffsetScale;

	// デバッグビュー入力
	if (m_OBBDebugView)
	{
		m_OBBDebugView->SetTransform(m_OBB.Position, m_OffsetScale, multQuaternion);
	}
}
