#pragma once

/**
 * @file meta_ai.h
 * @brief メタAI
 */

#pragma once

#include"../../actor.h"
#include"../../../../ai/astar.h"

 /**
 * @brief メタAIクラス
 */
class MetaAI : public Actor
{
public:
	enum Character
	{
		// 自分
		PLAYER,
		// 味方
		ALLY,
		// 敵001
		ENEMY001,
		// 敵002
		ENEMY002,
		// 最大数
		CHARMAX
	};
public:

	//! ノード格納用
	WeightedGraph m_WeightGraph;

	//! 経路マップ
	AStarMap m_AStarMap[CHARMAX];

	//! ルートマップ
	AStarRouteMap m_AStarRouteMap[CHARMAX];

	// 次に向かうノード
	WeightedGraphNode* m_NextNode[CHARMAX];

	//! モビルスーツ
	class MobileSuit* m_MobileSuits[CHARMAX];

	//! メタAIステートマップ
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
	* @brief メタAI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief メタAI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief メタAI独自の更新処理
	*/
	virtual void UpdateActor()override;

private:

	/**
	* @brief ルートを決定する
	*/
	void DecideRoute();
};