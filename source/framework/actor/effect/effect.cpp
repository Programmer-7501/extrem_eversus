
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
	// �J�E���g�A�b�v
	m_FrameCount++;

	if (m_FrameCount < m_FinishFrame)
	{
		return;
	}

	switch (m_EffectMode)
	{
	case Effect::EFFECTMODE_ONCE:
		// ��A�N�e�B�u��Ԃɂ���
		SetActive(false);
		break;
	case Effect::EFFECTMODE_REPETITION:
		// �t���[���J�E���g������������
		m_FrameCount = 0;
		break;
	case Effect::EFFECTMODE_MAX:
		break;
	default:
		break;
	}
}
