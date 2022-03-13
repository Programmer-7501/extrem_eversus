/**
 * @file mobilesuit_state_air_idle.h
 * @brief �󒆑ҋ@���
 */

#pragma once

#include"mobilesuit_state.h"

 /**
 * @brief �󒆑ҋ@��ԃN���X
 */
class MobileSuitStateAirIdle : public MobileSuitState
{
private:
	//! �W�����v�̑���
	float m_JumpSpeed;
public:
	MobileSuitStateAirIdle(class MobileSuitStateComponent* ownerComponent);

	~MobileSuitStateAirIdle()override{}

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
		return "MobileSuitStateAirIdle";
	}
};

