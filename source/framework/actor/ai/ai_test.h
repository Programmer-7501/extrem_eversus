/**
 * @file ai_test.h
 * @brief AI�e�X�g�p����
 */

#pragma once

#include"../actor.h"

#include"../../../ai/bfs.h"

 /**
 * @brief AI�e�X�g�p���̃N���X
 */
class AITest : public Actor
{
public:
	static void LoadModel();
	static void UnloadModel();

protected:
	//! ���f��
	class ModelComponent* m_ModelComponent;

	//! �m�[�h�i�[�p
	Graph m_Graph;

	//! �o�H�}�b�v
	NodeToParentMap m_NodeToParentMap;

	//! ���[�g�}�b�v
	BFSRouteMap m_RouteMap;

	// ���Ɍ������m�[�h
	GraphNode* m_NextNode;

	//! �x�N�^�[�z��̃C���f�b�N�X
	int m_Index;
public:
	AITest();
	~AITest();

	inline void SetGraph(Graph graph) { m_Graph = graph; }
	inline BFSRouteMap GetRouteMap() { return m_RouteMap; }
protected:

	/**
	* @brief AI�e�X�g�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief AI�e�X�g�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief AI�e�X�g�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

	/**
	* @brief �ړI�n�ύX
	*/
	void ChangeTarget();
};