
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
	//! フェード用テクスチャファイル名
	static const char* k_FadeTextureFileName = "asset/texture/fade.png";
}

void SceneManager::Init()
{
	// フェード作成
	m_Fade = new Fade();
	TextureManager& textureManager = TextureManager::GetInstance();
	ID3D11ShaderResourceView* fadeSRV = textureManager.Load(SceneManagerData::k_FadeTextureFileName);
	if (fadeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::Init fadeSRVがnullptr");
		return;
	}
	m_Fade->SetSRV(fadeSRV);
	m_Fade->Init();

	// シーン生成
	m_CurrentSceneName = SN_TITLE_SCENE;
	m_NextSceneName = m_CurrentSceneName;
	m_CurrentScene = new TitleScene;
	m_CurrentScene->Init();
}

void SceneManager::Update()
{
	// nullチェック
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::ProcessInput m_CurrentSceneがnullptr");
		return;
	}
	m_CurrentScene->Update();

	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::ProcessInput m_Fadeがnullptr");
		return;
	}
	// フェードアップデート
	m_Fade->Update();
}

void SceneManager::Draw()
{
	// nullチェック
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::ProcessInput m_CurrentSceneがnullptr");
		return;
	}

	// シーン描画
	m_CurrentScene->Draw();


	MyImGui& myImgui = MyImGui::GetInstance();
	// ImGuiの描画
	myImgui.Draw();

	// フェード描画
	m_Fade->Draw();
}

void SceneManager::SceneExChange()
{
	// シーン名が同じならリターン
	if (m_NextSceneName == m_CurrentSceneName) 
	{
		return;
	}

	//フェード中は変更しない
	if (m_Fade->GetActive() == true)
	{
		return;
	}

	if (m_CurrentScene)
	{
		//終了処理
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

	// nullチェック
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::SceneExChange() m_CurrentSceneがnullptr");
		return;
	}

	//初期化処理
	m_CurrentScene->Init();

	// シーン名を入力する
	m_CurrentSceneName = m_NextSceneName;
}

void SceneManager::FadeInStart(int fadeTime)
{
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::FadeInStart m_Fadeがnullptr");
		return;
	}
	m_Fade->SetFade(Fade::FADE_IN, fadeTime);
}

void SceneManager::FadeOutStart(int fadeTime)
{
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::FadeOutStart m_Fadeがnullptr");
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
	// nullチェック
	if (m_CurrentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SceneManager::Uninit m_CurrentSceneがnullptr");
		return;
	}
	m_CurrentScene->Uninit();
	delete m_CurrentScene;
	m_CurrentScene = nullptr;

	// フェード用テクスチャ破棄
	TextureManager::GetInstance().Unload(SceneManagerData::k_FadeTextureFileName);
}
