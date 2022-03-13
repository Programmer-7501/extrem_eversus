/**
 * @file mobilesuit_ai_state_component.h
 * @brief モビルスーツAIステートコンポーネント
 */

#pragma once

#include"mobilesuit_state_component.h"

#include"../../../ai/astar.h"

 /**
 * @brief モビルスーツAIステートコンポーネント
 */
class MobileSuitAIStateComponent : public MobileSuitStateComponent
{
protected:
	//! キャラクターAIステートマップ
	std::unordered_map<std::string, class CharcterAIState*> m_CharacterAIStateMap;

	//! 現在の状態
	class CharcterAIState* m_CurrentCharacterAIState;

	// ノードグラフ
	WeightedGraph m_WeightedGraph;

	//! 経路マップ
	AStarMap m_AStarMap;

	//! ルートマップ
	AStarRouteMap m_AStarRouteMap;

	//! 次に向かうノード
	WeightedGraphNode* m_NextNode;

	//! 経路を探索済みかどうか
	bool m_IsPathfinding;

	//! ベクター配列の保管用インデックス
	int m_TargetNodeIndex;
public:
	MobileSuitAIStateComponent(class Actor* owner);
	~MobileSuitAIStateComponent();

	/**
	* @brief キャラクターAIの状態変更
	* @param[in] const std::string&(name) 状態名
	*/
	void ChangeCharacterAIState(const std::string& name);

	/**
	* @brief キャラクターAIステートクリエイト関数
	*/
	template <typename T>
	void CreateCharacterState()
	{
		//作成
		T* state = new T(this);

		if (state == nullptr)
		{
			Logger::GetInstance().SetLog("CreateCharacterState stateがnullptr");
			return;
		}

		const auto& iter = m_CharacterAIStateMap.find(state->GetName());

		//重複していないかチェック
		if (iter != m_CharacterAIStateMap.end())
		{
			//重複していたら削除
			delete state;
			state = nullptr;
		}
		else
		{
			//登録
			m_CharacterAIStateMap.emplace(state->GetName(), state);
		}
	}

	inline void SetWeightedGraph(WeightedGraph weightedGraph) { m_WeightedGraph = weightedGraph; }

	inline bool GetIsPathfinding() { return m_IsPathfinding; }

	WeightedGraphNode* GetNextNode() { return m_NextNode; }

	inline class CharcterAIState* GetCharcterAIState() { return m_CurrentCharacterAIState; }

	/**
	* @brief 経路探索
	* @param[in] const Conv_XM::Vector3f(target) 目的地
	*/
	void Pathfinding(const Conv_XM::Vector3f& target);
protected:

	/**
	* @brief 初期化処理
	*/
	virtual void Init()override;

	/**
	* @brief 終了処理
	*/
	virtual void Uninit()override;

	/**
	* @brief 入力処理
	*/
	virtual void ProcessInput()override;

	/**
	* @brief 更新処理
	*/
	virtual void Update()override;

	/**
	* @brief 入力を空にする
	*/
	void CleanInput();

	/**
	* @brief 目的地についたかどうかを判別する
	*/
	void IsArrivalTarget();
};