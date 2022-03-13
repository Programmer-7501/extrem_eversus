/**
 * @file mobilesuit_state_beamrifle_shot.h
 * @brief ビームライフルショット状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief ビームライフルショット状態クラス
 * @details 現在はブーストダッシュキャンセルを実装
 */
class MobileSuitStateBeamRifleShot : public MobileSuitState
{
private:
	//! 歩く速さ
	float m_WalkSpeed;

	//! ブーストダッシュ速度
	float m_BoostDashInitialSpeed;

	//! ブーストダッシュ入力猶予
	int m_BoostDashCount;

	//! フレームカウント
	int m_FrameCount;

	//! ショットを撃ったかどうか
	bool m_IsShotFlag;

	//! エネミー001のポインタ
	class MobileSuit* m_EnemyMobileSuit001;
	//! エネミー002のポインタ
	class MobileSuit* m_EnemyMobileSuit002;
	//! ターゲットナンバー
	int m_TargetNum;
	//! ビームライフルの速度
	Conv_XM::Vector3f m_BeamVelocity;
public:
	MobileSuitStateBeamRifleShot(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBeamRifleShot()override{}

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
		return "MobileSuitStateBeamRifleShot";
	}
};