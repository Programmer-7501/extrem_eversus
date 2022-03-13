/**
* @file weighted_graph_node_loder.h
* @brief Weighted_Graph�ǂݍ���
*/


#pragma once


#include"../actor.h"

#include"../../../ai/weighted_graph.h"

/**
* @brief Weighted_Graph�ǂݍ��݃N���X
*/
class WeightedGraphLoader : public Actor
{
private:
	struct StoreEdgeData
	{
		int Index001;
		int Index002;
		float Weight;
	};

public:

	/**
	* @brief ���f���̓ǂݍ���
	*/
	static void LoadModel();

	/**
	* @brief ���f���̓ǂݍ���
	*/
	static void UnloadModel();
private:
	//! �d�ݕt���O���t
	WeightedGraph m_WeightedGraph;

	//! �C���X�^���V���O���f���R���|�[�l���g
	class InstancingColorModelComponent* m_NodeInstancingColorModelComponent;

	//! �o�͗p�m�[�h�y�A���X�g
	std::vector<StoreEdgeData> m_StoreEdgeData;
public:
	WeightedGraphLoader();
	~WeightedGraphLoader();

	/**
	* @brief ����
	*/
	void Load(const char* fileName);

	inline WeightedGraph GetWeightGraph() { return m_WeightedGraph; }
protected:

	/**
	* @brief Weighted_Graph�ǂݍ��ݓƎ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief Weighted_Graph�ǂݍ��ݓƎ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief Weighted_Graph�ǂݍ��ݓƎ��̏I������
	*/
	virtual void UninitActor() override;

	/**
	* @brief Weighted_Graph�ǂݍ��ݓƎ��̍X�V����
	*/
	virtual void UpdateActor()override;


private:

	/**
	* @brief ���������炢����WeightedEdge�̍쐬����
	*/
	void MakeBothDirectionsWeightedEdge(WeightedGraphNode* target001, WeightedGraphNode* target002, float weight);

};