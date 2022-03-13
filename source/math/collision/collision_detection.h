/**
 * @file triangle_collision.h
 * @brief 三角形コリジョン
 */

#pragma once

#include"linesegment_collision.h"
#include"obb_collision.h"
#include"aabb_collision.h"

 /**
 * @namespace Collision
 * @brief コリジョン集約
 */
namespace Collision
{
	bool Intersect(const OBBCollision &obb_A, const OBBCollision &obb_B);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(Conv_XM::Vector3f *Sep, Conv_XM::Vector3f *e1, Conv_XM::Vector3f *e2, Conv_XM::Vector3f *e3 = 0);

	bool Intersect(const AABBCollision &aabb_A, const AABBCollision &aabb_B, Conv_XM::Vector3f* hitinfo);

	bool Intersect(const AABBCollision &aabb, const OBBCollision &obb);
}