/**
 * @file AStar_goal_node.h
 * @brief AStar�pGoalNode�z�u
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/astar.h"

 /**
 * @brief AStarGoalNode�N���X
 */
class AStarGoalNode : public Actor
{
public:

	/**
	* @brief ���f���̓ǂݍ���
	*/
	static void LoadModel();

	/**
	* @brief ���f���̔j��
	*/
	static void UnloadModel();

	class AStarTest* m_AStarTest;
private:
	//! ���f���R���|�[�l���g
	class ModelComponent* m_ModelComponent;
public:
	AStarGoalNode();
	~AStarGoalNode();

	inline void SetAStarTest(class AStarTest* astarTest) { m_AStarTest = astarTest; }
protected:

	/**
	* @brief AStarGoalNode�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief AStarGoalNode�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief AStarGoalNode�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};