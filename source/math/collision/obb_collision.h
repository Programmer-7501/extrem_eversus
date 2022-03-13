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
	* @brief OBB�̃R���W����
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