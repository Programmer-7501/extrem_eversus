/**
 * @file mobilesuit_state_landing_recovery.h
 * @brief 着地硬直状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief 歩き状態クラス
 */
class MobileSuitStateNKakutouFirst : public MobileSuitState
{
private:
	//! フレームカウント
	int m_CountFrame;

	//! 攻撃ブースト速度
	float m_BoostSpeed;

	//! 攻撃ブースト使用量
	float m_UseBoostNum;

	//! 次の格闘に行くかどうか
	bool m_IsGoNextKakutou;
public:
	MobileSuitStateNKakutouFirst(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateNKakutouFirst()override {}

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
		return "MobileSuitStateNKakutouFirst";
	}
private:
	/**
	* @brief エネミーに自機を向ける
	*/
	void RotateToEnemy();
};