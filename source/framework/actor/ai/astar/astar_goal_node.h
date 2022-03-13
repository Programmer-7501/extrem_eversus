/**
 * @file AStar_goal_node.h
 * @brief AStar用GoalNode配置
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/astar.h"

 /**
 * @brief AStarGoalNodeクラス
 */
class AStarGoalNode : public Actor
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

	class AStarTest* m_AStarTest;
private:
	//! モデルコンポーネント
	class ModelComponent* m_ModelComponent;
public:
	AStarGoalNode();
	~AStarGoalNode();

	inline void SetAStarTest(class AStarTest* astarTest) { m_AStarTest = astarTest; }
protected:

	/**
	* @brief AStarGoalNode独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief AStarGoalNode独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief AStarGoalNode独自の更新処理
	*/
	virtual void UpdateActor()override;
};