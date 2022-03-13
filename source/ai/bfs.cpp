
#include"../system/main.h"
#include"bfs.h"

bool BFS(const Graph & graph, GraphNode * start, GraphNode * goal, NodeToParentMap & outMap)
{
	// �o�H�����������ǂ���
	bool pathFound = false;
	// ��������m�[�h
	std::queue<GraphNode*> q;
	// �ŏ��̃m�[�h���L���[�ɓ����
	q.emplace(start);

	while (!q.empty())
	{
		// �m�[�h����L���[����o��
		GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// �܂��L���[�ɓ����Ă��Ȃ��אڃm�[�h���G���L���[����
		for (GraphNode* node : current->Adjacent)
		{
			// �����e��nullptr�Ȃ�A�܂��L���[�ɒǉ�����Ă��Ȃ�
			// (�J�n�m�[�h�͗�O)
			GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start)
			{
				// ���̃m�[�h���G���L���[�Ɛe�̐ݒ������
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
}

void MakeBFSRoute(GraphNode * start, GraphNode * goal, NodeToParentMap & map, BFSRouteMap & outRoute)
{
	// �t���ɓ���
	std::vector<GraphNode*> m_ReverseRoute;

	// �ŏ��ɃS�[����������
	m_ReverseRoute.emplace_back(goal);

	// ���̃m�[�h
	GraphNode* nextNode = goal;

	// ���̃m�[�h���X�^�[�g�̃m�[�h�ɂȂ�܂ōs��
	while (nextNode != start)
	{
		// �}�b�v����m�[�h��T��
		auto&& iter = map.find(nextNode);

		// ���[�g�ɓ���
		GraphNode* node = iter->second;
		node->IsRoute = true;
		m_ReverseRoute.emplace_back(node);

		// ���̃m�[�h�ɐݒ�
		nextNode = node;
	}

	// �t������߂�
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