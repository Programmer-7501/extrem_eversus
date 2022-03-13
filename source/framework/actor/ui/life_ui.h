/**
 * @file life_ui.h
 * @brief ���C�tUI
 */

#pragma once

#include"../actor.h"

namespace LifeUIData
{
	//! �����̌���
	static constexpr int k_Digit = 3;
}

 /**
 * @brief ���C�tUI�N���X
 */
class LifeUI : public Actor
{
protected:
	//! ���C�t�R���|�[�l���g
	class LifeComponent* m_LifeComponent;

	//! �����\���p�X�v���C�g
	class NumberSpriteComponent* m_NumberSpriteComponent[LifeUIData::k_Digit];

	class MobileSuit* m_MobileSuit;
public:
	LifeUI();
	~LifeUI();

	// �Z�b�^�[
	inline void SetMobileSuit(class MobileSuit* mobileSuit) { m_MobileSuit = mobileSuit; }
protected:

	/**
	* @brief ���C�tUI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ���C�tUI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief ���C�tUI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};