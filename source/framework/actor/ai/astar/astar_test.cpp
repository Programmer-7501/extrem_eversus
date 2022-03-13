
#include"../../../../system/main.h"
#include "astar_test.h"
#include"../../../component/mesh_component/model_component.h"
#include"../../../../directx/resource/model_manager.h"


/**
* @namespace AStarTestData
* @brief AStarTest情報
*/
namespace AStarTestData
{
	//! モデルファイルネーム
	static const char* k_NodeModelFileName = "asset/model/sphere/sphere.bin";

	//! モデルスケール
	static const Conv_XM::Vector3f k_ModelScale = Conv_XM::Vector3f(0.4f, 0.4f, 0.4f);
}


void AStarTest::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(AStarTestData::k_NodeModelFileName);
}

void AStarTest::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(AStarTestData::k_NodeModelFileName);
}

AStarTest::AStarTest()
{
}

AStarTest::~AStarTest()
{
}

void AStarTest::LoadActor()
{
	// モデルコンポーネントを作成
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void AStarTest::InitActor()
{
	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AStarTest::InitActor nullptr");
		return;
	}

	ModelManager& modelManager = ModelManager::GetInstance();

	Model* model = modelManager.GetModel(AStarTestData::k_NodeModelFileName);
	if (model == nullptr)
	{
		Logger::GetInstance().SetLog("AStarTest::InitActor modelがnullptr");
		return;
	}
	m_ModelComponent->SetModel(model);
	m_ModelComponent->SetOffsetScale(AStarTestData::k_ModelScale);

	bool found = AStar(m_WeightGraph, m_WeightGraph.Nodes[0], m_WeightGraph.Nodes[9], m_AStarMap);

	if (found == true)
	{
		MakeAStarRoute(m_WeightGraph.Nodes[0], m_WeightGraph.Nodes[9], m_AStarMap, m_AStarRouteMap);
	}

	// 次の目的地を設定する
	m_NextNode = m_AStarRouteMap[m_Index];
}

void AStarTest::UpdateActor()
{
	Conv_XM::Vector3f dif = m_NextNode->Position - m_Position;
	float len = Conv_XM::Vector3f::LengthSq(dif);
	if (len < 0.01f)
	{
		m_Index++;
		if (m_Index < m_AStarRouteMap.size())
		{
			m_NextNode = m_AStarRouteMap[m_Index];
		}
		else
		{
			ChangeTarget();
		}
	}
	else
	{
		Conv_XM::Vector3f normalDif = DirectX::XMVector3Normalize(dif);
		Conv_XM::Vector3f position = m_Position + normalDif * 0.1f;
		SetPosition(position);
	}
}

void AStarTest::ChangeTarget()
{
	// 自分のいるノードを探す
	int startNodeIndex = FindAStarNearNode(m_WeightGraph, m_Position);

	// スタートのノード
	WeightedGraphNode* startNode = m_WeightGraph.Nodes[startNodeIndex];

	// ゴールのノード
	WeightedGraphNode* goalNode = nullptr;
	do {
		int goalNodeIndex = rand() % m_WeightGraph.Nodes.size();
		goalNode = m_WeightGraph.Nodes[goalNodeIndex];
	} while (startNode == goalNode);

	// マップを初期化
	m_AStarMap.clear();
	bool found = AStar(m_WeightGraph, startNode, goalNode, m_AStarMap);

	// ルートを初期化
	m_AStarRouteMap.clear();
	if (found == true)
	{
		MakeAStarRoute(startNode, goalNode, m_AStarMap, m_AStarRouteMap);
	}

	// インデックスを初期化
	m_Index = 0;

	// 次の目的地を設定する
	m_NextNode = m_AStarRouteMap[m_Index];
}
