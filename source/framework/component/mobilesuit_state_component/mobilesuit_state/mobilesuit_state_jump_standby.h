/**
 * @file mobilesuit_state_jump_standby.h
 * @brief �W�����v�ҋ@���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �ҋ@��ԃN���X
 */
class MobileSuitStateJumpStandby : public MobileSuitState
{
private:
	//! �t���[���J�E���g
	int m_CountFrame;

	//! �u�[�X�g�{�^�������������Ă��邩�ǂ���
	bool m_IsPressJumpButton;

	//! �W�����v�̑��x
	float m_BoostJumpSpeed;

	//! �u�[�X�g�_�b�V���Ŏg�p�����u�[�X�g��
	float m_BoostJumpUseBoostNum;

	//! �u�[�X�g�_�b�V���̏���
	float m_BoostDashSpeed;

	//! �u�[�X�g�_�b�V���Ŏg�p�����u�[�X�g��
	float m_BoostDashUseBoostNum;

public:
	MobileSuitStateJumpStandby(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateJumpStandby()override{}

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
		return "MobileSuitStateJumpStandby";
	}
};

