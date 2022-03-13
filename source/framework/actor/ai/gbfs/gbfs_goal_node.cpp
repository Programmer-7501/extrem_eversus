
#include"../../../../system/main.h"
#include "gbfs_goal_node.h"
#include"../../../../directx/resource/model_manager.h"
#include"../../../component/mesh_component/instancing_model_component.h"
#include"../../../component/mesh_component/model_component.h"
#include"gbfs_test.h"

/**
* @namespace GBFSNodeData
* @brief GBFSNode情報
*/
namespace GBFSGoalNodeData
{
	//! ゴールノードファイルネーム
	static const char* k_GoalNodeModelFileName = "asset/model/ai/goalnode/goalnode.bin";

	//! ゴールノードモデルスケール
	static const Conv_XM::Vector3f k_GoalNodeScale = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
}


void GBFSGoalNode::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(GBFSGoalNodeData::k_GoalNodeModelFileName);
}

void GBFSGoalNode::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(GBFSGoalNodeData::k_GoalNodeModelFileName);
}

GBFSGoalNode::GBFSGoalNode()
{
}

GBFSGoalNode::~GBFSGoalNode()
{
}

void GBFSGoalNode::LoadActor()
{
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void GBFSGoalNode::InitActor()
{
	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GBFSGoalNode::InitActor nullptr");
		return;
	}

	ModelManager& modelManager = ModelManager::GetInstance();

	Model* model = modelManager.GetModel(GBFSGoalNodeData::k_GoalNodeModelFileName);

	if (model == nullptr)
	{
		Logger::GetInstance().SetLog("GBFSGoalNode::InitActor modelがnullptr");
		return;
	}

	m_ModelComponent->SetModel(model);
	m_ModelComponent->SetOffsetScale(GBFSGoalNodeData::k_GoalNodeScale);
}

void GBFSGoalNode::UpdateActor()
{
	if (m_GBFSTest == nullptr)
	{
		Logger::GetInstance().SetLog("GBFSGoalNode::UpdateActor m_GBFSTestがnullptr");
		return;
	}
	GBFSRouteMap routeMap = m_GBFSTest->GetRouteMap();
	WeightedGraphNode* goalNode = routeMap[routeMap.size() - 1];

	SetPosition(goalNode->Position);
}
