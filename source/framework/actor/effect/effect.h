/**
 * @file effect.h
 * @brief エフェクト
 */

#pragma once

#include"../actor.h"

 /**
 * @brief エフェクトクラス
 */
class Effect : public Actor
{
public:
	enum EffectMode
	{
		// 一回きり
		EFFECTMODE_ONCE,
		// 繰り返し
		EFFECTMODE_REPETITION,
		EFFECTMODE_MAX,
	};
protected:
	//! フレームカウント
	int m_FrameCount;

	//! 終了フレーム
	int m_FinishFrame;

	//! エフェクトタイプ
	EffectMode m_EffectMode;
public:
	Effect();
	virtual ~Effect() override;

	inline void SetFinishFrame(int finishFrame) { m_FinishFrame = finishFrame; }

protected:

	/**
	* @brief v独自の更新処理
	*/
	virtual void UpdateActor()override;
};