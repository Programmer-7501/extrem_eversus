
#include"../../../system/main.h"
#include "ai_node.h"
#include"../../../directx/resource/model_manager.h"
#include"../../component/mesh_component/instancing_model_component.h"
#include"../../component/mesh_component/model_component.h"

/**
* @namespace AINodeData
* @brief AINode���
*/
namespace AITestData
{
	//! �m�[�h���f���t�@�C���l�[��
	static const char* k_NodeModelFileName = "asset/model/ai/node/node.bin";

	//! �m�[�h���f���X�P�[��
	static const Conv_XM::Vector3f k_NodeScale = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
}

void AINode::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Load(AITestData::k_NodeModelFileName);
}

void AINode::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();

	modelManager.Unload(AITestData::k_NodeModelFileName);
}

AINode::AINode()
{
}

AINode::~AINode()
{
}

void AINode::LoadActor()
{
	// �m�[�h�I�u�W�F�N�g�C���X�^���V���O���f���R���|�[�l���g�쐬
	m_NodeInstancingModelComponent = CreateComponent<InstancingModelComponent>();
}

void AINode::InitActor()
{
	if (m_NodeInstancingModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AINode::InitActor nullptr");
		return;
	}

	// �C���X�^���V���O�f�[�^����
	Conv_XM::Matrix4x4f mtxS, mtxT, mtxW;
	std::vector<Conv_XM::Matrix4x4f> nodeMTX;
	mtxS = DirectX::XMMatrixScaling(AITestData::k_NodeScale.x, AITestData::k_NodeScale.y, AITestData::k_NodeScale.z);

	for (int i = 0; i < m_Graph.Nodes.size() ; i++)
	{
		mtxT = DirectX::XMMatrixTranslation(m_Graph.Nodes[i]->Position.x, m_Graph.Nodes[i]->Position.y, m_Graph.Nodes[i]->Position.z);
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
	Model* nodeModel = modelManager.GetModel(AITestData::k_NodeModelFileName);
	if (nodeModel == nullptr)
	{
		Logger::GetInstance().SetLog("AINode::InitActor nodeModel��nullptr");
		return;
	}
	m_NodeInstancingModelComponent->SetModel(nodeModel);
}

void AINode::UpdateActor()
{
}
