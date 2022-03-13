/**
 * @file mobilesuit_state_walk.h
 * @brief �������
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief ������ԃN���X
 */
class MobileSuitStateWalk : public MobileSuitState
{
private:
	//! ��������
	float m_WalkSpeed;

	//! �W�����v�̑���
	float m_JumpSpeed;

	//! �t���[���J�E���g
	int m_FrameCount;
public:
	MobileSuitStateWalk(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateWalk()override{}

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
		return "MobileSuitStateWalk";
	}
};

