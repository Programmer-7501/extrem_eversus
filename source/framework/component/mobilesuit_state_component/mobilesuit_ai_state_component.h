/**
 * @file mobilesuit_ai_state_component.h
 * @brief ���r���X�[�cAI�X�e�[�g�R���|�[�l���g
 */

#pragma once

#include"mobilesuit_state_component.h"

#include"../../../ai/astar.h"

 /**
 * @brief ���r���X�[�cAI�X�e�[�g�R���|�[�l���g
 */
class MobileSuitAIStateComponent : public MobileSuitStateComponent
{
protected:
	//! �L�����N�^�[AI�X�e�[�g�}�b�v
	std::unordered_map<std::string, class CharcterAIState*> m_CharacterAIStateMap;

	//! ���݂̏��
	class CharcterAIState* m_CurrentCharacterAIState;

	// �m�[�h�O���t
	WeightedGraph m_WeightedGraph;

	//! �o�H�}�b�v
	AStarMap m_AStarMap;

	//! ���[�g�}�b�v
	AStarRouteMap m_AStarRouteMap;

	//! ���Ɍ������m�[�h
	WeightedGraphNode* m_NextNode;

	//! �o�H��T���ς݂��ǂ���
	bool m_IsPathfinding;

	//! �x�N�^�[�z��̕ۊǗp�C���f�b�N�X
	int m_TargetNodeIndex;
public:
	MobileSuitAIStateComponent(class Actor* owner);
	~MobileSuitAIStateComponent();

	/**
	* @brief �L�����N�^�[AI�̏�ԕύX
	* @param[in] const std::string&(name) ��Ԗ�
	*/
	void ChangeCharacterAIState(const std::string& name);

	/**
	* @brief �L�����N�^�[AI�X�e�[�g�N���G�C�g�֐�
	*/
	template <typename T>
	void CreateCharacterState()
	{
		//�쐬
		T* state = new T(this);

		if (state == nullptr)
		{
			Logger::GetInstance().SetLog("CreateCharacterState state��nullptr");
			return;
		}

		const auto& iter = m_CharacterAIStateMap.find(state->GetName());

		//�d�����Ă��Ȃ����`�F�b�N
		if (iter != m_CharacterAIStateMap.end())
		{
			//�d�����Ă�����폜
			delete state;
			state = nullptr;
		}
		else
		{
			//�o�^
			m_CharacterAIStateMap.emplace(state->GetName(), state);
		}
	}

	inline void SetWeightedGraph(WeightedGraph weightedGraph) { m_WeightedGraph = weightedGraph; }

	inline bool GetIsPathfinding() { return m_IsPathfinding; }

	WeightedGraphNode* GetNextNode() { return m_NextNode; }

	inline class CharcterAIState* GetCharcterAIState() { return m_CurrentCharacterAIState; }

	/**
	* @brief �o�H�T��
	* @param[in] const Conv_XM::Vector3f(target) �ړI�n
	*/
	void Pathfinding(const Conv_XM::Vector3f& target);
protected:

	/**
	* @brief ����������
	*/
	virtual void Init()override;

	/**
	* @brief �I������
	*/
	virtual void Uninit()override;

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput()override;

	/**
	* @brief �X�V����
	*/
	virtual void Update()override;

	/**
	* @brief ���͂���ɂ���
	*/
	void CleanInput();

	/**
	* @brief �ړI�n�ɂ������ǂ����𔻕ʂ���
	*/
	void IsArrivalTarget();
};