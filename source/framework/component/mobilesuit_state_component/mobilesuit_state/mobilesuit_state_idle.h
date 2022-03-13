/**
 * @file state_idle.h
 * @brief �ҋ@���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �ҋ@��ԃN���X
 */
class MobileSuitStateIdle : public MobileSuitState
{
private:
	//! �W�����v�̑���
	float m_JumpSpeed;

public:
	MobileSuitStateIdle(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateIdle()override{}

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
		return "MobileSuitStateIdle";
	}
};

