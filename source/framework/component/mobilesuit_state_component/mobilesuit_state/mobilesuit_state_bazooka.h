/**
 * @file mobilesuit_state_bazooka.h
 * @brief �o�Y�[�J���ˏ��
 */

#pragma once

#include"../mobilesuit_state/mobilesuit_state.h"

 /**
 * @brief �o�Y�[�J���ˏ�ԃN���X
 */
class MobileSuitStateBazooka : public MobileSuitState
{
private:
	//! �J�E���g
	float m_CountFrame;

	//! �W�����v�{�^�������������ǂ���
	bool m_IsPushJumpButton;

	//! �u�[�X�g�_�b�V���܂ł̓��͗\������
	int m_BoostDashCount;

	//! �u�[�X�g�_�b�V���̏���
	float m_BoostDashInitialSpeed;

	//! �V���b�g�����������ǂ���
	bool m_IsShotFlag;

	//! �e��
	float m_BazookaSpeed;
public:
	MobileSuitStateBazooka(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBazooka()override {}

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
		return "MobileSuitStateBazooka";
	}
};

