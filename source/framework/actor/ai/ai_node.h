/**
 * @file ai_node.h
 * @brief AI用Node配置
 */

#pragma once

#include"../actor.h"

#include"../../../ai/bfs.h"

 /**
 * @brief AINodeクラス
 */
class AINode : public Actor
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

	//! ノード格納用
	Graph m_Graph;

	//! 経路マップ
	NodeToParentMap m_NodeToParentMap;

	class AITest* m_AITest;
private:
	//! ノードオブジェクト
	class InstancingModelComponent* m_NodeInstancingModelComponent;

public:
	AINode();
	~AINode();

	inline void SetGraph(Graph graph) { m_Graph = graph; }
protected:

	/**
	* @brief AINode独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief AINode独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief AINode独自の更新処理
	*/
	virtual void UpdateActor()override;
};