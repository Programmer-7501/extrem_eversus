
#include"../../../system/main.h"
#include "effect.h"


Effect::Effect()
	:m_FrameCount(0)
{
}

Effect::~Effect()
{
}

void Effect::UpdateActor()
{
	// カウントアップ
	m_FrameCount++;

	if (m_FrameCount < m_FinishFrame)
	{
		return;
	}

	switch (m_EffectMode)
	{
	case Effect::EFFECTMODE_ONCE:
		// 非アクティブ状態にする
		SetActive(false);
		break;
	case Effect::EFFECTMODE_REPETITION:
		// フレームカウントを初期化する
		m_FrameCount = 0;
		break;
	case Effect::EFFECTMODE_MAX:
		break;
	default:
		break;
	}
}
