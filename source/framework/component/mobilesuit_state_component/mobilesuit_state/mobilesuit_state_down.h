/**
 * @file mobilesuit_state_down.h
 * @brief ダウン状態
 */

#pragma once

#include"../mobilesuit_state/mobilesuit_state.h"

 /**
 * @brief ダウン状態クラス
 */
class MobileSuitStateDown : public MobileSuitState
{
private:
	//! カウント
	float m_CountFrame;

public:
	MobileSuitStateDown(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateDown()override {}

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
		return "MobileSuitStateDown";
	}
};

