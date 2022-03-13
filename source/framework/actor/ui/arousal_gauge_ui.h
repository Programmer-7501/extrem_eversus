/**
 * @file arousal_gauge_ui.h
 * @brief �o���Q�[�WUI
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �o���Q�[�WUI�N���X
 */
class ArousalGaugeUI : public Actor
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
	//! �o���R���|�[�l���g
	class ArousalComponent* m_ArousalComponent;

	//! UI�\���p�X�v���C�g
	class GaugeSpriteComponent* m_GaugeSpriteComponent;

	class MobileSuit* m_MobileSuit;
public:
	ArousalGaugeUI();
	~ArousalGaugeUI();

	// �Z�b�^�[
	inline void SetMobileSuit(class MobileSuit* mobileSuit) { m_MobileSuit = mobileSuit; }
protected:

	/**
	* @brief �o���Q�[�WUI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �o���Q�[�WUI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �o���Q�[�WUI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};