/**
 * @file radar.h
 * @brief ���[�_�[
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief ���[�_�[�N���X
 */
class Radar : public Actor
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
	//! �v���C���[���r���X�[�c
	class MobileSuit* m_PlayerMobileSuit;

	//! �������r���X�[�c
	class MobileSuit* m_AllienMobileSuit;

	//! �G���r���X�[�c
	class MobileSuit* m_Enemy001MobileSuit;

	//! �G���r���X�[�c
	class MobileSuit* m_Enemy002MobileSuit;

	//! ���[�_�[�X�v���C�g�R���|�[�l���g
	class SpriteComponent* m_BackGroundSpriteComponent;

	//! ������X�v���C�g�R���|�[�l���g
	class ChangeableSpriteComponent* m_PlayerPieceSpriteComponent;

	//! ������X�v���C�g�R���|�[�l���g
	class ChangeableSpriteComponent* m_AllienPieceSpriteComponent;

	//! �G001��X�v���C�g�R���|�[�l���g
	class ChangeableSpriteComponent* m_Enemy001PieceSpriteComponent;

	//! �G002��X�v���C�g�R���|�[�l���g
	class ChangeableSpriteComponent* m_Enemy002PieceSpriteComponent;
public:
	Radar();
	~Radar();

	inline void SetParameter(class MobileSuit* player, class MobileSuit* allien, class MobileSuit* enemy001, class MobileSuit* enemy002)
	{
		m_PlayerMobileSuit = player;
		m_AllienMobileSuit = allien;
		m_Enemy001MobileSuit = enemy001;
		m_Enemy002MobileSuit = enemy002;
	}

protected:

	/**
	* @brief ���[�_�[�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ���[�_�[�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief ���[�_�[UI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor() override;
};