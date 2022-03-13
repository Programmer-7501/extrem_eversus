/**
 * @file GBFS_goal_node.h
 * @brief GBFS�pGoalNode�z�u
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/gbfs.h"

 /**
 * @brief GBFSGoalNode�N���X
 */
class GBFSGoalNode : public Actor
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

	class GBFSTest* m_GBFSTest;
private:
	//! ���f���R���|�[�l���g
	class ModelComponent* m_ModelComponent;
public:
	GBFSGoalNode();
	~GBFSGoalNode();

	inline void SetBGFSTest(class GBFSTest* gbfsTest) { m_GBFSTest = gbfsTest; }
protected:

	/**
	* @brief GBFSGoalNode�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief GBFSGoalNode�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief GBFSGoalNode�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};