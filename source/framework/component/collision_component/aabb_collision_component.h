/**
 * @file aabb_collision_component.h
 * @brief AABB�R���W�����R���|�[�l���g
 */

#pragma once

#include"collision_component.h"
#include"../../../math/collision/aabb_collision.h"

 /**
 * @brief AABB�R���W�����R���|�[�l���g
 */
class AABBCollisionComponent : public CollisionComponent
{
private:
	//! �I�t�Z�b�g�|�W�V����
	Conv_XM::Vector3f m_OffsetPosition;
	//! �I�t�Z�b�g�X�P�[��
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