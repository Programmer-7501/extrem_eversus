

#include"../../../system/main.h"
#include "weighted_graph_node_maker.h"
#include"../../component/mesh_component/instancing_color_model_component.h"
#include"../../component/mesh_component/model_component.h"
#include"../../component/mesh_component/debug/debug_number_billboard_component.h"
#include"../../../directx/resource/model_manager.h"
#include"../../../system/myimgui.h"

/**
* @namespace WeightedGraphMakerData
* @brief WeightedGraphMaker情報
*/
namespace WeightedGraphMakerData
{
	//! ノードモデルファイルネーム
	static const char* k_NodeModelFileName = "asset/model/ai/node/node.bin";

	//! ノードモデルスケール
	static const Conv_XM::Vector3f k_NodeScale = Conv_XM::Vector3f(0.2f, 0.2f, 0.2f);

	//! 出力ファイル名
	static const char* k_GraphFileName = "asset/graphdata/graph.bin";

	//! 位置表示モデルサイズ
	static const Conv_XM::Vector3f k_Scale = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);

	//! エッジモデルスケール
	static const Conv_XM::Vector3f k_EdgeScale = Conv_XM::Vector3f(0.2f, 0.2f, 0.2f);

	//! エッジモデルカラー
	static const Conv_XM::Vector4f k_OneEdgeColor = Conv_XM::Vector4f(0.2f, 0.2f, 1.0f, 1.0f);
	static const Conv_XM::Vector4f k_TwoEdgeColor = Conv_XM::Vector4f(0.2f, 1.0f, 0.2f, 1.0f);
	static const Conv_XM::Vector4f k_ThreeEdgeColor = Conv_XM::Vector4f(1.0f, 0.2f, 0.2f, 1.0f);

	//! 最大インスタンシング数
	static constexpr int k_MaxInstancingNum = 8192;
}

void WeightedGraphMaker::LoadModel()
{
	// モデル読み込み
	ModelManager& modelManager = ModelManager::GetInstance();
	modelManager.Load(WeightedGraphMakerData::k_NodeModelFileName);
}

void WeightedGraphMaker::UnloadModel()
{
	// モデル破棄
	ModelManager& modelManager = ModelManager::GetInstance();
	modelManager.Unload(WeightedGraphMakerData::k_NodeModelFileName);
}

WeightedGraphMaker::WeightedGraphMaker()
	:m_NodeInstancingColorModelComponent(nullptr)
	,m_NodePosition{}
	, m_NodeIndex{}
	, m_Weight(0.0f)
	, m_DeleteIndex(0)
	, m_FrameCount(0)
{
}

WeightedGraphMaker::~WeightedGraphMaker()
{
}

void WeightedGraphMaker::LoadActor()
{
	// インスタンシングコンポーネント作成
	m_NodeInstancingColorModelComponent = CreateComponent<InstancingColorModelComponent>();
	m_NodeInstancingColorModelComponent->SetMaxInstancingNum(WeightedGraphMakerData::k_MaxInstancingNum);
	// モデルコンポーネント作成
	m_ModelComponent = CreateComponent<ModelComponent>();

	// デバッグ数字ビルボードコンポーネント
	for (int i = 0; i < DEBUGSPRITENUM; i++)
	{
		m_DebugNumberBillboardComponent[i] = CreateComponent<DebugNumberBillBoardComponent>();
	}
}

void WeightedGraphMaker::InitActor()
{
	if (m_NodeInstancingColorModelComponent == nullptr || m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("AINode::InitActor nullptr");
		return;
	}

	// マテリアル設定
	CBufferData::Material material;
	material.Metaric = 0.5f;
	material.Smoothness = 0.5f;

	m_NodeInstancingColorModelComponent->SetMaterial(material);

	// モデル設定
	ModelManager& modelManager = ModelManager::GetInstance();
	Model* nodeModel = modelManager.GetModel(WeightedGraphMakerData::k_NodeModelFileName);
	if (nodeModel == nullptr)
	{
		Logger::GetInstance().SetLog("AStarNode::InitActor nodeModelがnullptr");
		return;
	}
	m_NodeInstancingColorModelComponent->SetModel(nodeModel);

	m_ModelComponent->SetMaterial(material);
	m_ModelComponent->SetModel(nodeModel);
	SetScale(WeightedGraphMakerData::k_Scale);

	// デバッグ数字ビルボードコンポーネント
	for (int i = 0; i < DEBUGSPRITENUM; i++)
	{
		m_DebugNumberBillboardComponent[i]->SetVisible(false);
	}
}

