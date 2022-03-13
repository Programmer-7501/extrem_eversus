/**
 * @file charcter_ai_state_move.h
 * @brief キャラクターAIステート移動
 */

#pragma once

#include"character_ai_state.h"

 /**
 * @brief キャラクターAIステート移動クラス
 */
class CharcterAIStateMove : public CharcterAIState
{
private:
	//! フレームカウント
	int m_FrameCount;
	
	//! 経路探索用カウント
	int m_PathfindingTime;

public:
	CharcterAIStateMove(class MobileSuitAIStateComponent* ownerComponent);

	virtual ~CharcterAIStateMove() {}

	/**
	* @brief 初期化処理
	*/
	virtual void Init()override;

	/**
	* @brief 入力処理
	*/
	virtual void ProcessInput()override;

	/**
	* @brief 更新処理
	*/
	virtual void Update()override;

	/**
	* @brief 状態に入った時の処理
	*/
	virtual void OnEnter()override;

	/**
	* @brief 状態から出るときの処理
	*/
	virtual void OnExit()override;

	/**
	* @brief 状態名
	*/
	const char* GetName() const override
	{
		return "CharcterAIStateMove";
	}
};

