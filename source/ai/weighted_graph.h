#pragma once

struct WeightedEdge
{
	// エッジにつながっているノード
	struct WeightedGraphNode* From;
	struct WeightedGraphNode* To;
	// エッジの重み
	float mWeight;
};

struct WeightedGraphNode
{
	// このノードから出ていくエッジを格納
	std::vector<WeightedEdge*> Edges;

	//! ノードのポジション
	Conv_XM::Vector3f Position;
};

struct WeightedGraph
{
	// グラフはノードを含む
	std::vector<WeightedGraphNode*> Nodes;
};