/**
 * @file charcter_ai_state_wander_around.h
 * @brief キャラクターAIステートうろうろする
 */

#pragma once

#include"character_ai_state.h"

 /**
 * @brief キャラクターAIステートうろうろするクラス
 */
class CharcterAIStateWanderAround : public CharcterAIState
{
public:
	CharcterAIStateWanderAround(class MobileSuitAIStateComponent* ownerComponent);

	virtual ~CharcterAIStateWanderAround() {}

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
		return "CharcterAIStateWanderAround";
	}
};

