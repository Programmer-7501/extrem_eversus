
#include"../../system/main.h"
#include "scene_manager.h"
#include"../../directx/renderer.h"
#include"../../directx/resource/texture_manager.h"
#include"../../system/fade.h"

#include"../../scenes/title_scene.h"
#include"../../scenes/demo_scene.h"
#include"../../scenes/game_scene.h"
#include"../../scenes/mode_select_scene.h"
#include"../../scenes/game_clear_scene.h"
#include"../../scenes/game_over_scene.h"
#include"../../scenes/ai_scene.h"
#include"../../scenes/make_node_scene.h"
#include"../../system/myimgui.h"

namespace SceneManagerData
{
	//! �t�F�[�h�p�e�N�X�`���t�@�C����
	static const char* k_FadeTextureFileName = "asset/texture/fade.png";
}

void SceneManager::Init()
{
	// �t�F�[�h�쐬
	m_Fade = new Fade();
	TextureManager& textureManager = TextureManager::GetInstance();
	ID3D11ShaderResourceView* fadeSRV = textureManager.Load(SceneManagerData::k_FadeTextureFileName);
	if (fadeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::Init fadeSRV��nullptr");
		return;
	}
	m_Fade->SetSRV(fadeSRV);
	m_Fade->Init();

	// �V�[������
	m_CurrentSceneName = SN_TITLE_SCENE;
	m_NextSceneName = m_CurrentSceneName;
	m_CurrentScene = new TitleScene;
	m_CurrentScene->Init();
}

void SceneManager::Update()
{
	// null�`�F�b�N
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::ProcessInput m_CurrentScene��nullptr");
		return;
	}
	m_CurrentScene->Update();

	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::ProcessInput m_Fade��nullptr");
		return;
	}
	// �t�F�[�h�A�b�v�f�[�g
	m_Fade->Update();
}

void SceneManager::Draw()
{
	// null�`�F�b�N
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::ProcessInput m_CurrentScene��nullptr");
		return;
	}

	// �V�[���`��
	m_CurrentScene->Draw();


	MyImGui& myImgui = MyImGui::GetInstance();
	// ImGui�̕`��
	myImgui.Draw();

	// �t�F�[�h�`��
	m_Fade->Draw();
}

void SceneManager::SceneExChange()
{
	// �V�[�����������Ȃ烊�^�[��
	if (m_NextSceneName == m_CurrentSceneName) 
	{
		return;
	}

	//�t�F�[�h���͕ύX���Ȃ�
	if (m_Fade->GetActive() == true)
	{
		return;
	}

	if (m_CurrentScene)
	{
		//�I������
		m_CurrentScene->Uninit();
		delete m_CurrentScene;
		m_CurrentScene = nullptr;
	}

	switch (m_NextSceneName)
	{
	case SN_TITLE_SCENE:
		m_CurrentScene = new TitleScene;
		break;
	case SN_MODESELECT_SCENE:
		m_CurrentScene = new ModeSelectScene;
		break;
	case SN_GAME_SCENE:
		m_CurrentScene = new GameScene;
		break;
	case SN_DEMO_SCENE:
		m_CurrentScene = new DemoScene;
		break;
	case SN_GAME_OVER_SCENE:
		m_CurrentScene = new GameOverScene;
		break;
	case SN_GAME_CLEAR_SCENE:
		m_CurrentScene = new GameClearScene;
		break;
	case SN_AI_TEST_SCENE:
		m_CurrentScene = new AIScene;
		break;
	case SN_MAKE_NODE_SCENE:
		m_CurrentScene = new MakeNodeScene;
		break;
	default:
		break;
	}

	// null�`�F�b�N
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::SceneExChange() m_CurrentScene��nullptr");
		return;
	}

	//����������
	m_CurrentScene->Init();

	// �V�[��������͂���
	m_CurrentSceneName = m_NextSceneName;
}

void SceneManager::FadeInStart(int fadeTime)
{
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::FadeInStart m_Fade��nullptr");
		return;
	}
	m_Fade->SetFade(Fade::FADE_IN, fadeTime);
}

void SceneManager::FadeOutStart(int fadeTime)
{
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::FadeOutStart m_Fade��nullptr");
		return;
	}
	m_Fade->SetFade(Fade::FADE_OUT, fadeTime);
}

void SceneManager::Uninit()
{
	if (m_Fade)
	{
		delete m_Fade;
		m_Fade = nullptr;
	}
	// null�`�F�b�N
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::Uninit m_CurrentScene��nullptr");
		return;
	}
	m_CurrentScene->Uninit();
	delete m_CurrentScene;
	m_CurrentScene = nullptr;

	// �t�F�[�h�p�e�N�X�`���j��
	TextureManager::GetInstance().Unload(SceneManagerData::k_FadeTextureFileName);
}