void WeightedGraphMaker::UninitActor()
{
	for (auto* node : m_WeightedGraph.Nodes)
	{
		for (auto* edge : node->Edges)
		{
			if (edge)
			{
				delete edge;
				edge = nullptr;
			}
		}

		if (node)
		{
			delete node;
			node = nullptr;
		}
	}

	m_StoreEdgeData.clear();
}

void WeightedGraphMaker::UpdateActor()
{
	ImGui::Begin(MyImGuiData::k_DebugWindowName);

	ImGui::InputFloat3("NodePosition", m_NodePosition);

	if (ImGui::Button("MakeWeightedGraphNode") == true)
	{
		MakeWeightedGraphNode(Conv_XM::Vector3f(m_NodePosition[0], m_NodePosition[1], m_NodePosition[2]));
	}

	SetPosition(Conv_XM::Vector3f(m_NodePosition[0], m_NodePosition[1], m_NodePosition[2]));
	if (m_FrameCount % 100 < 80)
	{
		m_ModelComponent->SetVisible(true);
	}
	else
	{
		m_ModelComponent->SetVisible(false);
	}

	ImGui::InputInt2("SelectConnectNodeIndex", m_NodeIndex);
	ImGui::InputFloat("EdgeWeight", &m_Weight);
	
	if (ImGui::Button("MakeBothDirectionsWeightedEdge") == true)
	{
		if (m_NodeIndex[0] < m_WeightedGraph.Nodes.size() && m_NodeIndex[1] < m_WeightedGraph.Nodes.size() && m_NodeIndex[0] != m_NodeIndex[1])
		{
			MakeBothDirectionsWeightedEdge(m_WeightedGraph.Nodes[m_NodeIndex[0]], m_WeightedGraph.Nodes[m_NodeIndex[1]], m_Weight);
			StoreEdgeData data = {};
			data.Index001 = m_NodeIndex[0];
			data.Index002 = m_NodeIndex[1];
			data.Weight = m_Weight;
			m_StoreEdgeData.emplace_back(data);

			UpdateInstancingModel();
		}
		else
		{
			ImGui::Text("error");
		}
	}


	ImGui::InputInt("DeleteIndex", &m_DeleteIndex);
	if (ImGui::Button("Delete") == true)
	{
		if (m_WeightedGraph.Nodes.empty() == false && m_DeleteIndex < m_WeightedGraph.Nodes.size())
		{
			DeleteNode(m_WeightedGraph.Nodes[m_DeleteIndex]);
		}
	}

	ImGui::InputText("SaveFileName", m_SaveFileName, sizeof(m_SaveFileName));
	if (ImGui::Button("Save") == true)
	{
		Output(m_SaveFileName);
	}


	ImGui::InputText("LoadFileName", m_LoadFileName, sizeof(m_LoadFileName));
	if (ImGui::Button("Load") == true)
	{
		Load(m_LoadFileName);
	}

	int loop = 0;
	for (auto* node : m_WeightedGraph.Nodes)
	{
		ImGui::Text(u8"%d番のノードのポジション(X:%f Y:%f Z:%f", loop, node->Position.x, node->Position.y, node->Position.z);

		for (auto* edge : node->Edges)
		{
			ImGui::Text(u8"行先のポジション(X:%f Y:%f Z:%f) 重み:%f", edge->To->Position.x, edge->To->Position.y, edge->To->Position.z, edge->mWeight);
		}

		loop++;
	}

	ImGui::End();

	// カウントアップ
	m_FrameCount++;
}

void WeightedGraphMaker::MakeWeightedGraphNode(const Conv_XM::Vector3f& position)
{
	WeightedGraphNode* weightedGraph = new WeightedGraphNode;
	weightedGraph->Position = position;
	m_WeightedGraph.Nodes.emplace_back(weightedGraph);

	UpdateInstancingModel();
}

void WeightedGraphMaker::MakeWeightedEdge(WeightedGraphNode* target, WeightedGraphNode * to, float weight)
{
	WeightedEdge* edge = new WeightedEdge;
	edge->From = target;
	edge->To = to;
	edge->mWeight = weight;
	target->Edges.emplace_back(edge);
}

