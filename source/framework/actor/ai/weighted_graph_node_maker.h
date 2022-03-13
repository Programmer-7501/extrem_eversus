/**
* @file weighted_graph_node_maker.h
* @brief Weighted_Graph�쐬
*/


#pragma once


#include"../actor.h"

#include"../../../ai/weighted_graph.h"

#define MAXFILENAME (256)
#define DEBUGSPRITENUM (1024)
/**
* @brief Weighted_Graph�쐬�N���X
*/
class WeightedGraphMaker : public Actor
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

	//! ���f���R���|�[�l���g
	class ModelComponent* m_ModelComponent;

	//! �f�o�b�O�i���o�[�r���{�[�h�R���|�[�l���g
	class DebugNumberBillBoardComponent* m_DebugNumberBillboardComponent[DEBUGSPRITENUM];

	//! ImGui�|�W�V�������͗p
	float m_NodePosition[3];

	//! ImGui�m�[�h�C���f�b�N�X�I��p
	int m_NodeIndex[2];

	//! ImGui �G�b�W�d�ݓ��͗p
	float m_Weight;

	//! ImGui �폜���͗p
	int m_DeleteIndex;

	//! �o�͗p�m�[�h�y�A���X�g
	std::vector<StoreEdgeData> m_StoreEdgeData;

	//! ImGui �Z�[�u�t�@�C�������͗p
	char m_SaveFileName[MAXFILENAME];

	//! ImGui ���[�h�t�@�C�������͗p
	char m_LoadFileName[MAXFILENAME];

	//! �t���[���J�E���g
	int m_FrameCount;
public:
	WeightedGraphMaker();
	~WeightedGraphMaker();

protected:

	/**
	* @brief Weighted_Graph�쐬�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief Weighted_Graph�쐬�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief Weighted_Graph�쐬�Ǝ��̏I������
	*/
	virtual void UninitActor() override;

	/**
	* @brief Weighted_Graph�쐬�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

private:

	/**
	* @brief WeightedGraphNode�̍쐬����
	*/
	void MakeWeightedGraphNode(const Conv_XM::Vector3f& position);

	/**
	* @brief WeightedEdge�̍쐬����
	*/
	void MakeWeightedEdge(WeightedGraphNode* target, WeightedGraphNode* to,float weight);


	/**
	* @brief ���������炢����WeightedEdge�̍쐬����
	*/
	void MakeBothDirectionsWeightedEdge(WeightedGraphNode* target001, WeightedGraphNode* target002, float weight);

	/**
	* @brief �C���X�^���V���O���f���̍X�V
	*/
	void UpdateInstancingModel();

	/**
	* @brief �m�[�h�̍폜
	*/
	void DeleteNode(WeightedGraphNode* target);

	/**
	* @brief �o��
	*/
	void Output(char* fileName);

	/**
	* @brief ����
	*/
	void Load(char* fileName);
};