/**
* @file weighted_graph_node_loder.h
* @brief Weighted_Graph読み込み
*/


#pragma once


#include"../actor.h"

#include"../../../ai/weighted_graph.h"

/**
* @brief Weighted_Graph読み込みクラス
*/
class WeightedGraphLoader : public Actor
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

	//! 出力用ノードペアリスト
	std::vector<StoreEdgeData> m_StoreEdgeData;
public:
	WeightedGraphLoader();
	~WeightedGraphLoader();

	/**
	* @brief 入力
	*/
	void Load(const char* fileName);

	inline WeightedGraph GetWeightGraph() { return m_WeightedGraph; }
protected:

	/**
	* @brief Weighted_Graph読み込み独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief Weighted_Graph読み込み独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief Weighted_Graph読み込み独自の終了処理
	*/
	virtual void UninitActor() override;

	/**
	* @brief Weighted_Graph読み込み独自の更新処理
	*/
	virtual void UpdateActor()override;


private:

	/**
	* @brief 両方向からいけるWeightedEdgeの作成処理
	*/
	void MakeBothDirectionsWeightedEdge(WeightedGraphNode* target001, WeightedGraphNode* target002, float weight);

};