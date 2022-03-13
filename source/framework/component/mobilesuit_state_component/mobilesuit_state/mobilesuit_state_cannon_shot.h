/**
 * @file mobilesuit_state_cannon_shot.h
 * @brief キャノンショット状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief キャノンショット状態クラス
 * @details 現在はブーストダッシュキャンセルを実装
 */
class MobileSuitStateCannonShot : public MobileSuitState
{
private:
	//! ブーストダッシュ速度
	float m_BoostDashInitialSpeed;

	//! ブーストダッシュ入力猶予
	int m_BoostDashCount;

	//! フレームカウント
	int m_FrameCount;

	//! ショットを撃ったかどうか
	bool m_IsShotFlag;

	//! キャノンの速度
	Conv_XM::Vector3f m_CannonVelocity;

	//! 使用するキャノンのポインタ
	class Bullet* m_CannonBullet;
public:
	MobileSuitStateCannonShot(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateCannonShot()override{}

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
		return "MobileSuitStateCannonShot";
	}

private:

	/**
	* @brief エネミーに時期を向ける
	*/
	void RotateToEnemy();


	/**
	* @brief まっすぐに向ける
	*/
	void RotateToForward();
};