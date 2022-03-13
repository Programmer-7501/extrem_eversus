

#include "convenient_directxmath.h"
#include <string.h>

float Conv_XM::Vector2f::Dot(const Vector2f & a, const Vector2f & b)
{
	Vector2f angle = DirectX::XMVector2Dot(a, b);
	return angle.x;
}

const Conv_XM::Vector2f Conv_XM::Vector2f::Forward(0.0f, 1.0f);

float Conv_XM::Vector3f::Dot(const Vector3f & a, const Vector3f & b)
{
	Vector3f angle = DirectX::XMVector3Dot(a, b);
	return angle.x;
}

Conv_XM::Vector3f Conv_XM::Vector3f::Cross(const Vector3f & a, const Vector3f & b, Vector3f * pout)
{
	if (pout)
	{
		*pout = DirectX::XMVector3Cross(a, b);
		return *pout;
	}
	Vector3f res = DirectX::XMVector3Cross(a, b);
	return res;
}

float Conv_XM::Vector3f::Length(const Vector3f & a)
{
	Vector3f length = DirectX::XMVector3Length(a);
	return length.x;
}

float Conv_XM::Vector3f::LengthSq(const Vector3f & a)
{
	Vector3f lengthSq = DirectX::XMVector3LengthSq(a);
	return lengthSq.x;
}

Conv_XM::Vector3f * Conv_XM::Vector3f::Linear(Vector3f * out, const Vector3f & start, const Vector3f & end, float t)
{
	// クランプする
	t = Math::Clamp(t, 0.0f, 1.0f);

	// 差を求める
	Vector3f dif = end - start;

	// 足す
	*out = start + dif * t;

	return out;
}

Conv_XM::Vector3f* Conv_XM::Vector3f::SphereLinear(Vector3f * out, const Vector3f & start, const Vector3f & end, float t)
{
	Vector3f s, e;
	s = DirectX::XMVector3Normalize(start);
	e = DirectX::XMVector3Normalize(end);

	// 2ベクトル間の角度（鋭角側）

	float angle = Dot(s, e);

	// sinθ
	float SinTh = sinf(angle);

	// 補間係数
	float Ps = sinf(angle * (1 - t));
	float Pe = sinf(angle * t);

	*out = (s * Ps + e * Pe) / SinTh;

	// 正規化して球面線形補間に
	*out = DirectX::XMVector3Normalize(*out);

	return out;
}

const Conv_XM::Vector3f Conv_XM::Vector3f::Zero(0.0f, 0.0f, 0.0f);
const Conv_XM::Vector3f Conv_XM::Vector3f::WorldForward(0.0f, 0.0f, 1.0f);
const Conv_XM::Vector3f Conv_XM::Vector3f::WorldRight(1.0f, 0.0f, 0.0f);
const Conv_XM::Vector3f Conv_XM::Vector3f::WorldUp(0.0f, 1.0f, 0.0f);
const Conv_XM::Vector3f Conv_XM::Vector3f::UnitX(1.0f, 0.0f, 0.0f);
const Conv_XM::Vector3f Conv_XM::Vector3f::UnitY(0.0f, 1.0f, 0.0f);
const Conv_XM::Vector3f Conv_XM::Vector3f::UnitZ(1.0f, 0.0f, 1.0f);

Conv_XM::Matrix4x4f * Conv_XM::Matrix4x4f::Linear(Matrix4x4f * out, const Matrix4x4f & start, const Matrix4x4f & end, float t)
{
	Conv_XM::Matrix4x4f Start, End;
	Start = DirectX::XMMatrixTranspose(start);
	End = DirectX::XMMatrixTranspose(end);

	Conv_XM::Vector3f startTrans = Conv_XM::Vector3f(Start._41, Start._42, Start._43);
	Conv_XM::Vector3f endTrans = Conv_XM::Vector3f(End._41, End._42, End._43);

	Conv_XM::Vector4f startRot = DirectX::XMQuaternionRotationMatrix(Start);
	Conv_XM::Vector4f endRot = DirectX::XMQuaternionRotationMatrix(End);

	//Conv_XM::Vector3f startRotX = Conv_XM::Vector3f(start._11, start._12, start._13);
	//Conv_XM::Vector3f startRotY = Conv_XM::Vector3f(start._21, start._22, start._23);
	//Conv_XM::Vector3f startRotZ = Conv_XM::Vector3f(start._31, start._32, start._33);

	//Conv_XM::Vector4f startqX, startqY, startqZ, startqRes;

	//startqX = DirectX::XMQuaternionRotationRollPitchYaw(startRotX.x, startRotX.y, startRotX.z);
	//startqY = DirectX::XMQuaternionRotationRollPitchYaw(startRotY.x, startRotY.y, startRotY.z);
	//startqZ = DirectX::XMQuaternionRotationRollPitchYaw(startRotZ.x, startRotZ.y, startRotZ.z);

	//startqRes = DirectX::XMQuaternionMultiply(startqX, startqY);
	//startqRes = DirectX::XMQuaternionMultiply(startqRes, startqZ);


	Conv_XM::Vector3f startScale = Conv_XM::Matrix4x4f::MatrixToScale(Start);
	Conv_XM::Vector3f endScale = Conv_XM::Matrix4x4f::MatrixToScale(End);

	Conv_XM::Vector3f resT, resS;
	Conv_XM::Vector4f resQ;

	Conv_XM::Vector3f::Linear(&resT, startTrans, endTrans, t);
	Conv_XM::Vector3f::Linear(&resS, startScale, endScale, t);
	resQ = DirectX::XMQuaternionSlerp(startRot, endRot, t);

	Conv_XM::Matrix4x4f mtxT, mtxR, mtxS, mtxW;

	mtxT = DirectX::XMMatrixTranslation(resT.x, resT.y, resT.z);
	mtxR = DirectX::XMMatrixRotationQuaternion(resQ);
	mtxS = DirectX::XMMatrixScaling(resS.x, resS.y, resS.z);

	mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;

	mtxW = DirectX::XMMatrixTranspose(mtxW);

	*out = mtxW;

	return out;
}

