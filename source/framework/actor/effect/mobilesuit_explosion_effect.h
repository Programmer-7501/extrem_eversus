/**
 * @file mobilesuit_explosion_effect.h
 * @brief ���r���X�[�c�̔����G�t�F�N�g
 */

#pragma once

#include"effect.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief ���r���X�[�c�̔����G�t�F�N�g�N���X
 */
class MobileSuitExplosionEffect : public Effect
{
private:
	//! �p�[�e�B�N���G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_Emitter;

public:
	MobileSuitExplosionEffect();
	virtual ~MobileSuitExplosionEffect() override;

	/**
	* @brief �A�N�e�B�u����
	*/
	virtual void SetActive(bool flag)override;
private:

	/**
	* @brief �o���b�g�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �o���b�g�Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief v�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};