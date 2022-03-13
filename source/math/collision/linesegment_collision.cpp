
#include"../../system/main.h"
#include"linesegment_collision.h"

float Collision::PointToLineSegmentDistSq(const LineSegment & lineSegment, const Conv_XM::Vector3f & point)
{
	float res;
	Conv_XM::Vector3f ab = lineSegment.End - lineSegment.Start;
	Conv_XM::Vector3f ba = -ab;
	Conv_XM::Vector3f ac = point - lineSegment.Start;
	Conv_XM::Vector3f bc = point - lineSegment.End;

	// �|�C���g���X�^�[�g�n�_�̑O�ɓ˂��o�Ă���
	if (Conv_XM::Vector3f::Dot(ab, ac) < 0.0f)
	{
		res = Conv_XM::Vector3f::LengthSq(ac);
		return res;
	}
	else if (Conv_XM::Vector3f::Dot(ba, bc) < 0.0f)	// �|�C���g���G���h�n�_�̌�ɓ˂��o�Ă���
	{
		res = Conv_XM::Vector3f::LengthSq(bc);
		return res;
	}
	else// �|�C���g������Ɏˉe����
	{
		float scalar = Conv_XM::Vector3f::Dot(ac, ab) / Conv_XM::Vector3f::Dot(ab, ab);
		Conv_XM::Vector3f p = scalar * ab;
		res = Conv_XM::Vector3f::LengthSq((ac - p));
		return res;
	}
}
