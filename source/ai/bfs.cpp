
#include"../system/main.h"
#include"bfs.h"

bool BFS(const Graph & graph, GraphNode * start, GraphNode * goal, NodeToParentMap & outMap)
{
	// 経路を見つけたかどうか
	bool pathFound = false;
	// 検討するノード
	std::queue<GraphNode*> q;
	// 最初のノードをキューに入れる
	q.emplace(start);

	while (!q.empty())
	{
		// ノードを一つキューから出す
		GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// まだキューに入っていない隣接ノードをエンキューする
		for (GraphNode* node : current->Adjacent)
		{
			// もし親がnullptrなら、まだキューに追加されていない
			// (開始ノードは例外)
			GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start)
			{
				// このノードをエンキューと親の設定をする
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
}

void MakeBFSRoute(GraphNode * start, GraphNode * goal, NodeToParentMap & map, BFSRouteMap & outRoute)
{
	// 逆順に入る
	std::vector<GraphNode*> m_ReverseRoute;

	// 最初にゴールから入れる
	m_ReverseRoute.emplace_back(goal);

	// 次のノード
	GraphNode* nextNode = goal;

	// 次のノードがスタートのノードになるまで行う
	while (nextNode != start)
	{
		// マップからノードを探す
		auto&& iter = map.find(nextNode);

		// ルートに入力
		GraphNode* node = iter->second;
		node->IsRoute = true;
		m_ReverseRoute.emplace_back(node);

		// 次のノードに設定
		nextNode = node;
	}

	// 逆順から戻す
	for (int i = 0; i < m_ReverseRoute.size(); i++)
	{
		GraphNode* node = m_ReverseRoute[m_ReverseRoute.size() - 1 - i];
		outRoute.emplace_back(node);
	}
}

int FindBFSNearNode(const Graph& graph, const Conv_XM::Vector3f position)
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