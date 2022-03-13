/**
 * @file mobilesuit_state_landing_recovery.h
 * @brief ���n�d�����
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief ������ԃN���X
 */
class MobileSuitStateLandingRecovery : public MobileSuitState
{
private:
	//! �t���[���J�E���g
	int m_CountFrame;

	//! �d���t���[��
	int m_RecoveryFrame;
public:
	MobileSuitStateLandingRecovery(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateLandingRecovery()override{}

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
		return "MobileSuitStateLandingRecovery";
	}
};