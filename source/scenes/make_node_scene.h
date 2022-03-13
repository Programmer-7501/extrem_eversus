/**
 * @file make_node_scene.h
 * @brief �m�[�h�쐬�������V�[��
 */

#pragma once

#include"../framework/scene/scene.h"

class MakeNodeScene : public Scene
{
private:
	//! �V�[���`�F���W�\��t���O
	bool m_IsSceneChange;

	//! �t�F�[�h
	class Fade* m_Fade;
public:
	MakeNodeScene();
	~MakeNodeScene();

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
