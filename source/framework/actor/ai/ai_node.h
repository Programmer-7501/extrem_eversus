/**
 * @file ai_node.h
 * @brief AI�pNode�z�u
 */

#pragma once

#include"../actor.h"

#include"../../../ai/bfs.h"

 /**
 * @brief AINode�N���X
 */
class AINode : public Actor
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

	//! �m�[�h�i�[�p
	Graph m_Graph;

	//! �o�H�}�b�v
	NodeToParentMap m_NodeToParentMap;

	class AITest* m_AITest;
private:
	//! �m�[�h�I�u�W�F�N�g
	class InstancingModelComponent* m_NodeInstancingModelComponent;

public:
	AINode();
	~AINode();

	inline void SetGraph(Graph graph) { m_Graph = graph; }
protected:

	/**
	* @brief AINode�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief AINode�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief AINode�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};