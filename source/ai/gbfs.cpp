

#include"../system/main.h"
#include"gbfs.h"
#include"heuristic.h"

bool GBFS(const WeightedGraph & g, const WeightedGraphNode * start, const WeightedGraphNode * goal, GBFSMap & outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// ���݂̃m�[�h�Ɏn�_���Z�b�g���A�N���[�Y�Z�b�g�ɓ����
	const WeightedGraphNode* current = start;

	outMap[current].mInClosedSet = true;

	do
	{
		// �אڃm�[�h���I�[�v���Z�b�g�ɒǉ�����
		for (const WeightedEdge* edge : current->Edges)
		{
			// ���̃m�[�h�̃X�N���b�`�f�[�^���擾
			GBFSScratch& data = outMap[edge->To];
			// �N���[�Y�h�Z�b�g�ɂȂ��Ƃ��Ɍ���ǉ�
			if (!data.mInClosedSet)
			{
				// �אڃm�[�h�̐e�G�b�W��ݒ肷��
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					// �q���[���X�e�B�b�N���v�Z���ăI�[�v���Z�b�g�ɒǉ�����
					data.mHeuristic = ComputeHeuristic(edge->To, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->To);
				}
			}
		}

		// �S�ẴI�[�v���Z�b�g����Ȃ�
		if (openSet.empty())
		{
			break;
		}

		// �ł��R�X�g�̒Ⴂ�m�[�h���I�[�v���Z�b�g����T��
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		// ������J�����g�m�[�h�ɂ��āA�I�[�v���Z�b�g����N���[�Y�Z�b�g�Ɉڂ�
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	// �p�X��������ꂽ���ǂ�����Ԃ�
	return (current == goal) ? true : false;
}

void MakeGBFSRoute(WeightedGraphNode * start, WeightedGraphNode * goal, GBFSMap & map, GBFSRouteMap & outRoute)
{
	// �t���ɓ���
	std::vector<WeightedGraphNode*> m_ReverseRoute;

	// �ŏ��ɃS�[����������
	m_ReverseRoute.emplace_back(goal);

	// ���̃m�[�h
	WeightedGraphNode* nextNode = goal;

	// ���̃m�[�h���X�^�[�g�̃m�[�h�ɂȂ�܂ōs��
	while (nextNode != start)
	{
		// �}�b�v����m�[�h��T��
		auto&& iter = map.find(nextNode);

		// ���[�g�ɓ���
		WeightedGraphNode* node = iter->second.mParentEdge->From;
		m_ReverseRoute.emplace_back(node);

		// ���̃m�[�h�ɐݒ�
		nextNode = node;
	}


	// �t������߂�
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