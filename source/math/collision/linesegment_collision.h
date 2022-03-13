/**
 * @file linesegment_collision.h
 * @brief 線分コリジョン
 */

#pragma once


 /**
 * @namespace Collision
 * @brief コリジョン集約
 */
namespace Collision
{
	/**
	* @brief 線分のコリジョン
	*/
	struct LineSegment
	{
		Conv_XM::Vector3f Start;
		Conv_XM::Vector3f End;
	};

	/**
	* @brief 点と線分との最短距離の2乗
	* @param[in] const LineSegment&(lineSegment) 対象の線分
	* @param[in] const Conv_XM::Vector3f&(point) 対象の点
	* @return float 点と線分との最短距離の2乗
	*/
	float PointToLineSegmentDistSq(const LineSegment& lineSegment, const Conv_XM::Vector3f& point);
}