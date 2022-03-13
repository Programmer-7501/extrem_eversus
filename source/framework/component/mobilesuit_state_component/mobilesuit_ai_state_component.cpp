
#include"../../../system/main.h"
#include"mobilesuit_ai_state_component.h"
#include"character_ai_state/character_ai_state.h"
#include"mobilesuit_state/mobilesuit_state.h"
#include"../../actor/actor.h"


namespace MobileSuitAIStateComponentData
{
	//! 到達地点許容誤差
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
	//現在の状態を抜ける
	if (m_CurrentCharacterAIState)
	{
		m_CurrentCharacterAIState->OnExit();
	}

	//新しい状態を連想配列から探す
	const auto& iter = m_CharacterAIStateMap.find(name);
	if (iter == m_CharacterAIStateMap.end())
	{
		// 状態登録
		m_CurrentCharacterAIState = nullptr;
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::ChangeCharacterAIState 登録なし");
		return;
	}
	else
	{
		// 状態登録
		m_CurrentCharacterAIState = iter->second;

		//新しい状態に入る
		if (m_CurrentCharacterAIState)
		{
			m_CurrentCharacterAIState->OnEnter();
		}
	}
}

void MobileSuitAIStateComponent::Init()
{
	// 初期化処理
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
	//登録されているステートマップを削除
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
	// 動かさない
	if (m_IsMoveFlag == false)
	{
		return;
	}

	// AIを動かす
	if (m_CurrentCharacterAIState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::ProcessInput m_CurrentCharacterAIStateがnullptr");
		return;
	}

	m_CurrentCharacterAIState->ProcessInput();

	// モビルスーツの状態のふるまいを行う
	if (m_CurrentMobileSuitState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::ProcessInput m_CurrentMobileSuitStateがnullptr");
		return;
	}

	m_CurrentMobileSuitState->ProcessInput();
}

void MobileSuitAIStateComponent::Update()
{
	// 動かさない
	if (m_IsMoveFlag == false)
	{
		return;
	}

	// AIを動かす
	if (m_CurrentCharacterAIState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::Update m_CurrentCharacterAIStateがnullptr");
		return;
	}

	m_CurrentCharacterAIState->Update();

	// モビルスーツの状態のふるまいを行う
	if (m_CurrentMobileSuitState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateComponent::Update m_CurrentMobileSuitStateがnullptr");
		return;
	}
	m_CurrentMobileSuitState->Update();

	// 目的地に到着しているかどうかを判断
	IsArrivalTarget();

	// 入力を消す
	CleanInput();
}

void MobileSuitAIStateComponent::CleanInput()
{
	// トリガー入力用変数
	for (int i = 0; i < INPUT_MAX; i++)
	{
		m_IsTriggerInput[i] = false;
		m_IsPressInput[i] = false;
		m_InputDirection = Conv_XM::Vector2f(0.0f, 0.0f);
	}
}

void MobileSuitAIStateComponent::IsArrivalTarget()
{
	// 差を取る
	Conv_XM::Vector3f dif = m_NextNode->Position - m_Owner->GetPosition();

	// 距離の2乗を図る
	float lenSq = Conv_XM::Vector3f::LengthSq(dif);

	// 到着していたら
	if (lenSq < MobileSuitAIStateComponentData::ArrivalAllowableErrorSq)
	{
		// インデックスカウントをインクリメントする
		m_TargetNodeIndex++;

		// ルートのインデックスを超えていなかったら
		if (m_TargetNodeIndex < m_AStarRouteMap.size())
		{
			m_NextNode = m_AStarRouteMap[m_TargetNodeIndex];
		}
		else// 超えていたら
		{
			// 探索済みフラグをオフにする
			m_IsPathfinding = false;
		}
	}
}

void MobileSuitAIStateComponent::Pathfinding(const Conv_XM::Vector3f & target)
{
	if (m_Owner == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::Pathfinding m_Ownerがnullptr");
		return;
	}

	Conv_XM::Vector3f myPos = m_Owner->GetPosition();

	// 自分のいるノードを探す
	int startNodeIndex = FindAStarNearNode(m_WeightedGraph, myPos);

	// スタートのノード
	WeightedGraphNode* startNode = m_WeightedGraph.Nodes[startNodeIndex];

	// ゴールのノードを探す
	int goalNodeIndex = FindAStarNearNode(m_WeightedGraph, target);

	// ゴールのノード
	if (startNodeIndex == goalNodeIndex)// スタートのノードとゴールのノードが同一の場合
	{
		// 別の場所に行くようにする
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

	// マップを初期化
	m_AStarMap.clear();
	bool found = AStar(m_WeightedGraph, startNode, goalNode, m_AStarMap);

	// ルートを初期化
	m_AStarRouteMap.clear();
	if (found == true)
	{
		MakeAStarRoute(startNode, goalNode, m_AStarMap, m_AStarRouteMap);
	}
	else
	{
		Logger::GetInstance().SetLog("MobileSuitAIStateComponent::Pathfinding ルートが見つかりません");
		return;
	}

	// インデックスを初期化
	m_TargetNodeIndex = 0;

	// 次の目的地を設定する
	m_NextNode = m_AStarRouteMap[m_TargetNodeIndex];

	// 探索済みフラグをONにする
	m_IsPathfinding = true;
}
