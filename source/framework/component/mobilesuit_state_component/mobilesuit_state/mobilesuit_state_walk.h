/**
 * @file mobilesuit_state_walk.h
 * @brief 歩き状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief 歩き状態クラス
 */
class MobileSuitStateWalk : public MobileSuitState
{
private:
	//! 歩く速さ
	float m_WalkSpeed;

	//! ジャンプの速さ
	float m_JumpSpeed;

	//! フレームカウント
	int m_FrameCount;
public:
	MobileSuitStateWalk(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateWalk()override{}

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
		return "MobileSuitStateWalk";
	}
};

