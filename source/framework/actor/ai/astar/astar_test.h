/**
 * @file astar_test.h
 * @brief AStarテスト用物体
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/astar.h"

 /**
 * @brief AStarテスト用物体クラス
 */
class AStarTest : public Actor
{
public:

	/**
	* @brief モデルの読み込み
	*/
	static void LoadModel();

	/**
	* @brief モデルの読み込み
	*/
	static void UnloadModel();

protected:
	//! モデル
	class ModelComponent* m_ModelComponent;

	//! ノード格納用
	WeightedGraph m_WeightGraph;

	//! 経路マップ
	AStarMap m_AStarMap;

	//! ルートマップ
	AStarRouteMap m_AStarRouteMap;

	// 次に向かうノード
	WeightedGraphNode* m_NextNode;

	//! ベクター配列のインデックス
	int m_Index;
public:
	AStarTest();
	~AStarTest();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
	inline AStarRouteMap GetRouteMap() { return m_AStarRouteMap; }
protected:

	/**
	* @brief AStarテスト独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief AStarテスト独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief AStarテスト独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 目的地変更
	*/
	void ChangeTarget();
};