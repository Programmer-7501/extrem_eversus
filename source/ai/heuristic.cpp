

#include"../system/main.h"
#include"heuristic.h"

float ComputeHeuristic(const WeightedGraphNode * a, const WeightedGraphNode * b)
{
	float len = Conv_XM::Vector3f::Length(a->Position - b->Position);
	return len;
}