/**
 * @file mobilesuit_state_return.h
 * @brief �񕜏��
 */

#pragma once

#include"../mobilesuit_state/mobilesuit_state.h"

 /**
 * @brief �ҋ@��ԃN���X
 */
class MobileSuitStateReturn : public MobileSuitState
{
private:
	//! �J�E���g
	float m_CountFrame;

public:
	MobileSuitStateReturn(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateReturn()override {}

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
		return "MobileSuitStateReturn";
	}
};

