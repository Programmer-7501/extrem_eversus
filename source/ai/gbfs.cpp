

#include"../system/main.h"
#include"gbfs.h"
#include"heuristic.h"

bool GBFS(const WeightedGraph & g, const WeightedGraphNode * start, const WeightedGraphNode * goal, GBFSMap & outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// 現在のノードに始点をセットし、クローズセットに入れる
	const WeightedGraphNode* current = start;

	outMap[current].mInClosedSet = true;

	do
	{
		// 隣接ノードをオープンセットに追加する
		for (const WeightedEdge* edge : current->Edges)
		{
			// このノードのスクラッチデータを取得
			GBFSScratch& data = outMap[edge->To];
			// クローズドセットにないときに限り追加
			if (!data.mInClosedSet)
			{
				// 隣接ノードの親エッジを設定する
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					// ヒューリスティックを計算してオープンセットに追加する
					data.mHeuristic = ComputeHeuristic(edge->To, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->To);
				}
			}
		}

		// 全てのオープンセットが空なら
		if (openSet.empty())
		{
			break;
		}

		// 最もコストの低いノードをオープンセットから探す
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		// それをカレントノードにして、オープンセットからクローズセットに移す
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	// パスを見つけられたかどうかを返す
	return (current == goal) ? true : false;
}

void MakeGBFSRoute(WeightedGraphNode * start, WeightedGraphNode * goal, GBFSMap & map, GBFSRouteMap & outRoute)
{
	// 逆順に入る
	std::vector<WeightedGraphNode*> m_ReverseRoute;

	// 最初にゴールから入れる
	m_ReverseRoute.emplace_back(goal);

	// 次のノード
	WeightedGraphNode* nextNode = goal;

	// 次のノードがスタートのノードになるまで行う
	while (nextNode != start)
	{
		// マップからノードを探す
		auto&& iter = map.find(nextNode);

		// ルートに入力
		WeightedGraphNode* node = iter->second.mParentEdge->From;
		m_ReverseRoute.emplace_back(node);

		// 次のノードに設定
		nextNode = node;
	}


	// 逆順から戻す
	for (int i = 0; i < m_ReverseRoute.size(); i++)
	{
		WeightedGraphNode* node = m_ReverseRoute[m_ReverseRoute.size() - 1 - i];
		outRoute.emplace_back(node);
	}
}

int FindGBFSNearNode(const WeightedGraph & graph, const Conv_XM::Vector3f position)
{
	float min = -1.0f;
	int resIndex = 0;
	for (int i = 0; i < graph.Nodes.size(); i++)
	{
		if (i == 0)
		{
			min = Conv_XM::Vector3f::LengthSq(graph.Nodes[i]->Position - position);
		}
		else
		{
			float lensq = Conv_XM::Vector3f::LengthSq(graph.Nodes[i]->Position - position);

			if (lensq < min)
			{
				resIndex = i;
				min = lensq;
			}
		}
	}

	return resIndex;
}

void testHeuristic()
{
	WeightedGraph g;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			WeightedGraphNode* node = new WeightedGraphNode;
			g.Nodes.emplace_back(node);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			WeightedGraphNode* node = g.Nodes[i * 5 + j];
			if (i > 0)
			{
				WeightedEdge* e = new WeightedEdge;
				e->From = node;
				e->To = g.Nodes[(i - 1) * 5 + j];
				e->mWeight = 1.0f;
				node->Edges.emplace_back(e);
			}
			if (i < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->From = node;
				e->To = g.Nodes[(i + 1) * 5 + j];
				e->mWeight = 1.0f;
				node->Edges.emplace_back(e);
			}
			if (j > 0)
			{
				WeightedEdge* e = new WeightedEdge;
				e->From = node;
				e->To = g.Nodes[i * 5 + j - 1];
				e->mWeight = 1.0f;
				node->Edges.emplace_back(e);
			}
			if (j < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->From = node;
				e->To = g.Nodes[i * 5 + j + 1];
				e->mWeight = 1.0f;
				node->Edges.emplace_back(e);
			}
		}
	}
	bool found = false;

	GBFSMap map;
	found = GBFS(g, g.Nodes[0], g.Nodes[9], map);


}