/**
 * @file state_idle.h
 * @brief 待機状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief 待機状態クラス
 */
class MobileSuitStateIdle : public MobileSuitState
{
private:
	//! ジャンプの速さ
	float m_JumpSpeed;

public:
	MobileSuitStateIdle(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateIdle()override{}

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
		return "MobileSuitStateIdle";
	}
};

