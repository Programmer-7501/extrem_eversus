/**
 * @file gbfs_node.h
 * @brief GBFSNode�z�u
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/gbfs.h"

 /**
 * @brief GBFSNode�N���X
 */
class GBFSNode : public Actor
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
	GBFSMap m_GBFSMap;

	//! �m�[�h�I�u�W�F�N�g
	class InstancingModelComponent* m_NodeInstancingModelComponent;

public:
	GBFSNode();
	~GBFSNode();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
protected:

	/**
	* @brief GBFSNode�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief GBFSNode�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief GBFSNode�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};