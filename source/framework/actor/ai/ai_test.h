/**
 * @file ai_test.h
 * @brief AIテスト用物体
 */

#pragma once

#include"../actor.h"

#include"../../../ai/bfs.h"

 /**
 * @brief AIテスト用物体クラス
 */
class AITest : public Actor
{
public:
	static void LoadModel();
	static void UnloadModel();

protected:
	//! モデル
	class ModelComponent* m_ModelComponent;

	//! ノード格納用
	Graph m_Graph;

	//! 経路マップ
	NodeToParentMap m_NodeToParentMap;

	//! ルートマップ
	BFSRouteMap m_RouteMap;

	// 次に向かうノード
	GraphNode* m_NextNode;

	//! ベクター配列のインデックス
	int m_Index;
public:
	AITest();
	~AITest();

	inline void SetGraph(Graph graph) { m_Graph = graph; }
	inline BFSRouteMap GetRouteMap() { return m_RouteMap; }
protected:

	/**
	* @brief AIテスト独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief AIテスト独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief AIテスト独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 目的地変更
	*/
	void ChangeTarget();
};