void WeightedGraphMaker::MakeBothDirectionsWeightedEdge(WeightedGraphNode * target001, WeightedGraphNode * target002, float weight)
{
	WeightedEdge* edge001 = new WeightedEdge;
	edge001->From = target001;
	edge001->To = target002;
	edge001->mWeight = weight;
	target001->Edges.emplace_back(edge001);

	WeightedEdge* edge002 = new WeightedEdge;
	edge002->From = target002;
	edge002->To = target001;
	edge002->mWeight = weight;
	target002->Edges.emplace_back(edge002);
}

void WeightedGraphMaker::UpdateInstancingModel()
{
	std::vector<InstancingColorModelComponent::InstanceColorData> instancingData;

	Conv_XM::Matrix4x4f mtxW, mtxT, mtxS;
	InstancingColorModelComponent::InstanceColorData data;
	data.Color = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	int loop = 0;
	int debugSpriteNum = 0;
	Conv_XM::Vector3f debugSpritePosition;
	for (auto node : m_WeightedGraph.Nodes)
	{
		mtxS = DirectX::XMMatrixScaling(WeightedGraphMakerData::k_NodeScale.x, WeightedGraphMakerData::k_NodeScale.y, WeightedGraphMakerData::k_NodeScale.z);
		mtxT = DirectX::XMMatrixTranslation(node->Position.x, node->Position.y, node->Position.z);
		mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxT;
		mtxW = DirectX::XMMatrixTranspose(mtxW);
		data.Matrix = mtxW;
		instancingData.emplace_back(data);

		debugSpritePosition = node->Position;
		debugSpritePosition.y += 1.0f;

		if (debugSpriteNum >= DEBUGSPRITENUM)
		{
			continue;
		}
		if(loop >= 100)
		{
			int hundreds = loop / 100;
			int tenth = (loop / 10) % 10;
			int onece = loop % 100;
			debugSpritePosition.x -= 2.0f;
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetVisible(true);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetPosition(debugSpritePosition);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetNumber(hundreds);
			debugSpriteNum++;
			debugSpritePosition.x += 1.0f;
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetVisible(true);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetPosition(debugSpritePosition);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetNumber(tenth);
			debugSpriteNum++;
			debugSpritePosition.x += 1.0f;
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetVisible(true);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetPosition(debugSpritePosition);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetNumber(onece);
			debugSpriteNum++;
		}
		else if (loop >= 10)
		{
			int tenth = loop / 10;
			int onece = loop % 10;
			debugSpritePosition.x -= 1.0f;
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetVisible(true);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetPosition(debugSpritePosition);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetNumber(tenth);
			debugSpriteNum++;
			debugSpritePosition.x += 2.0f;
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetVisible(true);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetPosition(debugSpritePosition);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetNumber(onece);
			debugSpriteNum++;
		}
		else
		{
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetVisible(true);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetPosition(debugSpritePosition);
			m_DebugNumberBillboardComponent[debugSpriteNum]->SetNumber(loop);
			debugSpriteNum++;
		}
		loop++;
	}

	Conv_XM::Vector3f edgepos;
	for (auto edge : m_StoreEdgeData)
	{
		if (edge.Weight <= 1.0f)
		{
			data.Color = WeightedGraphMakerData::k_OneEdgeColor;
		}
		else if (edge.Weight <= 2.0f)
		{
			data.Color = WeightedGraphMakerData::k_TwoEdgeColor;
		}
		else
		{
			data.Color = WeightedGraphMakerData::k_ThreeEdgeColor;
		}
		mtxS = DirectX::XMMatrixScaling(WeightedGraphMakerData::k_EdgeScale.x, WeightedGraphMakerData::k_EdgeScale.y, WeightedGraphMakerData::k_EdgeScale.z);

		for (int i = 0; i < 4; i++)
		{
			float t = (1.0f / 5.0f) * static_cast<float>(i + 1);
			Conv_XM::Vector3f::Linear(&edgepos, m_WeightedGraph.Nodes[edge.Index001]->Position, m_WeightedGraph.Nodes[edge.Index002]->Position, t);
			mtxT = DirectX::XMMatrixTranslation(edgepos.x, edgepos.y, edgepos.z);
			mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxT;
			mtxW = DirectX::XMMatrixTranspose(mtxW);
			data.Matrix = mtxW;
			instancingData.emplace_back(data);
		}
	}

	if (m_NodeInstancingColorModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("WeightedGraphMaker::UpdateInstancingModel nullptr");
		return;
	}

	m_NodeInstancingColorModelComponent->SetInstanceData(instancingData);
}

