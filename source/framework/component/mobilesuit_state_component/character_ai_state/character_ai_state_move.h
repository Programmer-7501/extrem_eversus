/**
 * @file charcter_ai_state_move.h
 * @brief �L�����N�^�[AI�X�e�[�g�ړ�
 */

#pragma once

#include"character_ai_state.h"

 /**
 * @brief �L�����N�^�[AI�X�e�[�g�ړ��N���X
 */
class CharcterAIStateMove : public CharcterAIState
{
private:
	//! �t���[���J�E���g
	int m_FrameCount;
	
	//! �o�H�T���p�J�E���g
	int m_PathfindingTime;

public:
	CharcterAIStateMove(class MobileSuitAIStateComponent* ownerComponent);

	virtual ~CharcterAIStateMove() {}

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
		return "CharcterAIStateMove";
	}
};

