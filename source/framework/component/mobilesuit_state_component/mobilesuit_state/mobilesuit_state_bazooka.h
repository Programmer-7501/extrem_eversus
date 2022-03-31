/**
 * @file mobilesuit_state_bazooka.h
 * @brief バズーカ発射状態
 */

#pragma once

#include"../mobilesuit_state/mobilesuit_state.h"

 /**
 * @brief バズーカ発射状態クラス
 */
class MobileSuitStateBazooka : public MobileSuitState
{
private:
	//! カウント
	float m_CountFrame;

	//! ジャンプボタンを押したかどうか
	bool m_IsPushJumpButton;

	//! ブーストダッシュまでの入力予備時間
	int m_BoostDashCount;

	//! ブーストダッシュの初速
	float m_BoostDashInitialSpeed;

	//! ショットを撃ったかどうか
	bool m_IsShotFlag;

	//! 弾速
	float m_BazookaSpeed;
public:
	MobileSuitStateBazooka(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBazooka()override {}

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
		return "MobileSuitStateBazooka";
	}
};

