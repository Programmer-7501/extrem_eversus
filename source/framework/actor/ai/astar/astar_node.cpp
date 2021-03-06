
#include"../../../../system/main.h"
#include "astar_node.h"
#include"../../../../directx/resource/model_manager.h"
#include"../../../component/mesh_component/instancing_model_component.h"
#include"../../../component/mesh_component/instancing_color_model_component.h"
#include"../../../component/mesh_component/model_component.h"


/**
* @namespace AStarNodeData
* @brief AStarNode情報
*/
namespace AStarNodeData
{
	//! ノードモデルファイルネーム
	static const char* k_NodeModelFileName = "asset/model/ai/node/node.bin";

	//! ノードモデルスケール
	static const Conv_XM::Vector3f k_NodeScale = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
}

void AStarNode::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(AStarNodeData::k_NodeModelFileName);
}

void AStarNode::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(AStarNodeData::k_NodeModelFileName);
}

AStarNode::AStarNode()
{
}

AStarNode::~AStarNode()
{
}

void AStarNode::LoadActor()
{
	// ノードオブジェクトインスタンシングモデルコンポーネント作成
	m_NodeInstancingModelComponent = CreateComponent<InstancingModelComponent>();
}

void AStarNode::InitActor()
{
	if (m_NodeInstancingModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AINode::InitActor nullptr");
		return;
	}

	// インスタンシングデータ入力
	Conv_XM::Matrix4x4f mtxS, mtxT, mtxW;
	std::vector<Conv_XM::Matrix4x4f> nodeMTX;
	mtxS = DirectX::XMMatrixScaling(AStarNodeData::k_NodeScale.x, AStarNodeData::k_NodeScale.y, AStarNodeData::k_NodeScale.z);

	for (int i = 0; i < m_WeightGraph.Nodes.size(); i++)
	{
		mtxT = DirectX::XMMatrixTranslation(m_WeightGraph.Nodes[i]->Position.x, m_WeightGraph.Nodes[i]->Position.y, m_WeightGraph.Nodes[i]->Position.z);
		mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxT;
		mtxW = DirectX::XMMatrixTranspose(mtxW);
		nodeMTX.emplace_back(mtxW);
	}

	CBufferData::Material material;
	material.Metaric = 0.5f;
	material.Smoothness = 0.5f;

	m_NodeInstancingModelComponent->SetInstanceMatrixData(nodeMTX);
	m_NodeInstancingModelComponent->SetMaterial(material);

	ModelManager& modelManager = ModelManager::GetInstance();
	Model* nodeModel = modelManager.GetModel(AStarNodeData::k_NodeModelFileName);
	if (nodeModel == nullptr)
	{
		Logger::GetInstance().SetLog("AStarNode::InitActor nodeModelがnullptr");
		return;
	}
	m_NodeInstancingModelComponent->SetModel(nodeModel);

}

void AStarNode::UpdateActor()
{
}
