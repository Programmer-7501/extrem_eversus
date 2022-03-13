
#include"../../../system/main.h"
#include "weighted_graph_node_loder.h"
#include"../../component/mesh_component/instancing_color_model_component.h"
#include"../../component/mesh_component/model_component.h"
#include"../../component/mesh_component/debug/debug_number_billboard_component.h"
#include"../../../directx/resource/model_manager.h"

void WeightedGraphLoader::LoadModel()
{
}

void WeightedGraphLoader::UnloadModel()
{
}

WeightedGraphLoader::WeightedGraphLoader()
{
}

WeightedGraphLoader::~WeightedGraphLoader()
{
}

void WeightedGraphLoader::LoadActor()
{
}

void WeightedGraphLoader::InitActor()
{
}

void WeightedGraphLoader::UninitActor()
{
	for (auto* node : m_WeightedGraph.Nodes)
	{
		for (auto* edge : node->Edges)
		{
			if (edge)
			{
				delete edge;
				edge = nullptr;
			}
		}

		if (node)
		{
			delete node;
			node = nullptr;
		}
	}

	m_StoreEdgeData.clear();
}

void WeightedGraphLoader::UpdateActor()
{
}

void WeightedGraphLoader::Load(const char * fileName)
{
	//�t�@�C��������o�C�i���t�@�C���ŏ����o��
	FILE *fp = fopen(fileName, "rb");
	if (fp == nullptr)
	{
		Logger::GetInstance().SetLog("WeightedGraphMaker::Output �t�@�C���|�C���^��nullptr");
		return;
	}

	//�O���t�m�[�h�̐���ǂݍ���
	int nodeNum = 0;
	fread(&nodeNum, sizeof(nodeNum), 1, fp);

	// �m�[�h���쐬
	Conv_XM::Vector3f pos;
	for (int i = 0; i < nodeNum; i++)
	{
		fread(&pos, sizeof(Conv_XM::Vector3f), 1, fp);
		WeightedGraphNode* weightedGraph = new WeightedGraphNode;
		weightedGraph->Position = pos;
		m_WeightedGraph.Nodes.emplace_back(weightedGraph);
	}


	//�G�b�W�f�[�^�̐���ǂݍ���
	int edgeNum = 0;
	fread(&edgeNum, sizeof(edgeNum), 1, fp);

	// �G�b�W�f�[�^�o��
	StoreEdgeData edge = {};
	for (int i = 0; i < edgeNum; i++)
	{
		fread(&edge, sizeof(StoreEdgeData), 1, fp);

		MakeBothDirectionsWeightedEdge(m_WeightedGraph.Nodes[edge.Index001], m_WeightedGraph.Nodes[edge.Index002], edge.Weight);
		m_StoreEdgeData.emplace_back(edge);
	}

	fclose(fp);

	// �C���X�^���V���O���f���̍X�V
	//UpdateInstancingModel();
}

void WeightedGraphLoader::MakeBothDirectionsWeightedEdge(WeightedGraphNode * target001, WeightedGraphNode * target002, float weight)
{
	WeightedEdge* edge001 = new WeightedEdge;
	edge001->From = target001;
	edge001->To = target002;
	edge001->mWeight = weight;
	target001->Edges.emplace_back(edge001);

	WeightedEdge* edge002 = new WeightedEdge;
	edge002->From = target002;
	edge002->To = target001;
	edge002->mWeight = weight;
	target002->Edges.emplace_back(edge002);
}
