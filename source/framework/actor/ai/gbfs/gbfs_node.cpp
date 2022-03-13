
#include"../../../../system/main.h"
#include "gbfs_node.h"
#include"../../../../directx/resource/model_manager.h"
#include"../../../component/mesh_component/instancing_model_component.h"
#include"../../../component/mesh_component/model_component.h"

/**
* @namespace GBFSNodeData
* @brief GBFSNode情報
*/
namespace GBFSNodeData
{
	//! ノードモデルファイルネーム
	static const char* k_NodeModelFileName = "asset/model/ai/node/node.bin";

	//! ノードモデルスケール
	static const Conv_XM::Vector3f k_NodeScale = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
}

void GBFSNode::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(GBFSNodeData::k_NodeModelFileName);
}

void GBFSNode::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(GBFSNodeData::k_NodeModelFileName);
}

GBFSNode::GBFSNode()
{

}

GBFSNode::~GBFSNode()
{
}

void GBFSNode::LoadActor()
{
	// ノードオブジェクトインスタンシングモデルコンポーネント作成
	m_NodeInstancingModelComponent = CreateComponent<InstancingModelComponent>();
}

void GBFSNode::InitActor()
{
	if (m_NodeInstancingModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AINode::InitActor nullptr");
		return;
	}

	// インスタンシングデータ入力
	Conv_XM::Matrix4x4f mtxS, mtxT, mtxW;
	std::vector<Conv_XM::Matrix4x4f> nodeMTX;
	mtxS = DirectX::XMMatrixScaling(GBFSNodeData::k_NodeScale.x, GBFSNodeData::k_NodeScale.y, GBFSNodeData::k_NodeScale.z);

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
	Model* nodeModel = modelManager.GetModel(GBFSNodeData::k_NodeModelFileName);
	if (nodeModel == nullptr)
	{
		Logger::GetInstance().SetLog("GBFSNode::InitActor nodeModelがnullptr");
		return;
	}
	m_NodeInstancingModelComponent->SetModel(nodeModel);
}

void GBFSNode::UpdateActor()
{
}
