/**
 * @file mode_select_scene.h
 * @brief ���[�h�I���V�[��
 */

#pragma once

#include"../framework/scene/scene.h"


 /**
 * @namespace ModeSelectSceneData
 * @brief ���[�h�I���V�[�����
 */
namespace ModeSelectSceneData
{
	//! �I�[�f�B�I�f�[�^
	//! BGM
	static const char* BGM = "asset/sound/BGM/ModeSelectBGM.wav";
}

class ModeSelectScene : public Scene
{
private:
	//! �V�[���`�F���W�\��t���O
	bool m_IsSceneChange;

	//! �t�F�[�h
	class Fade* m_Fade;

	//! �f�o�b�N�R���W�����r���[��\�����邩�ǂ���
	bool m_IsUseDebugCollisionViewFlag;

	//! �Z���N�g
	class ModeSelectSceneUI* m_ModeSelectSceneUI;
public:
	ModeSelectScene();
	~ModeSelectScene();

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
