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
	* @brief OBBのコリジョン
	*/
	struct OBBCollision
	{
		Conv_XM::Vector3f Position;
		Conv_XM::Vector4f Quaternion;
		Conv_XM::Vector3f Scale;
	};

	// dir 0 : x, 1 : y, 2 : z
	Conv_XM::Vector3f GetDirection(const OBBCollision& obb, int dir);
}