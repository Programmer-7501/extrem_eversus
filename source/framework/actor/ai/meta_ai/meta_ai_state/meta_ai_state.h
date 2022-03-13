/**
 * @file meta_ai_state.h
 * @brief ���^AI�X�e�[�g
 */

#pragma once

#include"../meta_ai.h"

 /**
 * @brief ���^AI�X�e�[�g�N���X
 */
class MetaAIState
{
private:
	class MetaAI* m_Owner;
public:
	MetaAIState(class MetaAI* owner)
		:m_Owner(owner)
	{}

	virtual ~MetaAIState() {}

	/**
	* @brief ����������
	*/
	virtual void Init() = 0;

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput() = 0;

	/**
	* @brief �X�V����
	*/
	virtual void Update() = 0;

	/**
	* @brief ��Ԃɓ��������̏���
	*/
	virtual void OnEnter() = 0;

	/**
	* @brief ��Ԃ���o��Ƃ��̏���
	*/
	virtual void OnExit() = 0;

	/**
	* @brief ��Ԗ�
	*/
	virtual const char* GetName() const = 0;
};

