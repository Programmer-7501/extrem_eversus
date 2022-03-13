/**
 * @file astar_node.h
 * @brief AStarNode配置
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/astar.h"

 /**
 * @brief AStarNodeクラス
 */
class AStarNode : public Actor
{
public:

	/**
	* @brief モデルの読み込み
	*/
	static void LoadModel();

	/**
	* @brief モデルの破棄
	*/
	static void UnloadModel();


private:

	//! ノード格納用
	WeightedGraph m_WeightGraph;

	//! 経路マップ
	AStarMap m_AStarMap;

	//! ノードオブジェクト
	class InstancingModelComponent* m_NodeInstancingModelComponent;

public:
	AStarNode();
	~AStarNode();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
protected:

	/**
	* @brief AStarNode独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief AStarNode独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief AStarNode独自の更新処理
	*/
	virtual void UpdateActor()override;
};