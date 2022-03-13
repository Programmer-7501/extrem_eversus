/**
 * @file obb_collision.h
 * @brief ���R���W����
 */

#pragma once

 /**
 * @namespace Collision
 * @brief �R���W�����W��
 */
namespace Collision
{
	/**
	* @brief AABB�̃R���W����
	*/
	struct AABBCollision
	{
		Conv_XM::Vector3f Min;
		Conv_XM::Vector3f Max;
	};

	float SqDistPointAABB(const AABBCollision& aabb, const Conv_XM::Vector3f& point);

	bool Contains(const AABBCollision& aabb, const Conv_XM::Vector3f& point);
}