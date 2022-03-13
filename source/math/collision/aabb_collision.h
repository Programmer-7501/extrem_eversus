/**
 * @file obb_collision.h
 * @brief 球コリジョン
 */

#pragma once

 /**
 * @namespace Collision
 * @brief コリジョン集約
 */
namespace Collision
{
	/**
	* @brief AABBのコリジョン
	*/
	struct AABBCollision
	{
		Conv_XM::Vector3f Min;
		Conv_XM::Vector3f Max;
	};

	float SqDistPointAABB(const AABBCollision& aabb, const Conv_XM::Vector3f& point);

	bool Contains(const AABBCollision& aabb, const Conv_XM::Vector3f& point);
}