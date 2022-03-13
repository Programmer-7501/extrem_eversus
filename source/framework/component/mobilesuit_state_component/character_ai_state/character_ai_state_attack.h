/**
 * @file charcter_ai_state_attack.h
 * @brief �L�����N�^�[AI�X�e�[�g�U��
 */

#pragma once

#include"character_ai_state.h"

 /**
 * @brief �L�����N�^�[AI�X�e�[�g�U���N���X
 */
class CharcterAIStateAttack : public CharcterAIState
{
private:
	//! �t���[���J�E���g
	int m_FrameCount;

	//! �o�H�T���p�J�E���g
	int m_PathfindingTime;

	//! �u�[�X�g�_�b�V���N�[���^�C��
	int m_BoostDashCoolTime;

	//! �r�[�����C�t����ł�����
	int m_BeamRifleShotNum;
public:
	CharcterAIStateAttack(class MobileSuitAIStateComponent* ownerComponent);

	virtual ~CharcterAIStateAttack() {}

	/**
	* @brief ����������
	*/
	virtual void Init()override;

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput()override;

	/**
	* @brief �X�V����
	*/
	virtual void Update()override;

	/**
	* @brief ��Ԃɓ��������̏���
	*/
	virtual void OnEnter()override;

	/**
	* @brief ��Ԃ���o��Ƃ��̏���
	*/
	virtual void OnExit()override;

	/**
	* @brief ��Ԗ�
	*/
	const char* GetName() const override
	{
		return "CharcterAIStateAttack";
	}
};

