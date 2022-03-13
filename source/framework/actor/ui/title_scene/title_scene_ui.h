/**
 * @file title_scene_ui.h
 * @brief �^�C�g��UI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief �^�C�g��UI�N���X
 */
class TitleSceneUI : public Actor
{
public:
	static ID3D11ShaderResourceView* m_TitleSprite001SRV;
	static ID3D11ShaderResourceView* m_TitleSprite002SRV;
	static ID3D11ShaderResourceView* m_TitleSprite003SRV;
	static ID3D11ShaderResourceView* m_PressEnterButtonSRV;

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

	//! �^�C�g���̃X�v���C�g
	class SpriteComponent* m_TitleSprite001;
	//! �^�C�g���̃X�v���C�g
	class SpriteComponent* m_TitleSprite002;
	//! �^�C�g���̃X�v���C�g
	class SpriteComponent* m_TitleSprite003;

	//! �v���X�G���^�[�{�^��
	class SpriteComponent* m_PressEnterButton;

	//! �t���[���J�E���g
	int m_FrameCount;

public:
	TitleSceneUI();
	~TitleSceneUI() override {};

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