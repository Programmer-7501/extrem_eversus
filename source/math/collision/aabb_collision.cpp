
#include"../../system/main.h"
#include"aabb_collision.h"

float Collision::SqDistPointAABB(const AABBCollision & aabb, const Conv_XM::Vector3f & point)
{
	float sqDist = 0.0f;

	if (point.x < aabb.Min.x) sqDist += (aabb.Min.x - point.x) * (aabb.Min.x - point.x);
	if (point.x > aabb.Max.x) sqDist += (point.x - aabb.Max.x) * (point.x - aabb.Max.x);

	if (point.y < aabb.Min.y) sqDist += (aabb.Min.y - point.y) * (aabb.Min.y - point.y);
	if (point.y > aabb.Max.y) sqDist += (point.y - aabb.Max.y) * (point.y - aabb.Max.y);

	if (point.z < aabb.Min.z) sqDist += (aabb.Min.z - point.z) * (aabb.Min.z - point.z);
	if (point.z > aabb.Max.z) sqDist += (point.z - aabb.Max.z) * (point.z - aabb.Max.z);

	return sqDist;
}

bool Collision::Contains(const AABBCollision & aabb, const Conv_XM::Vector3f & point)
{
	bool outside = point.x < aabb.Min.x ||
		point.y < aabb.Min.y ||
		point.z < aabb.Min.z ||
		point.x > aabb.Max.x ||
		point.y > aabb.Max.y ||
		point.z > aabb.Max.z;

	return !outside;
}
