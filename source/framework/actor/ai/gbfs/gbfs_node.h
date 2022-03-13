/**
 * @file gbfs_node.h
 * @brief GBFSNode配置
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/gbfs.h"

 /**
 * @brief GBFSNodeクラス
 */
class GBFSNode : public Actor
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
	GBFSMap m_GBFSMap;

	//! ノードオブジェクト
	class InstancingModelComponent* m_NodeInstancingModelComponent;

public:
	GBFSNode();
	~GBFSNode();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
protected:

	/**
	* @brief GBFSNode独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief GBFSNode独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief GBFSNode独自の更新処理
	*/
	virtual void UpdateActor()override;
};