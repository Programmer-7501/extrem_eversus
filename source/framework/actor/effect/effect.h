/**
 * @file effect.h
 * @brief �G�t�F�N�g
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �G�t�F�N�g�N���X
 */
class Effect : public Actor
{
public:
	enum EffectMode
	{
		// ��񂫂�
		EFFECTMODE_ONCE,
		// �J��Ԃ�
		EFFECTMODE_REPETITION,
		EFFECTMODE_MAX,
	};
protected:
	//! �t���[���J�E���g
	int m_FrameCount;

	//! �I���t���[��
	int m_FinishFrame;

	//! �G�t�F�N�g�^�C�v
	EffectMode m_EffectMode;
public:
	Effect();
	virtual ~Effect() override;

	inline void SetFinishFrame(int finishFrame) { m_FinishFrame = finishFrame; }

protected:

	/**
	* @brief v�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};