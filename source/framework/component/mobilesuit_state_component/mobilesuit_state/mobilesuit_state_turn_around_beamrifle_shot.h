/**
 * @file mobilesuit_state_turn_around_beamrifle_shot.h
 * @brief ビームライフル振り向きショット状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief ビームライフル振り向きショット状態クラス
 */
class MobileSuitStateTurnAroundBeamRifleShot : public MobileSuitState
{
private:
	//! ブーストダッシュ速度
	float m_BoostDashInitialSpeed;

	//! ブーストダッシュ入力猶予
	int m_BoostDashCount;

	//! フレームカウント
	int m_FrameCount;
public:
	MobileSuitStateTurnAroundBeamRifleShot(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateTurnAroundBeamRifleShot()override{}

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
		return "MobileSuitStateTurnAroundBeamRifleShot";
	}
private:
	/**
	* @brief エネミーに時期を向ける
	*/
	void RotateToEnemy();

};