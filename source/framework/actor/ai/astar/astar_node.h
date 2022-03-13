/**
 * @file astar_node.h
 * @brief AStarNode�z�u
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/astar.h"

 /**
 * @brief AStarNode�N���X
 */
class AStarNode : public Actor
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


private:

	//! �m�[�h�i�[�p
	WeightedGraph m_WeightGraph;

	//! �o�H�}�b�v
	AStarMap m_AStarMap;

	//! �m�[�h�I�u�W�F�N�g
	class InstancingModelComponent* m_NodeInstancingModelComponent;

public:
	AStarNode();
	~AStarNode();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
protected:

	/**
	* @brief AStarNode�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief AStarNode�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief AStarNode�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};