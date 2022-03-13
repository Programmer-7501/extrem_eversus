#pragma once


struct GraphNode
{
	// �e�m�[�h�͗אڃm�[�h�ւ̃|�C���^������
	std::vector<GraphNode*> Adjacent;

	//! �m�[�h�̃|�W�V����
	Conv_XM::Vector3f Position;

	//! �o�H�ɂȂ��Ă��邩�ǂ���
	bool IsRoute;
};

struct Graph
{
	// �O���t�̓m�[�h���܂�
	std::vector<GraphNode*> Nodes;
};
