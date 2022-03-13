
#include"../../../../system/main.h"
#include "gbfs_test.h"
#include"../../../component/mesh_component/model_component.h"
#include"../../../../directx/resource/model_manager.h"

/**
* @namespace GBFSTestData
* @brief GBFSTest情報
*/
namespace GBFSTestData
{
	//! モデルファイルネーム
	static const char* k_NodeModelFileName = "asset/model/sphere/sphere.bin";

	//! モデルスケール
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
	// モデルコンポーネントを作成
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
		Logger::GetInstance().SetLog("GBFSTest::InitActor modelがnullptr");
		return;
	}
	m_ModelComponent->SetModel(model);
	m_ModelComponent->SetOffsetScale(GBFSTestData::k_ModelScale);

	bool found = GBFS(m_WeightGraph, m_WeightGraph.Nodes[0], m_WeightGraph.Nodes[9], m_GBFSMap);

	if (found == true)
	{
		MakeGBFSRoute(m_WeightGraph.Nodes[0], m_WeightGraph.Nodes[9], m_GBFSMap, m_GBFSRouteMap);
	}

	// 次の目的地を設定する
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
	// 自分のいるノードを探す
	int startNodeIndex = FindGBFSNearNode(m_WeightGraph, m_Position);

	// スタートのノード
	WeightedGraphNode* startNode = m_WeightGraph.Nodes[startNodeIndex];

	// ゴールのノード
	WeightedGraphNode* goalNode = nullptr;
	do {
		int goalNodeIndex = rand() % m_WeightGraph.Nodes.size();
		goalNode = m_WeightGraph.Nodes[goalNodeIndex];
	} while (startNode == goalNode);

	// マップを初期化
	m_GBFSMap.clear();
	bool found = GBFS(m_WeightGraph, startNode, goalNode, m_GBFSMap);

	// ルートを初期化
	m_GBFSRouteMap.clear();
	if (found == true)
	{
		MakeGBFSRoute(startNode, goalNode, m_GBFSMap, m_GBFSRouteMap);
	}

	// インデックスを初期化
	m_Index = 0;

	// 次の目的地を設定する
	m_NextNode = m_GBFSRouteMap[m_Index];
}
