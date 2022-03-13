/**
 * @file obb_collision_component.h
 * @brief OBB�R���W�����R���|�[�l���g
 */

#pragma once

#include"collision_component.h"
#include"../../../math/collision/obb_collision.h"

 /**
 * @brief OBB�R���W�����R���|�[�l���g
 */
class OBBCollisionComponent : public CollisionComponent
{
private:
	//! �I�t�Z�b�g�|�W�V����
	Conv_XM::Vector3f m_OffsetPosition;
	//! �I�t�Z�b�g�X�P�[��
	Conv_XM::Vector3f m_OffsetScale;
	//! �I�t�Z�b�g���[�e�[�V����
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