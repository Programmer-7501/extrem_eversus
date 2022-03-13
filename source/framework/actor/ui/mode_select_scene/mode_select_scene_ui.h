/**
 * @file mode_select_scene_ui.h
 * @brief ���[�h�I��UI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief ���[�h�I��UI�N���X
 */
class ModeSelectSceneUI : public Actor
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

	/**
	* @brief �T�E���h�ǂݍ���
	*/
	static void LoadSound();

	/**
	* @brief �T�E���h�j��
	*/
	static void UnloadSound();

public:
	enum SelectScene
	{
		SS_NONE,
		SS_GAMESCENE,
		SS_SHADERDEMOSCENE,
		SS_AIDEMOSCENE,
		SS_GRAPHMAKESCENE,
		SS_MAX,
	};
protected:

	//! ���[�h�I��
	SelectScene m_ModeSelect;

	//! �V�[���ɑ��郂�[�h
	SelectScene m_SendMode;

	//! �w�i�̃X�v���C�g
	class SpriteComponent* m_BackGroundSprite;

	//! �Q�[���V�[���ւ̃X�v���C�g
	class ChangeableSpriteComponent* m_ToGameSceneSprite;

	//! �f���V�[���ւ̃X�v���C�g
	class ChangeableSpriteComponent* m_ToShaderDemoSceneSprite;

	//! AI�f���V�[���ւ̃X�v���C�g
	class ChangeableSpriteComponent* m_ToAIDemoSceneSprite;

	//! �O���t�쐬�V�[���ւ̃X�v���C�g
	class ChangeableSpriteComponent* m_ToMakeGraphSceneSprite;
public:
	ModeSelectSceneUI();
	~ModeSelectSceneUI();

	// �Q�b�^�[
	inline SelectScene GetSelectSceneMode() const { return m_SendMode; }

protected:

	/**
	* @brief ���[�h�I��UI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ���[�h�I��UI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief ���[�h�I��UI�Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor();
};