
#include"../../system/main.h"
#include "collision_detection.h"
#include"obb_collision.h"

/*
QlΆ£ ~iάιΨ―jΒ­λ[ΗΑΖR
URL@http://marupeke296.com/COL_3D_No13_OBBvsOBB.html
*/

bool Collision::Intersect(const OBBCollision & obb_A, const OBBCollision & obb_B)
{	
	// eϋόxNgΜmΫ
	// iN***:W»ϋόxNgj
	Conv_XM::Vector3f NAe1 = GetDirection(obb_A, 0), Ae1 = NAe1 * obb_A.Scale.x;
	Conv_XM::Vector3f NAe2 = GetDirection(obb_A, 1), Ae2 = NAe2 * obb_A.Scale.y;
	Conv_XM::Vector3f NAe3 = GetDirection(obb_A, 2), Ae3 = NAe3 * obb_A.Scale.z;
	Conv_XM::Vector3f NBe1 = GetDirection(obb_B, 0), Be1 = NBe1 * obb_B.Scale.x;
	Conv_XM::Vector3f NBe2 = GetDirection(obb_B, 1), Be2 = NBe2 * obb_B.Scale.y;
	Conv_XM::Vector3f NBe3 = GetDirection(obb_B, 2), Be3 = NBe3 * obb_B.Scale.z;
	Conv_XM::Vector3f Interval = obb_A.Position - obb_B.Position;

	// ͺ£² : Ae1
	float rA = Conv_XM::Vector3f::Length(Ae1);
	float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = fabs(Conv_XM::Vector3f::Dot(Interval, NAe1));
	if (L > rA + rB)
	{
		return false; // ΥΛ΅Δ’Θ’
	}

	// ͺ£² : Ae2
	rA = Conv_XM::Vector3f::Length(Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// ͺ£² : Ae3
	rA = Conv_XM::Vector3f::Length(Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// ͺ£² : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Conv_XM::Vector3f::Length(Be1);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// ͺ£² : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Conv_XM::Vector3f::Length(Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// ͺ£² : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Conv_XM::Vector3f::Length(Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// ͺ£² : C11
	Conv_XM::Vector3f Cross;
	Conv_XM::Vector3f::Cross(NAe1, NBe1, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C12
	Conv_XM::Vector3f::Cross(NAe1, NBe2, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C13
	Conv_XM::Vector3f::Cross(NAe1, NBe3, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C21
	Conv_XM::Vector3f::Cross(NAe2, NBe1, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C22
	Conv_XM::Vector3f::Cross(NAe2, NBe2, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C23
	Conv_XM::Vector3f::Cross(NAe2, NBe3, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C31
	Conv_XM::Vector3f::Cross(NAe3, NBe1, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C32
	Conv_XM::Vector3f::Cross(NAe3, NBe2, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C33
	Conv_XM::Vector3f::Cross(NAe3, NBe3, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£½ΚͺΆέ΅Θ’ΜΕuΥΛ΅Δ’ιv
	return true;
}

float Collision::LenSegOnSeparateAxis(Conv_XM::Vector3f * Sep, Conv_XM::Vector3f * e1, Conv_XM::Vector3f * e2, Conv_XM::Vector3f * e3)
{
	// 3ΒΜΰΟΜβΞlΜaΕeόͺ·πvZ
	// ͺ£²SepΝW»³κΔ’ι±Ζ
	float r1 = fabs(Conv_XM::Vector3f::Dot(*Sep, *e1));
	float r2 = fabs(Conv_XM::Vector3f::Dot(*Sep, *e2));
	float r3 = e3 ? (fabs(Conv_XM::Vector3f::Dot(*Sep, *e3))) : 0;
	return r1 + r2 + r3;
}

bool Collision::Intersect(const AABBCollision & aabb_A, const AABBCollision & aabb_B, Conv_XM::Vector3f* hitInfo)
{
	bool no = aabb_A.Max.x < aabb_B.Min.x ||
		aabb_A.Max.y < aabb_B.Min.y ||
		aabb_A.Max.z < aabb_B.Min.z ||
		aabb_B.Max.x < aabb_A.Min.x ||
		aabb_B.Max.y < aabb_A.Min.y ||
		aabb_B.Max.z < aabb_A.Min.z;

	if (no)
	{
		return false;
	}

	if (hitInfo)
	{
		// ·πvZ
		float dx1 = aabb_A.Max.x - aabb_B.Min.x;
		float dx2 = aabb_A.Min.x - aabb_B.Max.x;
		float dy1 = aabb_A.Max.y - aabb_B.Min.y;
		float dy2 = aabb_A.Min.y - aabb_B.Max.y;
		float dz1 = aabb_A.Max.z - aabb_B.Min.z;
		float dz2 = aabb_A.Min.z - aabb_B.Max.z;

		// dx1 dx2 βΞlͺ ¬³’Ω€πγό
		float dx = fabsf(dx1) < fabsf(dx2) ?
			dx1 : dx2;

		float dy = fabsf(dy1) < fabsf(dy2) ?
			dy1 : dy2;

		float dz = fabsf(dz1) < fabsf(dz2) ?
			dz1 : dz2;

		// Η±ͺκΤί’©
		if (fabsf(dx) <= fabsf(dy) && fabsf(dx) <= fabsf(dz))
		{
			hitInfo->x = dx;
		}
		else if (fabsf(dy) <= fabsf(dx) && fabsf(dy) <= fabsf(dz))
		{
			hitInfo->y = dy;
		}
		else
		{
			hitInfo->z = dz;
		}
	}

	//Ηκΰ^ΕΘ―κΞAπ··ι
	return !no;
}

bool Collision::Intersect(const AABBCollision & aabb, const OBBCollision & obb)
{
	// eϋόxNgΜmΫ
	// iN***:W»ϋόxNgj
	Conv_XM::Vector3f NAe1 = Conv_XM::Vector3f::UnitX, Ae1 = Conv_XM::Vector3f(aabb.Max.x - aabb.Min.x, 0.0f, 0.0f);
	Conv_XM::Vector3f NAe2 = Conv_XM::Vector3f::UnitY, Ae2 = Conv_XM::Vector3f(0.0f, aabb.Max.y - aabb.Min.y,0.0f);
	Conv_XM::Vector3f NAe3 = Conv_XM::Vector3f::UnitZ, Ae3 = Conv_XM::Vector3f(0.0f, 0.0f, aabb.Max.z - aabb.Min.z);
	Conv_XM::Vector3f NBe1 = GetDirection(obb, 0), Be1 = NAe1 * obb.Scale.x;
	Conv_XM::Vector3f NBe2 = GetDirection(obb, 1), Be2 = NAe2 * obb.Scale.y;
	Conv_XM::Vector3f NBe3 = GetDirection(obb, 2), Be3 = NAe3 * obb.Scale.z;
	Conv_XM::Vector3f Interval = ((aabb.Max - aabb.Min) / 2.0f) - obb.Position;

	// ͺ£² : Ae1
	float rA = Conv_XM::Vector3f::Length(Ae1);
	float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = fabs(Conv_XM::Vector3f::Dot(Interval, NAe1));
	if (L > rA + rB)
	{
		return false; // ΥΛ΅Δ’Θ’
	}

	// ͺ£² : Ae2
	rA = Conv_XM::Vector3f::Length(Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// ͺ£² : Ae3
	rA = Conv_XM::Vector3f::Length(Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// ͺ£² : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Conv_XM::Vector3f::Length(Be1);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// ͺ£² : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Conv_XM::Vector3f::Length(Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// ͺ£² : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Conv_XM::Vector3f::Length(Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// ͺ£² : C11
	Conv_XM::Vector3f Cross;
	Conv_XM::Vector3f::Cross(NAe1, NBe1, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C12
	Conv_XM::Vector3f::Cross(NAe1, NBe2, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C13
	Conv_XM::Vector3f::Cross(NAe1, NBe3, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C21
	Conv_XM::Vector3f::Cross(NAe2, NBe1, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C22
	Conv_XM::Vector3f::Cross(NAe2, NBe2, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C23
	Conv_XM::Vector3f::Cross(NAe2, NBe3, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C31
	Conv_XM::Vector3f::Cross(NAe3, NBe1, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C32
	Conv_XM::Vector3f::Cross(NAe3, NBe2, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£² : C33
	Conv_XM::Vector3f::Cross(NAe3, NBe3, &Cross);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Conv_XM::Vector3f::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ͺ£½ΚͺΆέ΅Θ’ΜΕuΥΛ΅Δ’ιv
	return true;
}
