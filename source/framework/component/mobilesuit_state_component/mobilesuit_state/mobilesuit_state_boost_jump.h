/**
 * @file state_boost_jump.h
 * @brief ブーストジャンプ状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief ブーストジャンプ状態クラス
 */
class MobileSuitStateBoostJump : public MobileSuitState
{
private:
	//! ブーストジャンプ速度
	float m_BoostJumpSpeed;

	//! カウントフレーム
	int m_CountFrame;

	//! ブーストダッシュの速さ
	float m_BoostDashSpeed;

	//! ブーストダッシュ時のブースト使用量
	float m_BoostDashUseBoostNum;

	//! ジャンプボタンを押し続けているかどうか
	bool m_IsPressJumpButton;
public:
	MobileSuitStateBoostJump(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBoostJump()override{}

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
		return "MobileSuitStateBoostJump";
	}
};