Conv_XM::Vector3f Conv_XM::Matrix4x4f::MatrixToScale(const Matrix4x4f & mat)
{
	Conv_XM::Vector3f res;
	float x, y, z;

	x = Conv_XM::Vector3f::Length(Conv_XM::Vector3f(mat._11, mat._12, mat._13));
	y = Conv_XM::Vector3f::Length(Conv_XM::Vector3f(mat._21, mat._22, mat._23));
	z = Conv_XM::Vector3f::Length(Conv_XM::Vector3f(mat._31, mat._32, mat._33));

	res = Conv_XM::Vector3f(x, y, z);

	return res;
}

Conv_XM::Matrix4x4f * Conv_XM::Matrix4x4f::CalcInterPause(Matrix4x4f * out, const Matrix4x4f & start, const Matrix4x4f & end, float t)
{

	// 各姿勢ベクトル抽出
	Vector3f Sy, Sz;
	Vector3f Ey, Ez;

	memcpy(&Sy, start.m[1], sizeof(float) * 3);
	memcpy(&Sz, start.m[2], sizeof(float) * 3);
	memcpy(&Ey, end.m[1], sizeof(float) * 3);
	memcpy(&Ez, end.m[2], sizeof(float) * 3);

	// 中間ベクトル算出
	Vector3f IY, IZ;
	Vector3f::SphereLinear(&IY, Sy, Ey, t);
	Vector3f::SphereLinear(&IZ, Sz, Ez, t);

	// 中間ベクトルから姿勢ベクトルを再算出
	Vector3f IX;
	Vector3f::Cross(IY, IZ, &IX);
	Vector3f::Cross(IZ, IX, &IY);
	IX = DirectX::XMVector3Normalize(IX);
	IY = DirectX::XMVector3Normalize(IY);
	IZ = DirectX::XMVector3Normalize(IZ);

	memset(out, 0, sizeof(Matrix4x4f));
	memcpy(out->m[0], &IX, sizeof(float) * 3);
	memcpy(out->m[1], &IY, sizeof(float) * 3);
	memcpy(out->m[2], &IZ, sizeof(float) * 3);
	out->_44 = 1.0f;

	return out;
}

Conv_XM::Vector4f Conv_XM::Vector4f::MakeQuaternionFromVec3(const Conv_XM::Vector3f & forward)
{
	Conv_XM::Vector4f NextVecQuaternion = DirectX::XMQuaternionIdentity();

	//正規化する
	Conv_XM::Vector3f nextVec = DirectX::XMVector3Normalize(forward);

	//0ベクトルかどうかエラーチェック
	float error = Conv_XM::Vector3f::LengthSq(nextVec);
	if (error < Math::k_Epsilon)
	{
		return NextVecQuaternion;
	}

	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, nextVec);
	float angle = acosf(dot);

	// 同じ方角なら
	if (dot > 0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionIdentity();
	}
	else if (dot < -0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis((DirectX::XMVECTOR)Conv_XM::Vector3f::WorldUp, DirectX::XM_PI);
	}
	else
	{	
		//外積を求め回転軸を求める
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, nextVec);

		//クォータニオンを求める
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	return NextVecQuaternion;
}

Conv_XM::Vector4f Conv_XM::Vector4f::MakeQuaternionFromEuler(float x, float y, float z)
{
	Conv_XM::Vector4f quaternion;
	float rad_X, rad_Y, rad_Z;

	rad_X = DirectX::XMConvertToRadians(x);
	rad_Y = DirectX::XMConvertToRadians(y);
	rad_Z = DirectX::XMConvertToRadians(z);

	quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rad_X, rad_Y, rad_Z);

	return quaternion;
}

Conv_XM::Vector4f Conv_XM::Vector4f::MakeQuaternionFromEuler(const Conv_XM::Vector3f & euler)
{
	Conv_XM::Vector4f quaternion;
	float rad_X, rad_Y, rad_Z;

	rad_X = DirectX::XMConvertToRadians(euler.x);
	rad_Y = DirectX::XMConvertToRadians(euler.y);
	rad_Z = DirectX::XMConvertToRadians(euler.z);

	quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rad_X, rad_Y, rad_Z);
	
	return quaternion;
}