/**
 * @file linesegment_collision.h
 * @brief �����R���W����
 */

#pragma once


 /**
 * @namespace Collision
 * @brief �R���W�����W��
 */
namespace Collision
{
	/**
	* @brief �����̃R���W����
	*/
	struct LineSegment
	{
		Conv_XM::Vector3f Start;
		Conv_XM::Vector3f End;
	};

	/**
	* @brief �_�Ɛ����Ƃ̍ŒZ������2��
	* @param[in] const LineSegment&(lineSegment) �Ώۂ̐���
	* @param[in] const Conv_XM::Vector3f&(point) �Ώۂ̓_
	* @return float �_�Ɛ����Ƃ̍ŒZ������2��
	*/
	float PointToLineSegmentDistSq(const LineSegment& lineSegment, const Conv_XM::Vector3f& point);
}