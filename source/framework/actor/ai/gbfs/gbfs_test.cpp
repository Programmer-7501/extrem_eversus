
#include"../../../../system/main.h"
#include "gbfs_test.h"
#include"../../../component/mesh_component/model_component.h"
#include"../../../../directx/resource/model_manager.h"

/**
* @namespace GBFSTestData
* @brief GBFSTest���
*/
namespace GBFSTestData
{
	//! ���f���t�@�C���l�[��
	static const char* k_NodeModelFileName = "asset/model/sphere/sphere.bin";

	//! ���f���X�P�[��
	static const Conv_XM::Vector3f k_ModelScale = Conv_XM::Vector3f(0.4f, 0.4f, 0.4f);
}

void GBFSTest::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(GBFSTestData::k_NodeModelFileName);
}

void GBFSTest::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(GBFSTestData::k_NodeModelFileName);
}

GBFSTest::GBFSTest()
{
}

GBFSTest::~GBFSTest()
{
}

void GBFSTest::LoadActor()
{
	// ���f���R���|�[�l���g���쐬
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void GBFSTest::InitActor()
{
	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GBFSTest::InitActor nullptr");
		return;
	}

	ModelManager& modelManager = ModelManager::GetInstance();

	Model* model = modelManager.GetModel(GBFSTestData::k_NodeModelFileName);
	if (model == nullptr)
	{
		Logger::GetInstance().SetLog("GBFSTest::InitActor model��nullptr");
		return;
	}
	m_ModelComponent->SetModel(model);
	m_ModelComponent->SetOffsetScale(GBFSTestData::k_ModelScale);

	bool found = GBFS(m_WeightGraph, m_WeightGraph.Nodes[0], m_WeightGraph.Nodes[9], m_GBFSMap);

	if (found == true)
	{
		MakeGBFSRoute(m_WeightGraph.Nodes[0], m_WeightGraph.Nodes[9], m_GBFSMap, m_GBFSRouteMap);
	}

	// ���̖ړI�n��ݒ肷��
	m_NextNode = m_GBFSRouteMap[m_Index];
}

void GBFSTest::UpdateActor()
{
	Conv_XM::Vector3f dif = m_NextNode->Position - m_Position;
	float len = Conv_XM::Vector3f::LengthSq(dif);
	if (len < 0.01f)
	{
		m_Index++;
		if (m_Index < m_GBFSRouteMap.size())
		{
			m_NextNode = m_GBFSRouteMap[m_Index];
		}
		else
		{
			ChangeTarget();
		}
	}
	else
	{
		Conv_XM::Vector3f normalDif = DirectX::XMVector3Normalize(dif);
		Conv_XM::Vector3f position = m_Position + normalDif * 0.05f;
		SetPosition(position);
	}
}

void GBFSTest::ChangeTarget()
{
	// �����̂���m�[�h��T��
	int startNodeIndex = FindGBFSNearNode(m_WeightGraph, m_Position);

	// �X�^�[�g�̃m�[�h
	WeightedGraphNode* startNode = m_WeightGraph.Nodes[startNodeIndex];

	// �S�[���̃m�[�h
	WeightedGraphNode* goalNode = nullptr;
	do {
		int goalNodeIndex = rand() % m_WeightGraph.Nodes.size();
		goalNode = m_WeightGraph.Nodes[goalNodeIndex];
	} while (startNode == goalNode);

	// �}�b�v��������
	m_GBFSMap.clear();
	bool found = GBFS(m_WeightGraph, startNode, goalNode, m_GBFSMap);

	// ���[�g��������
	m_GBFSRouteMap.clear();
	if (found == true)
	{
		MakeGBFSRoute(startNode, goalNode, m_GBFSMap, m_GBFSRouteMap);
	}

	// �C���f�b�N�X��������
	m_Index = 0;

	// ���̖ړI�n��ݒ肷��
	m_NextNode = m_GBFSRouteMap[m_Index];
}
