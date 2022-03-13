/**
 * @file remaining_bullets_ui.h
 * @brief �c�e���\��UI
 */

#pragma once

#include"../actor.h"
#include<chrono>

namespace RemaingBulletsUIData
{
	//! �����̌���
	static constexpr int k_MaxDigit = 3;
}

/**
* @brief �c�e���\��UI�N���X
*/
class RemaingBulletsUI : public Actor
{
protected:
	//! �����\���p�X�v���C�g
	class NumberSpriteComponent* m_NumberSpriteComponent[RemaingBulletsUIData::k_MaxDigit];

	//! �o���b�g�}�l�[�W���[
	class BulletManager* m_BulletManager;

	//! ��ԍ��̐����̃|�W�V����
	Conv_XM::Vector2f m_LeftPosition;

	//! ����
	int m_Digit;
public:
	RemaingBulletsUI();
	~RemaingBulletsUI();

	inline void SetBulletManager(class BulletManager* bulletManager) { m_BulletManager = bulletManager; }

	inline void SetLeftPosition(const Conv_XM::Vector2f& leftPosition) { m_LeftPosition = leftPosition; }
protected:

	/**
	* @brief �u�[�X�gUI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �u�[�X�gUI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �u�[�X�gUI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};