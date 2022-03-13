/**
 * @file charcter_ai_state_wander_around.h
 * @brief �L�����N�^�[AI�X�e�[�g���낤�낷��
 */

#pragma once

#include"character_ai_state.h"

 /**
 * @brief �L�����N�^�[AI�X�e�[�g���낤�낷��N���X
 */
class CharcterAIStateWanderAround : public CharcterAIState
{
public:
	CharcterAIStateWanderAround(class MobileSuitAIStateComponent* ownerComponent);

	virtual ~CharcterAIStateWanderAround() {}

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
		return "CharcterAIStateWanderAround";
	}
};

