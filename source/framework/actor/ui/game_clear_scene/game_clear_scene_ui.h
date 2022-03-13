/**
 * @file game_clear_scene_ui.h
 * @brief �Q�[���N���A�V�[��UI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief �Q�[���N���A�V�[��UI�N���X
 */
class GameClearSceneUI : public Actor
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

	//! �w�i�̃X�v���C�g
	class SpriteComponent* m_BackGroundSprite;

	//! youwin�X�v���C�g
	class SpriteComponent* m_YouWinSprite;

	//! PressEnter�̃X�v���C�g
	class SpriteComponent* m_PressEnterSprite;

	//! �t���[���J�E���g
	int m_FrameCount;
public:
	GameClearSceneUI();
	~GameClearSceneUI();

protected:

	/**
	* @brief �Q�[���N���A�V�[��UI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �Q�[���N���A�V�[��UI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �Q�[���N���A�V�[��UI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};