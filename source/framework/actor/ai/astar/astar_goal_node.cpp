
#include"../../../../system/main.h"
#include "astar_goal_node.h"
#include"../../../../directx/resource/model_manager.h"
#include"../../../component/mesh_component/instancing_model_component.h"
#include"../../../component/mesh_component/model_component.h"
#include"astar_test.h"


/**
* @namespace AStarNodeData
* @brief AStarNode情報
*/
namespace AStarGoalNodeData
{
	//! ゴールノードファイルネーム
	static const char* k_GoalNodeModelFileName = "asset/model/ai/goalnode/goalnode.bin";

	//! ゴールノードモデルスケール
	static const Conv_XM::Vector3f k_GoalNodeScale = Conv_XM::Vector3f(1.0f, 1.0f, 1.0f);
}


void AStarGoalNode::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(AStarGoalNodeData::k_GoalNodeModelFileName);
}

void AStarGoalNode::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(AStarGoalNodeData::k_GoalNodeModelFileName);
}

AStarGoalNode::AStarGoalNode()
{
}

AStarGoalNode::~AStarGoalNode()
{
}

void AStarGoalNode::LoadActor()
{
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void AStarGoalNode::InitActor()
{
	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AStarGoalNode::InitActor nullptr");
		return;
	}

	ModelManager& modelManager = ModelManager::GetInstance();

	Model* model = modelManager.GetModel(AStarGoalNodeData::k_GoalNodeModelFileName);

	if (model == nullptr)
	{
		Logger::GetInstance().SetLog("AStarGoalNode::InitActor modelがnullptr");
		return;
	}

	m_ModelComponent->SetModel(model);
	m_ModelComponent->SetOffsetScale(AStarGoalNodeData::k_GoalNodeScale);
}

void AStarGoalNode::UpdateActor()
{
	if (m_AStarTest == nullptr)
	{
		Logger::GetInstance().SetLog("AStarGoalNode::UpdateActor m_GBFSTestがnullptr");
		return;
	}
	AStarRouteMap routeMap = m_AStarTest->GetRouteMap();
	WeightedGraphNode* goalNode = routeMap[routeMap.size() - 1];

	SetPosition(goalNode->Position);
}
