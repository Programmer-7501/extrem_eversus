/**
 * @file ai_goal_node.h
 * @brief AI用GoalNode配置
 */

#pragma once

#include"../actor.h"

#include"../../../ai/bfs.h"

 /**
 * @brief AIGoalNodeクラス
 */
class AIGoalNode : public Actor
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

	class AITest* m_AITest;
private:
	//! モデルコンポーネント
	class ModelComponent* m_ModelComponent;
public:
	AIGoalNode();
	~AIGoalNode();

	inline void SetAITest(class AITest* aiTest) { m_AITest = aiTest; }
protected:

	/**
	* @brief AIGoalNode独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief AIGoalNode独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief AIGoalNode独自の更新処理
	*/
	virtual void UpdateActor()override;
};