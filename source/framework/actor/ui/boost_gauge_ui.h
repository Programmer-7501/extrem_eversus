/**
 * @file boost_gauge_ui.h
 * @brief �u�[�X�g�Q�[�WUI
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �u�[�X�g�Q�[�WUI�N���X
 */
class BoostGaugeUI : public Actor
{
public:
	/**
	* @brief �e�N�X�`���ǂݍ���
	*/
	static void LoadTexture();

	/**
	* @brief �e�N�X�`���j��
	*/
	static void UnloadTexture();

protected:
	//! �u�[�X�g�R���|�[�l���g
	class BoostComponent* m_BoostComponent;

	//! UI�\���p�X�v���C�g
	class GaugeSpriteComponent* m_GaugeSpriteComponent;

	//! UI�\���p�X�v���C�g
	class SpriteComponent* m_BoostSpriteComponent;

	//! ���r���X�[�c�A�N�^�[
	class MobileSuit* m_MobileSuit;
public:
	BoostGaugeUI();
	~BoostGaugeUI();

	// �Z�b�^�[
	inline void SetMobileSuit(class MobileSuit* mobileSuit) { m_MobileSuit = mobileSuit; }
protected:

	/**
	* @brief �u�[�X�g�Q�[�WUI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �u�[�X�g�Q�[�WUI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �u�[�X�g�Q�[�WUI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};