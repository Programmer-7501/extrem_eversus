/**
 * @file actor_manager.h
 * @brief �A�N�^�[�̊Ǘ����s��
 */

#pragma once

#include"../actor/actor.h"

 /**
 * @brief �A�N�^�[�̊Ǘ�������N���X
 */
class ActorManager
{
public:
	ActorManager() {}
	virtual ~ActorManager() {}

	/**
	* @brief �ǂݍ��ݏ���
	*/
	virtual void LoadData() = 0;

	/**
	* @brief ����������
	*/
	virtual void Init() = 0;

	/**
	* @brief �I������
	*/
	virtual void Uninit() = 0;

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput() = 0;

	/**
	* @brief �X�V����
	*/
	virtual void Update() = 0;
};