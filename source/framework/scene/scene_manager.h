/**
 * @file scene_manager.h
 * @brief �V�[���Ǘ��@�\
 */

#pragma once

#include"../../function/singleton.h"
#include"scene.h"

 /**
 * @brief �V�[���Ǘ��N���X
 */
class SceneManager : public Singleton<SceneManager>
{
public:
	enum SceneName
	{
		//! �^�C�g���V�[��
		SN_TITLE_SCENE,
		//! ���[�h�I���V�[��
		SN_MODESELECT_SCENE,
		//! �Q�[���V�[��
		SN_GAME_SCENE,
		//! �f���V�[��
		SN_DEMO_SCENE,
		//! �Q�[���I�[�o�[�V�[��
		SN_GAME_OVER_SCENE,
		//! �Q�[���N���A�V�[��
		SN_GAME_CLEAR_SCENE,
		//! AI�e�X�g�V�[��
		SN_AI_TEST_SCENE,
		//! �m�[�h�쐬�V�[��
		SN_MAKE_NODE_SCENE,
	};
private:
	//! ���ݎg�p���Ă���V�[��
	class Scene* m_CurrentScene = nullptr;

	//! �t�F�[�h�N���X
	class Fade* m_Fade = nullptr;

	//! ���݂̃V�[����
	SceneName m_CurrentSceneName;
	//! �؂�ւ���̃V�[����
	SceneName m_NextSceneName;
private:
	friend class Singleton<SceneManager>;

	SceneManager() = default;
	~SceneManager() = default;
public:
	/**
	* @brief ����������
	* @return bool �������������������ۂ�
	*/
	void Init();

	/**
	* @brief �I������
	*/
	void Uninit();

	/**
	* @brief �X�V����
	*/
	void Update();

	/**
	* @brief �`�揈��
	*/
	void Draw();

	/**
	* @brief �V�[���؂�ւ�
	*/
	void SceneExChange();


	/**
	* @brief �t�F�[�h�C��
	* @param[in] int(fadeTime) �t�F�[�h����
	*/
	void FadeInStart(int fadeTime = 60);

	/**
	* @brief �t�F�[�h�A�E�g
	* @param[in] int(fadeTime) �t�F�[�h����
	*/
	void FadeOutStart(int fadeTime = 60);

	// �Z�b�^�[�A�Q�b�^�[
	inline class Scene* GetCurrentScene() const { return m_CurrentScene; }
	inline class Fade* GetFade() const { return m_Fade; }

	inline void SceneChange(SceneName name) { m_NextSceneName = name; }

};
