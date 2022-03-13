/**
 * @file mobilesuit_state_bend_backward.h
 * @brief のけ反り状態
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief のけ反り状態クラス
 */
class MobileSuitStateBendBackWard : public MobileSuitState
{
private:
	//! フレームカウント
	int m_CountFrame;
public:
	MobileSuitStateBendBackWard(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBendBackWard()override{}

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
		return "MobileSuitStateBendBackWard";
	}
};


