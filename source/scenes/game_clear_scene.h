/**
 * @file game_over_scene.h
 * @brief �Q�[���I�[�o�[�V�[��
 */

#pragma once

#include"../framework/scene/scene.h"

class GameClearScene : public Scene
{
private:
	//! �V�[���`�F���W�\��t���O
	bool m_IsSceneChange;

	//! �t�F�[�h
	class Fade* m_Fade;
public:
	GameClearScene();
	~GameClearScene();

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
