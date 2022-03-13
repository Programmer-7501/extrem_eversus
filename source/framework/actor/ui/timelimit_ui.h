/**
 * @file timelimit_ui.h
 * @brief �^�C�����~�b�gUI
 */

#pragma once

#include"../actor.h"
#include<chrono>

namespace TimeLimitUIData
{
	//! �����̌���
	static constexpr int k_Digit = 5;
}

 /**
 * @brief �^�C�����~�b�gUI�N���X
 */
class TimeLimitUI : public Actor
{
protected:
	//! �����\���p�X�v���C�g
	class NumberSpriteComponent* m_NumberSpriteComponent[TimeLimitUIData::k_Digit];

	//! �ő�̎���
	float m_MaxTime;

	//! �\������
	float m_TimeLeft;

	//! �v���J�n����
	std::chrono::system_clock::time_point m_Start;

	//! �v���J�n�t���O
	bool m_IsMesureStart;
public:
	TimeLimitUI();
	~TimeLimitUI();

	inline void SetMesureStartFlag(bool flag) { m_IsMesureStart = flag; }

	inline float GetTimeLeft() { return m_TimeLeft; }
protected:

	/**
	* @brief �u�[�X�gUI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �u�[�X�gUI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �u�[�X�gUI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};