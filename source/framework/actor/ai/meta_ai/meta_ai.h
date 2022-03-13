#pragma once

/**
 * @file meta_ai.h
 * @brief ���^AI
 */

#pragma once

#include"../../actor.h"
#include"../../../../ai/astar.h"

 /**
 * @brief ���^AI�N���X
 */
class MetaAI : public Actor
{
public:
	enum Character
	{
		// ����
		PLAYER,
		// ����
		ALLY,
		// �G001
		ENEMY001,
		// �G002
		ENEMY002,
		// �ő吔
		CHARMAX
	};
public:

	//! �m�[�h�i�[�p
	WeightedGraph m_WeightGraph;

	//! �o�H�}�b�v
	AStarMap m_AStarMap[CHARMAX];

	//! ���[�g�}�b�v
	AStarRouteMap m_AStarRouteMap[CHARMAX];

	// ���Ɍ������m�[�h
	WeightedGraphNode* m_NextNode[CHARMAX];

	//! ���r���X�[�c
	class MobileSuit* m_MobileSuits[CHARMAX];

	//! ���^AI�X�e�[�g�}�b�v
	std::unordered_map<std::string, class MetaAIState*> m_MetaAIStateMap;
private:

public:
	MetaAI();
	~MetaAI();

	inline void SetWeightedGraph(WeightedGraph graph) { m_WeightGraph = graph; }

	inline void SetMobileSuit(class MobileSuit* player, class MobileSuit* ally, class MobileSuit* enemy001, class MobileSuit* enemy002)
	{
		m_MobileSuits[PLAYER] = player;
		m_MobileSuits[ALLY] = ally;
		m_MobileSuits[ENEMY001] = enemy001;
		m_MobileSuits[ENEMY002] = enemy002;
	}

protected:

	/**
	* @brief ���^AI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ���^AI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief ���^AI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

private:

	/**
	* @brief ���[�g�����肷��
	*/
	void DecideRoute();
};