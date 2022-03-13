/**
 * @file mobilesuit_state_turn_around_beamrifle_shot.h
 * @brief �r�[�����C�t���U������V���b�g���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �r�[�����C�t���U������V���b�g��ԃN���X
 */
class MobileSuitStateTurnAroundBeamRifleShot : public MobileSuitState
{
private:
	//! �u�[�X�g�_�b�V�����x
	float m_BoostDashInitialSpeed;

	//! �u�[�X�g�_�b�V�����͗P�\
	int m_BoostDashCount;

	//! �t���[���J�E���g
	int m_FrameCount;
public:
	MobileSuitStateTurnAroundBeamRifleShot(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateTurnAroundBeamRifleShot()override{}

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
		return "MobileSuitStateTurnAroundBeamRifleShot";
	}
private:
	/**
	* @brief �G�l�~�[�Ɏ�����������
	*/
	void RotateToEnemy();

};