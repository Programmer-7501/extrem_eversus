/**
 * @file state_boost_jump.h
 * @brief �u�[�X�g�W�����v���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �u�[�X�g�W�����v��ԃN���X
 */
class MobileSuitStateBoostJump : public MobileSuitState
{
private:
	//! �u�[�X�g�W�����v���x
	float m_BoostJumpSpeed;

	//! �J�E���g�t���[��
	int m_CountFrame;

	//! �u�[�X�g�_�b�V���̑���
	float m_BoostDashSpeed;

	//! �u�[�X�g�_�b�V�����̃u�[�X�g�g�p��
	float m_BoostDashUseBoostNum;

	//! �W�����v�{�^�������������Ă��邩�ǂ���
	bool m_IsPressJumpButton;
public:
	MobileSuitStateBoostJump(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBoostJump()override{}

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
		return "MobileSuitStateBoostJump";
	}
};