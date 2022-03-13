/**
 * @file mobilesuit_state_return.h
 * @brief 回復状態
 */

#pragma once

#include"../mobilesuit_state/mobilesuit_state.h"

 /**
 * @brief 待機状態クラス
 */
class MobileSuitStateReturn : public MobileSuitState
{
private:
	//! カウント
	float m_CountFrame;

public:
	MobileSuitStateReturn(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateReturn()override {}

	/**
	* @brief 初期化処理
	*/
	void Init()override;

	/**
	* @brief 入力処理
	*/
	void ProcessInput()override;

	/**
	* @brief 更新処理
	*/
	void Update()override;

	/**
	* @brief 状態に入った時の処理
	*/
	void OnEnter()override;

	/**
	* @brief 状態から出るときの処理
	*/
	void OnExit()override;

	/**
	* @brief 状態名
	*/
	const char* GetName() const override
	{
		return "MobileSuitStateReturn";
	}
};

