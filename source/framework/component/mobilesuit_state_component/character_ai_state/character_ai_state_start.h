/**
 * @file charcter_ai_state_start.h
 * @brief キャラクターAIステート開始
 */

#pragma once

#include"character_ai_state.h"

 /**
 * @brief キャラクターAIステート開始クラス
 */
class CharcterAIStateStart : public CharcterAIState
{
private:
	int m_FrameCount;
public:
	CharcterAIStateStart(class MobileSuitAIStateComponent* ownerComponent);

	virtual ~CharcterAIStateStart() {}

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
		return "CharcterAIStateStart";
	}
};

