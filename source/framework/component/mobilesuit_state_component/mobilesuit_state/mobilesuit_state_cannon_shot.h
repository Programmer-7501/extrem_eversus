/**
 * @file mobilesuit_state_cannon_shot.h
 * @brief �L���m���V���b�g���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �L���m���V���b�g��ԃN���X
 * @details ���݂̓u�[�X�g�_�b�V���L�����Z��������
 */
class MobileSuitStateCannonShot : public MobileSuitState
{
private:
	//! �u�[�X�g�_�b�V�����x
	float m_BoostDashInitialSpeed;

	//! �u�[�X�g�_�b�V�����͗P�\
	int m_BoostDashCount;

	//! �t���[���J�E���g
	int m_FrameCount;

	//! �V���b�g�����������ǂ���
	bool m_IsShotFlag;

	//! �L���m���̑��x
	Conv_XM::Vector3f m_CannonVelocity;

	//! �g�p����L���m���̃|�C���^
	class Bullet* m_CannonBullet;
public:
	MobileSuitStateCannonShot(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateCannonShot()override{}

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
		return "MobileSuitStateCannonShot";
	}

private:

	/**
	* @brief �G�l�~�[�Ɏ�����������
	*/
	void RotateToEnemy();


	/**
	* @brief �܂������Ɍ�����
	*/
	void RotateToForward();
};