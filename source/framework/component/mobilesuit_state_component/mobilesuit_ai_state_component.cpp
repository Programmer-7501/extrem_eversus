
#include"../../../system/main.h"
#include"mobilesuit_ai_state_component.h"
#include"character_ai_state/character_ai_state.h"
#include"mobilesuit_state/mobilesuit_state.h"
#include"../../actor/actor.h"


namespace MobileSuitAIStateComponentData
{
	//! ���B�n�_���e�덷
	static constexpr float ArrivalAllowableErrorSq = 100.0f;
}

MobileSuitAIStateComponent::MobileSuitAIStateComponent(Actor * owner)
	:MobileSuitStateComponent(owner)
	, m_CurrentCharacterAIState(nullptr)
	, m_NextNode(nullptr)
	, m_IsPathfinding(false)
	, m_TargetNodeIndex(0)
{
}

MobileSuitAIStateComponent::~MobileSuitAIStateComponent()
{
}

void MobileSuitAIStateComponent::ChangeCharacterAIState(const std::string & name)
{
	//���݂̏�Ԃ𔲂���
	if (m_CurrentCharacterAIState)
	{
		m_CurrentCharacterAIState->OnExit();
	}

	//�V������Ԃ�A�z�z�񂩂�T��
	const auto& iter = m_CharacterAIStateMap.find(name);
	if (iter == m_CharacterAIStateMap.end())
	{
		// ��ԓo�^
		m_CurrentCharacterAIState = nullptr;
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::ChangeCharacterAIState �o�^�Ȃ�");
		return;
	}
	else
	{
		// ��ԓo�^
		m_CurrentCharacterAIState = iter->second;

		//�V������Ԃɓ���
		if (m_CurrentCharacterAIState)
		{
			m_CurrentCharacterAIState->OnEnter();
		}
	}
}

void MobileSuitAIStateComponent::Init()
{
	// ����������
	for (auto&& state : m_CharacterAIStateMap)
	{
		if (state.second == nullptr)
		{
			continue;
		}

		state.second->Init();
	}

	MobileSuitStateComponent::Init();
}

void MobileSuitAIStateComponent::Uninit()
{
	//�o�^����Ă���X�e�[�g�}�b�v���폜
	for (auto&& iter = m_CharacterAIStateMap.begin(); iter != m_CharacterAIStateMap.end(); iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}
	m_CharacterAIStateMap.clear();

	MobileSuitStateComponent::Uninit();
}

void MobileSuitAIStateComponent::ProcessInput()
{
	// �������Ȃ�
	if (m_IsMoveFlag == false)
	{
		return;
	}

	// AI�𓮂���
	if (m_CurrentCharacterAIState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::ProcessInput m_CurrentCharacterAIState��nullptr");
		return;
	}

	m_CurrentCharacterAIState->ProcessInput();

	// ���r���X�[�c�̏�Ԃ̂ӂ�܂����s��
	if (m_CurrentMobileSuitState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::ProcessInput m_CurrentMobileSuitState��nullptr");
		return;
	}

	m_CurrentMobileSuitState->ProcessInput();
}

void MobileSuitAIStateComponent::Update()
{
	// �������Ȃ�
	if (m_IsMoveFlag == false)
	{
		return;
	}

	// AI�𓮂���
	if (m_CurrentCharacterAIState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::Update m_CurrentCharacterAIState��nullptr");
		return;
	}

	m_CurrentCharacterAIState->Update();

	// ���r���X�[�c�̏�Ԃ̂ӂ�܂����s��
	if (m_CurrentMobileSuitState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateComponent::Update m_CurrentMobileSuitState��nullptr");
		return;
	}
	m_CurrentMobileSuitState->Update();

	// �ړI�n�ɓ������Ă��邩�ǂ����𔻒f
	IsArrivalTarget();

	// ���͂�����
	CleanInput();
}

void MobileSuitAIStateComponent::CleanInput()
{
	// �g���K�[���͗p�ϐ�
	for (int i = 0; i < INPUT_MAX; i++)
	{
		m_IsTriggerInput[i] = false;
		m_IsPressInput[i] = false;
		m_InputDirection = Conv_XM::Vector2f(0.0f, 0.0f);
	}
}

void MobileSuitAIStateComponent::IsArrivalTarget()
{
	// �������
	Conv_XM::Vector3f dif = m_NextNode->Position - m_Owner->GetPosition();

	// ������2���}��
	float lenSq = Conv_XM::Vector3f::LengthSq(dif);

	// �������Ă�����
	if (lenSq < MobileSuitAIStateComponentData::ArrivalAllowableErrorSq)
	{
		// �C���f�b�N�X�J�E���g���C���N�������g����
		m_TargetNodeIndex++;

		// ���[�g�̃C���f�b�N�X�𒴂��Ă��Ȃ�������
		if (m_TargetNodeIndex < m_AStarRouteMap.size())
		{
			m_NextNode = m_AStarRouteMap[m_TargetNodeIndex];
		}
		else// �����Ă�����
		{
			// �T���ς݃t���O���I�t�ɂ���
			m_IsPathfinding = false;
		}
	}
}

void MobileSuitAIStateComponent::Pathfinding(const Conv_XM::Vector3f & target)
{
	if (m_Owner == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::Pathfinding m_Owner��nullptr");
		return;
	}

	Conv_XM::Vector3f myPos = m_Owner->GetPosition();

	// �����̂���m�[�h��T��
	int startNodeIndex = FindAStarNearNode(m_WeightedGraph, myPos);

	// �X�^�[�g�̃m�[�h
	WeightedGraphNode* startNode = m_WeightedGraph.Nodes[startNodeIndex];

	// �S�[���̃m�[�h��T��
	int goalNodeIndex = FindAStarNearNode(m_WeightedGraph, target);

	// �S�[���̃m�[�h
	if (startNodeIndex == goalNodeIndex)// �X�^�[�g�̃m�[�h�ƃS�[���̃m�[�h������̏ꍇ
	{
		// �ʂ̏ꏊ�ɍs���悤�ɂ���
		if (goalNodeIndex < m_WeightedGraph.Nodes.size() - 1)
		{
			goalNodeIndex++;
		}
		else
		{
			goalNodeIndex--;
		}
	}
	WeightedGraphNode* goalNode = m_WeightedGraph.Nodes[goalNodeIndex];

	// �}�b�v��������
	m_AStarMap.clear();
	bool found = AStar(m_WeightedGraph, startNode, goalNode, m_AStarMap);

	// ���[�g��������
	m_AStarRouteMap.clear();
	if (found == true)
	{
		MakeAStarRoute(startNode, goalNode, m_AStarMap, m_AStarRouteMap);
	}
	else
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::Pathfinding ���[�g��������܂���");
		return;
	}

	// �C���f�b�N�X��������
	m_TargetNodeIndex = 0;

	// ���̖ړI�n��ݒ肷��
	m_NextNode = m_AStarRouteMap[m_TargetNodeIndex];

	// �T���ς݃t���O��ON�ɂ���
	m_IsPathfinding = true;
}