void WeightedGraphMaker::DeleteNode(WeightedGraphNode * target)
{
	// イテレーターを取得
	auto&& iter = std::find(m_WeightedGraph.Nodes.begin(), m_WeightedGraph.Nodes.end(), target);

	// 存在するなら
	if (iter != m_WeightedGraph.Nodes.end())
	{
		// このノードが登録されているエッジを削除
		for (auto* node : m_WeightedGraph.Nodes)
		{
			if (node == nullptr)
			{
				continue;
			}

			std::vector<int> deleteIndex;
			int loop = 0;
			for (auto* edge : node->Edges)
			{
				if (edge == nullptr)
				{
					continue;
				}
				if (target == edge->From || target == edge->To)
				{
					if (edge)
					{
						delete edge;
						edge = nullptr;

						deleteIndex.emplace_back(loop);
					}
				}

				loop++;
			}

			// エッジリストから削除
			for (auto index : deleteIndex)
			{
				// イテレーターを取得
				auto&& edgeIter = std::find(node->Edges.begin(), node->Edges.end(), node->Edges[index]);
				node->Edges.erase(edgeIter);
			}
		}

		// erase
		m_WeightedGraph.Nodes.erase(iter);

		// ノード削除
		if (target)
		{
			delete target;
			target = nullptr;
		}

		// インスタンシングモデル更新
		UpdateInstancingModel();
	}

}

void WeightedGraphMaker::Output(char* fileName)
{
	std::string path, name;
	name = fileName;
	path = "asset/graphdata/" + name + ".bin";
	//ファイル名からバイナリファイルで書き出す
	FILE *fp = fopen(path.c_str(), "wb");
	if (fp == nullptr)
	{
		Logger::GetInstance().SetLog("WeightedGraphMaker::Output ファイルポインタがnullptr");
		return;
	}

	//グラフノードの数を書き込む
	int nodeNum = static_cast<int>(m_WeightedGraph.Nodes.size());
	fwrite(&nodeNum, sizeof(nodeNum), 1, fp);

	// ノードのポジションを出力
	for (auto* node : m_WeightedGraph.Nodes)
	{
		fwrite(&node->Position, sizeof(Conv_XM::Vector3f), 1, fp);
	}


	//エッジデータ数を書き込む(行きと帰りで1個)
	int edgeNum = static_cast<int>(m_StoreEdgeData.size());
	fwrite(&edgeNum, sizeof(edgeNum), 1, fp);

	// エッジデータ出力
	for (auto edge : m_StoreEdgeData)
	{
		fwrite(&edge, sizeof(StoreEdgeData), 1, fp);
	}

	fclose(fp);
}

void WeightedGraphMaker::Load(char* fileName)
{
	std::string path, name;
	name = fileName;
	path = "asset/graphdata/" + name + ".bin";
	//ファイル名からバイナリファイルで書き出す
	FILE *fp = fopen(path.c_str(), "rb");
	if (fp == nullptr)
	{
		Logger::GetInstance().SetLog("WeightedGraphMaker::Output ファイルポインタがnullptr");
		return;
	}

	//グラフノードの数を読み込む
	int nodeNum = 0;
	fread(&nodeNum, sizeof(nodeNum), 1, fp);

	// ノードを作成
	Conv_XM::Vector3f pos;
	for (int i = 0; i < nodeNum; i++)
	{
		fread(&pos, sizeof(Conv_XM::Vector3f), 1, fp);
		WeightedGraphNode* weightedGraph = new WeightedGraphNode;
		weightedGraph->Position = pos;
		m_WeightedGraph.Nodes.emplace_back(weightedGraph);
	}


	//エッジデータの数を読み込む
	int edgeNum = 0;
	fread(&edgeNum, sizeof(edgeNum), 1, fp);

	// エッジデータ出力
	StoreEdgeData edge = {};
	for (int i = 0; i < edgeNum; i++)
	{
		fread(&edge, sizeof(StoreEdgeData), 1, fp);

		MakeBothDirectionsWeightedEdge(m_WeightedGraph.Nodes[edge.Index001], m_WeightedGraph.Nodes[edge.Index002], edge.Weight);
		m_StoreEdgeData.emplace_back(edge);
	}

	fclose(fp);

	// インスタンシングモデルの更新
	UpdateInstancingModel();
}
