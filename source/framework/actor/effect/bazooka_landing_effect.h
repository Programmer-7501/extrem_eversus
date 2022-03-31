/**
 * @file bazooka_landing_effect.h
 * @brief �o�Y�[�J���e�G�t�F�N�g
 */

#pragma once

#include"effect.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief �o�Y�[�J���e�G�t�F�N�g�N���X
 */
class BazookaLandingEffect : public Effect
{
private:
	//! �p�[�e�B�N���G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_Emitter;

public:
	BazookaLandingEffect();
	virtual ~BazookaLandingEffect() override;

	/**
	* @brief �A�N�e�B�u����
	*/
	virtual void SetActive(bool flag)override;
private:

	/**
	* @brief �o�Y�[�J���e�G�t�F�N�g�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �o�Y�[�J���e�G�t�F�N�g�Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief �o�Y�[�J���e�G�t�F�N�g�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};