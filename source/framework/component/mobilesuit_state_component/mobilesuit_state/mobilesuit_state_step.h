/**
 * @file mobilesuit_state_step.h
 * @brief �X�e�b�v���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �ҋ@��ԃN���X
 */
class MobileSuitStateStep : public MobileSuitState
{
private:
	//! �t���[���J�E���g
	int m_FrameCount;

public:
	MobileSuitStateStep(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateStep()override{}

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
		return "MobileSuitStateStep";
	}
};

