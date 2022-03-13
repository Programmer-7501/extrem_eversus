
#include"../../system/main.h"
#include"obb_collision.h"

Conv_XM::Vector3f Collision::GetDirection(const OBBCollision & obb, int dir)
{
	Conv_XM::Vector3f res = Conv_XM::Vector3f(0.0f, 0.0f, 0.0f);
	switch (dir)
	{
	case 0:
		res = DirectX::XMVector3Rotate(Conv_XM::Vector3f::UnitX, obb.Quaternion);
		break;
	case 1:
		res = DirectX::XMVector3Rotate(Conv_XM::Vector3f::UnitY, obb.Quaternion);
		break;
	case 2:
		res = DirectX::XMVector3Rotate(Conv_XM::Vector3f::UnitZ, obb.Quaternion);
		break;
	default:
		res = Conv_XM::Vector3f(0.0f, 0.0f, 0.0f);
		break;
	}

	return res;
}
