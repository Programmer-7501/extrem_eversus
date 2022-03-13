/**
 * @file ai_goal_node.h
 * @brief AI�pGoalNode�z�u
 */

#pragma once

#include"../actor.h"

#include"../../../ai/bfs.h"

 /**
 * @brief AIGoalNode�N���X
 */
class AIGoalNode : public Actor
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

	class AITest* m_AITest;
private:
	//! ���f���R���|�[�l���g
	class ModelComponent* m_ModelComponent;
public:
	AIGoalNode();
	~AIGoalNode();

	inline void SetAITest(class AITest* aiTest) { m_AITest = aiTest; }
protected:

	/**
	* @brief AIGoalNode�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief AIGoalNode�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief AIGoalNode�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};