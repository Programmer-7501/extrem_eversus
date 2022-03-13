/**
 * @file mobilesuit_state_beamrifle_shot.h
 * @brief �r�[�����C�t���V���b�g���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �r�[�����C�t���V���b�g��ԃN���X
 * @details ���݂̓u�[�X�g�_�b�V���L�����Z��������
 */
class MobileSuitStateBeamRifleShot : public MobileSuitState
{
private:
	//! ��������
	float m_WalkSpeed;

	//! �u�[�X�g�_�b�V�����x
	float m_BoostDashInitialSpeed;

	//! �u�[�X�g�_�b�V�����͗P�\
	int m_BoostDashCount;

	//! �t���[���J�E���g
	int m_FrameCount;

	//! �V���b�g�����������ǂ���
	bool m_IsShotFlag;

	//! �G�l�~�[001�̃|�C���^
	class MobileSuit* m_EnemyMobileSuit001;
	//! �G�l�~�[002�̃|�C���^
	class MobileSuit* m_EnemyMobileSuit002;
	//! �^�[�Q�b�g�i���o�[
	int m_TargetNum;
	//! �r�[�����C�t���̑��x
	Conv_XM::Vector3f m_BeamVelocity;
public:
	MobileSuitStateBeamRifleShot(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateBeamRifleShot()override{}

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
		return "MobileSuitStateBeamRifleShot";
	}
};