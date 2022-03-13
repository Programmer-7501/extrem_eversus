/**
 * @file mobilesuit_state_bend_backward.h
 * @brief �̂�������
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �̂������ԃN���X
 */
class MobileSuitStateBendBackWard : public MobileSuitState
{
private:
	//! �t���[���J�E���g
	int m_CountFrame;
public:
	MobileSuitStateBendBackWard(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBendBackWard()override{}

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
		return "MobileSuitStateBendBackWard";
	}
};


