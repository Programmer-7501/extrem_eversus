/**
 * @file mobilesuit_state_jump_standby.h
 * @brief ジャンプ待機状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief 待機状態クラス
 */
class MobileSuitStateJumpStandby : public MobileSuitState
{
private:
	//! フレームカウント
	int m_CountFrame;

	//! ブーストボタンを押し続けているかどうか
	bool m_IsPressJumpButton;

	//! ジャンプの速度
	float m_BoostJumpSpeed;

	//! ブーストダッシュで使用されるブースト量
	float m_BoostJumpUseBoostNum;

	//! ブーストダッシュの初速
	float m_BoostDashSpeed;

	//! ブーストダッシュで使用されるブースト量
	float m_BoostDashUseBoostNum;

public:
	MobileSuitStateJumpStandby(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateJumpStandby()override{}

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
		return "MobileSuitStateJumpStandby";
	}
};

