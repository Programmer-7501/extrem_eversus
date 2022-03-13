/**
 * @file GBFS_goal_node.h
 * @brief GBFS用GoalNode配置
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/gbfs.h"

 /**
 * @brief GBFSGoalNodeクラス
 */
class GBFSGoalNode : public Actor
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

	class GBFSTest* m_GBFSTest;
private:
	//! モデルコンポーネント
	class ModelComponent* m_ModelComponent;
public:
	GBFSGoalNode();
	~GBFSGoalNode();

	inline void SetBGFSTest(class GBFSTest* gbfsTest) { m_GBFSTest = gbfsTest; }
protected:

	/**
	* @brief GBFSGoalNode独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief GBFSGoalNode独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief GBFSGoalNode独自の更新処理
	*/
	virtual void UpdateActor()override;
};