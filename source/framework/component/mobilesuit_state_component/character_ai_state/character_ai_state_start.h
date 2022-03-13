/**
 * @file charcter_ai_state_start.h
 * @brief �L�����N�^�[AI�X�e�[�g�J�n
 */

#pragma once

#include"character_ai_state.h"

 /**
 * @brief �L�����N�^�[AI�X�e�[�g�J�n�N���X
 */
class CharcterAIStateStart : public CharcterAIState
{
private:
	int m_FrameCount;
public:
	CharcterAIStateStart(class MobileSuitAIStateComponent* ownerComponent);

	virtual ~CharcterAIStateStart() {}

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
		return "CharcterAIStateStart";
	}
};

