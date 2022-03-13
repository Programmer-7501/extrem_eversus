/**
 * @file obb_collision_component.h
 * @brief OBBコリジョンコンポーネント
 */

#pragma once

#include"collision_component.h"
#include"../../../math/collision/obb_collision.h"

 /**
 * @brief OBBコリジョンコンポーネント
 */
class OBBCollisionComponent : public CollisionComponent
{
private:
	//! オフセットポジション
	Conv_XM::Vector3f m_OffsetPosition;
	//! オフセットスケール
	Conv_XM::Vector3f m_OffsetScale;
	//! オフセットローテーション
	Conv_XM::Vector4f m_OffsetQuaternion;

	//! OBB
	Collision::OBBCollision m_OBB;

	class OBBDebugViewComponent* m_OBBDebugView;
public:
	OBBCollisionComponent(class Actor* owner);
	~OBBCollisionComponent();

	void Load() override;

	void Init()override;

	void Update()override;

	void OnUpdateWorldTransform()override;

	inline void SetOffsetTransform(
		const Conv_XM::Vector3f& offsetPosition,
		const Conv_XM::Vector3f& offsetScale,
		const Conv_XM::Vector4f& offsetQuaternion)
	{
		m_OffsetPosition = offsetPosition;
		m_OffsetScale = offsetScale;
		m_OffsetQuaternion = offsetQuaternion;
	}

	inline void SetOffsetPosition(const Conv_XM::Vector3f& offsetPosition) { m_OffsetPosition = offsetPosition; }
	inline void SetOffsetScale(const Conv_XM::Vector3f& offsetScale) { m_OffsetScale = offsetScale; }
	inline void SetOffsetQuaternion(const Conv_XM::Vector4f& offsetQuaternion) { m_OffsetQuaternion = offsetQuaternion; }

	const Collision::OBBCollision& GetOBB() { return m_OBB; }
};