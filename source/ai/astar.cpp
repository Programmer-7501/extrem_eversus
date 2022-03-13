
#include"../system/main.h"
#include"astar.h"
#include"heuristic.h"

bool AStar(WeightedGraph & g, WeightedGraphNode * start, WeightedGraphNode * goal, AStarMap & outMap)
{
	std::vector<WeightedGraphNode*> openSet;

	// ���݂̃m�[�h�Ɏn�_���Z�b�g���A�N���[�Y�Z�b�g�ɓ����
	WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do
	{
		// �אڃm�[�h���I�[�v���Z�b�g�ɒǉ�����
		for (WeightedEdge* edge : current->Edges)
		{
			WeightedGraphNode* neighbor = edge->To;
			// ���̃m�[�h�̃X�N���b�`�f�[�^���擾
			AStarScratch& data = outMap[neighbor];
			// �N���[�Y�h�Z�b�g�ɂȂ��m�[�h�������`�F�b�N
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					// �I�[�v���Z�b�g�ɂȂ���ΐe�̓J�����g
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// ���ۂ̃R�X�g�́A�e�̃R�X�g + �G�b�W�����ǂ�R�X�g
					data.mActualFromStart = outMap[current].mActualFromStart +
						edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					// �J�����g��e�ɂ����Ƃ��̌o�H�R�X�g���v�Z
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart)
					{
						// ���̃m�[�h��e���J�����g�ɂ���
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		// �I�[�v���Z�b�g����Ȃ�
		if (openSet.empty())
		{
			break;
		}

		// �ł��R�X�g�̒Ⴂ�m�[�h���I�[�v���Z�b�g����T��
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](WeightedGraphNode* a, WeightedGraphNode* b) {
			float fOfA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
			float fOfB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
			return fOfA < fOfB;
		});
		// ������J�����g�m�[�h�ɃZ�b�g���āA�I�[�v���Z�b�g����N���[�Y�h�Z�b�g�Ɉڂ�
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	// �o�H�����������ǂ���
	return (current == goal) ? true : false;
}

void MakeAStarRoute(WeightedGraphNode * start, WeightedGraphNode * goal, AStarMap & map, AStarRouteMap & outRoute)
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
