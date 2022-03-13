/**
 * @file title_scene_ui.h
 * @brief �^�C�g��UI
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �^�C�g��UI�N���X
 */
class OtherUI : public Actor
{
public:
	static ID3D11ShaderResourceView* m_BeamRifleSRV;
	static ID3D11ShaderResourceView* m_CannonSRV;

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
	class SpriteComponent* m_BeamRifle;
	//! �^�C�g���̃X�v���C�g
	class SpriteComponent* m_Cannon;

	//! �t���[���J�E���g
	int m_FrameCount;

public:
	OtherUI();
	~OtherUI() override {};

protected:

	/**
	* @brief ���[�h�I��UI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ���[�h�I��UI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

};