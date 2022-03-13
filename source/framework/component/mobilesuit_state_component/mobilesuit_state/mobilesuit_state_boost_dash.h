/**
 * @file state_boost_dash.h
 * @brief ブーストダッシュ状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief ブーストダッシュ状態クラス
 */
class MobileSuitStateBoostDash : public MobileSuitState
{
private:
	//! ブーストダッシュ速度
	float m_BoostDashSpeed;

	//! 一回の入力におけるブーストの時間
	float m_BoostTime;

	//! フレームカウント
	int m_FrameCount;

	//! ジャンプ入力を押し続けているかどうか
	bool m_IsPressJump;

	//! 移動入力を押し続けているかどうか
	bool m_IsPressMove;
public:
	MobileSuitStateBoostDash(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBoostDash()override{}

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
		return "MobileSuitStateBoostDash";
	}
};