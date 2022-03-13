
#include"../../../system/main.h"
#include "ai_test.h"
#include"../../component/mesh_component/model_component.h"
#include"../../../directx/resource/model_manager.h"

/**
* @namespace AITestData
* @brief AITest���
*/
namespace AITestData
{
	//! ���f���t�@�C���l�[��
	static const char* k_NodeModelFileName = "asset/model/sphere/sphere.bin";

	//! ���f���X�P�[��
	static const Conv_XM::Vector3f k_ModelScale = Conv_XM::Vector3f(0.4f, 0.4f, 0.4f);
}

void AITest::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(AITestData::k_NodeModelFileName);
}

void AITest::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(AITestData::k_NodeModelFileName);
}

AITest::AITest()
	: m_ModelComponent(nullptr)
	, m_NextNode(nullptr)
	, m_Index(0)
{
}

AITest::~AITest()
{
}

void AITest::LoadActor()
{
	// ���f���R���|�[�l���g���쐬
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void AITest::InitActor()
{
	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AITest::InitActor nullptr");
		return;
	}

	ModelManager& modelManager = ModelManager::GetInstance();

	Model* model = modelManager.GetModel(AITestData::k_NodeModelFileName);
	if (model == nullptr)
	{
		Logger::GetInstance().SetLog("AITest::InitActor model��nullptr");
		return;
	}
	m_ModelComponent->SetModel(model);
	m_ModelComponent->SetOffsetScale(AITestData::k_ModelScale);

	bool found = BFS(m_Graph, m_Graph.Nodes[0], m_Graph.Nodes[9], m_NodeToParentMap);

	if (found == true)
	{
		MakeBFSRoute(m_Graph.Nodes[0], m_Graph.Nodes[9], m_NodeToParentMap, m_RouteMap);
	}

	// ���̖ړI�n��ݒ肷��
	m_NextNode = m_RouteMap[m_Index];
}

void AITest::UpdateActor()
{
	Conv_XM::Vector3f dif = m_NextNode->Position - m_Position;
	float len = Conv_XM::Vector3f::LengthSq(dif);
	if (len < 0.01f)
	{
		m_Index++;
		if (m_Index < m_RouteMap.size())
		{
			m_NextNode = m_RouteMap[m_Index];
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

void AITest::ChangeTarget()
{
	// �����̂���m�[�h��T��
	int startNodeIndex = FindBFSNearNode(m_Graph, m_Position);

	// �X�^�[�g�̃m�[�h
	GraphNode* startNode = m_Graph.Nodes[startNodeIndex];

	// �S�[���̃m�[�h
	GraphNode* goalNode = nullptr;
	do{
		int goalNodeIndex = rand() % m_Graph.Nodes.size();
		goalNode = m_Graph.Nodes[goalNodeIndex];
	} while (startNode == goalNode);

	// �}�b�v��������
	m_NodeToParentMap.clear();
	bool found = BFS(m_Graph, startNode, goalNode, m_NodeToParentMap);

	// ���[�g��������
	m_RouteMap.clear();
	if (found == true)
	{
		MakeBFSRoute(startNode, goalNode, m_NodeToParentMap, m_RouteMap);
	}

	// �C���f�b�N�X��������
	m_Index = 0;

	// ���̖ړI�n��ݒ肷��
	m_NextNode = m_RouteMap[m_Index];
}
