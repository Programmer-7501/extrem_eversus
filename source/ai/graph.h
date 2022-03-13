#pragma once


struct GraphNode
{
	// 各ノードは隣接ノードへのポインタを持つ
	std::vector<GraphNode*> Adjacent;

	//! ノードのポジション
	Conv_XM::Vector3f Position;

	//! 経路になっているかどうか
	bool IsRoute;
};

struct Graph
{
	// グラフはノードを含む
	std::vector<GraphNode*> Nodes;
};
