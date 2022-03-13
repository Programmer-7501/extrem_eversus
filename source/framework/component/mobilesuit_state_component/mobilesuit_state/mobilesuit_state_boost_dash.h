/**
 * @file state_boost_dash.h
 * @brief �u�[�X�g�_�b�V�����
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �u�[�X�g�_�b�V����ԃN���X
 */
class MobileSuitStateBoostDash : public MobileSuitState
{
private:
	//! �u�[�X�g�_�b�V�����x
	float m_BoostDashSpeed;

	//! ���̓��͂ɂ�����u�[�X�g�̎���
	float m_BoostTime;

	//! �t���[���J�E���g
	int m_FrameCount;

	//! �W�����v���͂����������Ă��邩�ǂ���
	bool m_IsPressJump;

	//! �ړ����͂����������Ă��邩�ǂ���
	bool m_IsPressMove;
public:
	MobileSuitStateBoostDash(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBoostDash()override{}

	/**
	* @brief ����������
	*/
	void Init()override;

	/**
	* @brief ���͏���
	*/
	void ProcessInput()override;

	/**
	* @brief �X�V����
	*/
	void Update()override;

	/**
	* @brief ��Ԃɓ��������̏���
	*/
	void OnEnter()override;

	/**
	* @brief ��Ԃ���o��Ƃ��̏���
	*/
	void OnExit()override;

	/**
	* @brief ��Ԗ�
	*/
	const char* GetName() const override
	{
		return "MobileSuitStateBoostDash";
	}
};