/**
 * @file astar_test.h
 * @brief AStar�e�X�g�p����
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/astar.h"

 /**
 * @brief AStar�e�X�g�p���̃N���X
 */
class AStarTest : public Actor
{
public:

	/**
	* @brief ���f���̓ǂݍ���
	*/
	static void LoadModel();

	/**
	* @brief ���f���̓ǂݍ���
	*/
	static void UnloadModel();

protected:
	//! ���f��
	class ModelComponent* m_ModelComponent;

	//! �m�[�h�i�[�p
	WeightedGraph m_WeightGraph;

	//! �o�H�}�b�v
	AStarMap m_AStarMap;

	//! ���[�g�}�b�v
	AStarRouteMap m_AStarRouteMap;

	// ���Ɍ������m�[�h
	WeightedGraphNode* m_NextNode;

	//! �x�N�^�[�z��̃C���f�b�N�X
	int m_Index;
public:
	AStarTest();
	~AStarTest();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
	inline AStarRouteMap GetRouteMap() { return m_AStarRouteMap; }
protected:

	/**
	* @brief AStar�e�X�g�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief AStar�e�X�g�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief AStar�e�X�g�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

	/**
	* @brief �ړI�n�ύX
	*/
	void ChangeTarget();
};