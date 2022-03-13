
#include"../../../system/main.h"
#include "ai_goal_node.h"
#include"../../../directx/resource/model_manager.h"
#include"../../component/mesh_component/model_component.h"
#include"ai_test.h"

/**
* @namespace AINodeData
* @brief AINode情報
*/
namespace AIGoalNodeData
{
	//! ゴールノードファイルネーム
	static const char* k_GoalNodeModelFileName = "asset/model/ai/goalnode/goalnode.bin";

	//! ゴールノードモデルスケール
	static const Conv_XM::Vector3f k_GoalNodeScale = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
}

void AIGoalNode::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(AIGoalNodeData::k_GoalNodeModelFileName);
}

void AIGoalNode::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(AIGoalNodeData::k_GoalNodeModelFileName);
}

AIGoalNode::AIGoalNode()
	:m_ModelComponent(nullptr)
{
}

AIGoalNode::~AIGoalNode()
{
}

void AIGoalNode::LoadActor()
{
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void AIGoalNode::InitActor()
{
	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AIGoalNode::InitActor nullptr");
		return;
	}

	ModelManager& modelManager = ModelManager::GetInstance();

	Model* model = modelManager.GetModel(AIGoalNodeData::k_GoalNodeModelFileName);

	if (model == nullptr)
	{
		Logger::GetInstance().SetLog("AIGoalNode::InitActor modelがnullptr");
		return;
	}

	m_ModelComponent->SetModel(model);
	m_ModelComponent->SetOffsetScale(AIGoalNodeData::k_GoalNodeScale);
}

void AIGoalNode::UpdateActor()
{
	if (m_AITest == nullptr)
	{
		Logger::GetInstance().SetLog("AIGoalNode::UpdateActor modelがnullptr");
		return;
	}
	BFSRouteMap routeMap = m_AITest->GetRouteMap();
	GraphNode* goalNode = routeMap[routeMap.size() - 1];

	SetPosition(goalNode->Position);
}
