/**
 * @file aabb_collision_component.h
 * @brief AABBコリジョンコンポーネント
 */

#pragma once

#include"collision_component.h"
#include"../../../math/collision/aabb_collision.h"

 /**
 * @brief AABBコリジョンコンポーネント
 */
class AABBCollisionComponent : public CollisionComponent
{
private:
	//! オフセットポジション
	Conv_XM::Vector3f m_OffsetPosition;
	//! オフセットスケール
	Conv_XM::Vector3f m_OffsetScale;

	//! OBB
	Collision::AABBCollision m_AABB;

	class AABBDebugViewComponent* m_AABBDebugViewComponent;
public:
	AABBCollisionComponent(class Actor* owner);
	~AABBCollisionComponent();

	void Load() override;

	void Init()override;

	void Update()override;

	void OnUpdateWorldTransform()override;

	inline void SetOffsetTransform(
		const Conv_XM::Vector3f& offsetPosition,
		const Conv_XM::Vector3f& offsetScale)
	{
		m_OffsetPosition = offsetPosition;
		m_OffsetScale = offsetScale;
	}

	inline void SetOffsetPosition(const Conv_XM::Vector3f& offsetPosition) { m_OffsetPosition = offsetPosition; }
	inline void SetOffsetScale(const Conv_XM::Vector3f& offsetScale) { m_OffsetScale = offsetScale; }

	const Collision::AABBCollision& GetAABB() { return m_AABB; }
};