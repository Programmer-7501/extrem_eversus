/**
 * @file fire.h
 * @brief ��
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief ���N���X
 */
class Fire : public Actor
{
protected:

	//! �g�p�p�[�e�B�N���l�[��
	std::string m_UseParticleName;

	//! �p�[�e�B�N���G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_Emitter;

public:
	Fire();
	~Fire()override {};

	/**
	* @brief �g�p�p�[�e�B�N���̖��O�̃Z�b�g
	* @param[in] const std::string&(particleName) �g�p�p�[�e�B�N���̖��O
	*/
	void SetUseParticleName(const std::string& particleName) { m_UseParticleName = particleName; }
protected:

	/**
	* @brief �o���b�g�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �o���b�g�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �o���b�g�Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;
};