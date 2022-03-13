/**
 * @file game_scene.h
 * @brief �Q�[���V�[��
 */

#pragma once

#include"../framework/scene/scene.h"

class GameScene : public Scene
{
private:
	//! �V�[���`�F���W�\��t���O
	bool m_IsSceneChange;

	//! �t�F�[�h
	class Fade* m_Fade;

	//! �f�o�b�N�R���W�����r���[��\�����邩�ǂ���
	bool m_IsUseDebugCollisionViewFlag;

	//! �Q�[���}�l�[�W���[
	class GameManager* m_GameManager;

	//! �^�C�����~�b�gUI
	class TimeLimitUI* m_TimeLimitUI;
public:
	GameScene();
	~GameScene();

private:
	/**
	* @brief �V�[���œǂݍ��ރf�[�^����
	*/
	void LoadData() override;

	/**
	* @brief �V�[���Ǝ��̔j��
	*/
	void UnloadData() override;

	/**
	* @brief �V�[���ŏ������O�ɏ�����͂��鏈��
	*/
	void SetData()override;

	/**
	* @brief �V�[���Ǝ��̍X�V����
	*/
	void UpdateScene() override;

	/**
	* @brief �V�[���ŗp������@�\�̐���
	*/
	void CreateSceneFunction() override;

	/**
	* @brief �V�[���ŗp����ꂽ�@�\�̍폜
	*/
	void UnloadSceneFunction() override;
};
