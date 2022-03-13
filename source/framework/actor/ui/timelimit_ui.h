/**
 * @file timelimit_ui.h
 * @brief タイムリミットUI
 */

#pragma once

#include"../actor.h"
#include<chrono>

namespace TimeLimitUIData
{
	//! 数字の桁数
	static constexpr int k_Digit = 5;
}

 /**
 * @brief タイムリミットUIクラス
 */
class TimeLimitUI : public Actor
{
protected:
	//! 数字表示用スプライト
	class NumberSpriteComponent* m_NumberSpriteComponent[TimeLimitUIData::k_Digit];

	//! 最大の時間
	float m_MaxTime;

	//! 表示時間
	float m_TimeLeft;

	//! 計測開始時間
	std::chrono::system_clock::time_point m_Start;

	//! 計測開始フラグ
	bool m_IsMesureStart;
public:
	TimeLimitUI();
	~TimeLimitUI();

	inline void SetMesureStartFlag(bool flag) { m_IsMesureStart = flag; }

	inline float GetTimeLeft() { return m_TimeLeft; }
protected:

	/**
	* @brief ブーストUI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ブーストUI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ブーストUI独自の更新処理
	*/
	virtual void UpdateActor()override;
};