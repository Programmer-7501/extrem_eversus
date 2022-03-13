#pragma once

struct WeightedEdge
{
	// �G�b�W�ɂȂ����Ă���m�[�h
	struct WeightedGraphNode* From;
	struct WeightedGraphNode* To;
	// �G�b�W�̏d��
	float mWeight;
};

struct WeightedGraphNode
{
	// ���̃m�[�h����o�Ă����G�b�W���i�[
	std::vector<WeightedEdge*> Edges;

	//! �m�[�h�̃|�W�V����
	Conv_XM::Vector3f Position;
};

struct WeightedGraph
{
	// �O���t�̓m�[�h���܂�
	std::vector<WeightedGraphNode*> Nodes;
};