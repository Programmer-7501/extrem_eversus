/**
* @file weighted_graph_node_maker.h
* @brief Weighted_Graph作成
*/


#pragma once


#include"../actor.h"

#include"../../../ai/weighted_graph.h"

#define MAXFILENAME (256)
#define DEBUGSPRITENUM (1024)
/**
* @brief Weighted_Graph作成クラス
*/
class WeightedGraphMaker : public Actor
{
private:
	struct StoreEdgeData
	{
		int Index001;
		int Index002;
		float Weight;
	};

public:

	/**
	* @brief モデルの読み込み
	*/
	static void LoadModel();

	/**
	* @brief モデルの読み込み
	*/
	static void UnloadModel();
private:
	//! 重み付きグラフ
	WeightedGraph m_WeightedGraph;

	//! インスタンシングモデルコンポーネント
	class InstancingColorModelComponent* m_NodeInstancingColorModelComponent;

	//! モデルコンポーネント
	class ModelComponent* m_ModelComponent;

	//! デバッグナンバービルボードコンポーネント
	class DebugNumberBillBoardComponent* m_DebugNumberBillboardComponent[DEBUGSPRITENUM];

	//! ImGuiポジション入力用
	float m_NodePosition[3];

	//! ImGuiノードインデックス選択用
	int m_NodeIndex[2];

	//! ImGui エッジ重み入力用
	float m_Weight;

	//! ImGui 削除入力用
	int m_DeleteIndex;

	//! 出力用ノードペアリスト
	std::vector<StoreEdgeData> m_StoreEdgeData;

	//! ImGui セーブファイル名入力用
	char m_SaveFileName[MAXFILENAME];

	//! ImGui ロードファイル名入力用
	char m_LoadFileName[MAXFILENAME];

	//! フレームカウント
	int m_FrameCount;
public:
	WeightedGraphMaker();
	~WeightedGraphMaker();

protected:

	/**
	* @brief Weighted_Graph作成独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief Weighted_Graph作成独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief Weighted_Graph作成独自の終了処理
	*/
	virtual void UninitActor() override;

	/**
	* @brief Weighted_Graph作成独自の更新処理
	*/
	virtual void UpdateActor()override;

private:

	/**
	* @brief WeightedGraphNodeの作成処理
	*/
	void MakeWeightedGraphNode(const Conv_XM::Vector3f& position);

	/**
	* @brief WeightedEdgeの作成処理
	*/
	void MakeWeightedEdge(WeightedGraphNode* target, WeightedGraphNode* to,float weight);


	/**
	* @brief 両方向からいけるWeightedEdgeの作成処理
	*/
	void MakeBothDirectionsWeightedEdge(WeightedGraphNode* target001, WeightedGraphNode* target002, float weight);

	/**
	* @brief インスタンシングモデルの更新
	*/
	void UpdateInstancingModel();

	/**
	* @brief ノードの削除
	*/
	void DeleteNode(WeightedGraphNode* target);

	/**
	* @brief 出力
	*/
	void Output(char* fileName);

	/**
	* @brief 入力
	*/
	void Load(char* fileName);
};