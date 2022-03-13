/**
 * @file mobilesuit_state_landing_recovery.h
 * @brief 着地硬直状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief 歩き状態クラス
 */
class MobileSuitStateLandingRecovery : public MobileSuitState
{
private:
	//! フレームカウント
	int m_CountFrame;

	//! 硬直フレーム
	int m_RecoveryFrame;
public:
	MobileSuitStateLandingRecovery(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateLandingRecovery()override{}

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
		return "MobileSuitStateLandingRecovery";
	}
};