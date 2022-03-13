
#include"../system/main.h"
#include"astar.h"
#include"heuristic.h"

bool AStar(WeightedGraph & g, WeightedGraphNode * start, WeightedGraphNode * goal, AStarMap & outMap)
{
	std::vector<WeightedGraphNode*> openSet;

	// 現在のノードに始点をセットし、クローズセットに入れる
	WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do
	{
		// 隣接ノードをオープンセットに追加する
		for (WeightedEdge* edge : current->Edges)
		{
			WeightedGraphNode* neighbor = edge->To;
			// このノードのスクラッチデータを取得
			AStarScratch& data = outMap[neighbor];
			// クローズドセットにないノードだけをチェック
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					// オープンセットになければ親はカレント
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// 実際のコストは、親のコスト + エッジをたどるコスト
					data.mActualFromStart = outMap[current].mActualFromStart +
						edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					// カレントを親にしたときの経路コストを計算
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart)
					{
						// このノードを親をカレントにする
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		// オープンセットが空なら
		if (openSet.empty())
		{
			break;
		}

		// 最もコストの低いノードをオープンセットから探す
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](WeightedGraphNode* a, WeightedGraphNode* b) {
			float fOfA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
			float fOfB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
			return fOfA < fOfB;
		});
		// それをカレントノードにセットして、オープンセットからクローズドセットに移す
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	// 経路を見つけたかどうか
	return (current == goal) ? true : false;
}

void MakeAStarRoute(WeightedGraphNode * start, WeightedGraphNode * goal, AStarMap & map, AStarRouteMap & outRoute)
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

int FindAStarNearNode(const WeightedGraph & graph, const Conv_XM::Vector3f position)
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
