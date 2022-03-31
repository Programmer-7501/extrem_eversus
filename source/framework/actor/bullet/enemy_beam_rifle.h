/**
 * @file enemy_beam_rifle.h
 * @brief �G�̃r�[�����C�t��
 */

#pragma once

#include"enemy_bullet.h"

 /**
 * @brief �G�̃r�[�����C�t��
 */
class EnemyBeamRifle : public EnemyBullet
{
protected:
	//! �g�p�p�[�e�B�N���l�[��
	std::string m_UseParticleName;

	//! �p�[�e�B�N���G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_Emitter;
public:
	EnemyBeamRifle();
	~EnemyBeamRifle();

	/**
	* @brief �g�p�p�[�e�B�N���̖��O�̃Z�b�g
	* @param[in] const std::string&(particleName) �g�p�p�[�e�B�N���̖��O
	*/
	void SetUseParticleName(const std::string& particleName) { m_UseParticleName = particleName; }

protected:

	/**
	* @brief �G�̃r�[�����C�t���Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �G�̃r�[�����C�t���Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �G�̃r�[�����C�t���Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief �G�̃r�[�����C�t���Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

	/**
	* @brief �����蔻�菈��
	* @details �Փˎ��ɌĂяo�����֐�
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;
};