/**
 * @file mobilesuit_state_landing_recovery.h
 * @brief ���n�d�����
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief ������ԃN���X
 */
class MobileSuitStateNKakutouFirst : public MobileSuitState
{
private:
	//! �t���[���J�E���g
	int m_CountFrame;

	//! �U���u�[�X�g���x
	float m_BoostSpeed;

	//! �U���u�[�X�g�g�p��
	float m_UseBoostNum;

	//! ���̊i���ɍs�����ǂ���
	bool m_IsGoNextKakutou;
public:
	MobileSuitStateNKakutouFirst(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateNKakutouFirst()override {}

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
		return "MobileSuitStateNKakutouFirst";
	}
private:
	/**
	* @brief �G�l�~�[�Ɏ��@��������
	*/
	void RotateToEnemy();
};