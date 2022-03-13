/**
 * @file gbfs_test.h
 * @brief GBFSテスト用物体
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/gbfs.h"

 /**
 * @brief GBFSテスト用物体クラス
 */
class GBFSTest : public Actor
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
	GBFSMap m_GBFSMap;

	//! ルートマップ
	GBFSRouteMap m_GBFSRouteMap;

	// 次に向かうノード
	WeightedGraphNode* m_NextNode;

	//! ベクター配列のインデックス
	int m_Index;
public:
	GBFSTest();
	~GBFSTest();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
	inline GBFSRouteMap GetRouteMap() { return m_GBFSRouteMap; }
protected:

	/**
	* @brief GBFSテスト独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief GBFSテスト独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief GBFSテスト独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 目的地変更
	*/
	void ChangeTarget();
};