/**
 * @file mobilesuit_state_down.h
 * @brief �_�E�����
 */

#pragma once

#include"../mobilesuit_state/mobilesuit_state.h"

 /**
 * @brief �_�E����ԃN���X
 */
class MobileSuitStateDown : public MobileSuitState
{
private:
	//! �J�E���g
	float m_CountFrame;

public:
	MobileSuitStateDown(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateDown()override {}

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
		return "MobileSuitStateDown";
	}
};

