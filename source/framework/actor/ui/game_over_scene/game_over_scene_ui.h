/**
 * @file game_over_scene_ui.h
 * @brief �Q�[���I�[�o�[�V�[��UI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief �Q�[���I�[�o�[�V�[��UI�N���X
 */
class GameOverSceneUI : public Actor
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

	//! youlose�X�v���C�g
	class SpriteComponent* m_YouLoseSprite;

	//! PressEnter�̃X�v���C�g
	class SpriteComponent* m_PressEnterSprite;

	//! �t���[���J�E���g
	int m_FrameCount;
public:
	GameOverSceneUI();
	~GameOverSceneUI();

protected:

	/**
	* @brief �Q�[���I�[�o�[�V�[��UI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �Q�[���I�[�o�[�V�[��UI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �Q�[���I�[�o�[�V�[��UI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};