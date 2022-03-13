/**
 * @file gbfs_test.h
 * @brief GBFS�e�X�g�p����
 */

#pragma once

#include"../../actor.h"

#include"../../../../ai/gbfs.h"

 /**
 * @brief GBFS�e�X�g�p���̃N���X
 */
class GBFSTest : public Actor
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
	GBFSMap m_GBFSMap;

	//! ���[�g�}�b�v
	GBFSRouteMap m_GBFSRouteMap;

	// ���Ɍ������m�[�h
	WeightedGraphNode* m_NextNode;

	//! �x�N�^�[�z��̃C���f�b�N�X
	int m_Index;
public:
	GBFSTest();
	~GBFSTest();

	inline void SetWeightedGraph(WeightedGraph weightGraph) { m_WeightGraph = weightGraph; }
	inline GBFSRouteMap GetRouteMap() { return m_GBFSRouteMap; }
protected:

	/**
	* @brief GBFS�e�X�g�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief GBFS�e�X�g�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief GBFS�e�X�g�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

	/**
	* @brief �ړI�n�ύX
	*/
	void ChangeTarget();
};