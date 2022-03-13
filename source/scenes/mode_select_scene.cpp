
#include"../system/main.h"
#include "mode_select_scene.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/actor/ui/mode_select_scene/mode_select_scene_ui.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../system/fade.h"
#include"game_scene.h"
#include"demo_scene.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../framework/actor/camera/debug_camera.h"
#include"../particle/particle_manager.h"
#include"../system/audio_manager.h"

/**
* @namespace ModeSelectSceneData
* @brief モード選択シーン情報
*/
namespace ModeSelectSceneData
{
	//! フェードイン時間
	static constexpr int k_FadeInTime = 60;

	//! フェードアウト時間
	static constexpr int k_FadeOutTime = 60;
}

ModeSelectScene::ModeSelectScene()
	: m_IsSceneChange(false)
	, m_Fade(nullptr)
	, m_ModeSelectSceneUI(nullptr)
{
}

ModeSelectScene::~ModeSelectScene()
{
}

void ModeSelectScene::LoadData()
{
	// オーディオを取得
	AudioManager& audioManager = AudioManager::GetInstance();
	// オーディオデータ読み込み
	audioManager.Load(ModeSelectSceneData::BGM);
	// 再生
	audioManager.Play(ModeSelectSceneData::BGM, true);

	// パーティクルマネージャー取得
	ParticleManager& particleManager = ParticleManager::GetInstance();
	// デバッグカウンター非表示
	particleManager.SetDebugCountView(false);

	// シーンマネージャー取得
	SceneManager& sceneManager = SceneManager::GetInstance();

	// フェード取得
	m_Fade = sceneManager.GetFade();
	// フェード開始
	m_Fade->SetFade(Fade::FADE_OUT, ModeSelectSceneData::k_FadeOutTime);

	// アクターマネージャー作成
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if(otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData otherActorManagerがnullptr");
		return;
	}

	// UIテクスチャ読み込み
	ModeSelectSceneUI::LoadTexture();
	// UIサウンド読み込み
	ModeSelectSceneUI::LoadSound();
	// UI作成
	m_ModeSelectSceneUI = otherActorManager->CreateActor<ModeSelectSceneUI>();
	if (m_ModeSelectSceneUI == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData m_ModeSelectSceneUIがnullptr");
		return;
	}

	// デバッグカメラを作成
	DebugCamera* debugCamera = otherActorManager->CreateActor<DebugCamera>();
	if (debugCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData debugCameraがnullptr");
		return;
	}
	debugCamera->SetPosition(Conv_XM::Vector3f(0.0f, 5.0f, -10.0f));

	// ライトカメラを作成
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActorがnullptr");
		return;
	}
	// ライトコンポーネント作成
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActorがnullptr");
		return;
	}
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	lightCamera->SetTarget(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	lightCamera->SetUp(Conv_XM::Vector3f(0.0f, 0.0f, 1.0f));
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManagerがnullptr");
		return;
	}
	// シーンレンダーに登録
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);
}

void ModeSelectScene::UnloadData()
{
	// テクスチャ破棄
	ModeSelectSceneUI::UnloadTexture();
	// サウンド破棄
	ModeSelectSceneUI::UnloadSound();
	// オーディオを取得
	AudioManager& audioManager = AudioManager::GetInstance();
	// オーディオデータ読み込み
	audioManager.Unload(ModeSelectSceneData::BGM);

}

void ModeSelectScene::SetData()
{
}

void ModeSelectScene::UpdateScene()
{
	// フェード、シーン切り替え
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fadeがnullptr");
		return;
	}

	// フェードし終えたらシーンを切り替える
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		// オーディオを取得
		AudioManager& audioManager = AudioManager::GetInstance();
		// 停止
		audioManager.Stop(ModeSelectSceneData::BGM);

		switch (m_ModeSelectSceneUI->GetSelectSceneMode())
		{
		case ModeSelectSceneUI::SS_NONE:
			break;
		case ModeSelectSceneUI::SS_GAMESCENE:
			// シーンを切り替える
			SceneManager::GetInstance().SceneChange(SceneManager::SN_GAME_SCENE);
			break;
		case ModeSelectSceneUI::SS_SHADERDEMOSCENE:
			// シーンを切り替える
			SceneManager::GetInstance().SceneChange(SceneManager::SN_DEMO_SCENE);
			break;
		case ModeSelectSceneUI::SS_AIDEMOSCENE:
			// シーンを切り替える
			SceneManager::GetInstance().SceneChange(SceneManager::SN_AI_TEST_SCENE);
			break;
		case ModeSelectSceneUI::SS_GRAPHMAKESCENE:
			// シーンを切り替える
			SceneManager::GetInstance().SceneChange(SceneManager::SN_MAKE_NODE_SCENE);
			break;
		case ModeSelectSceneUI::SS_MAX:
			break;
		default:
			break;
		}
	}


	if (m_IsSceneChange == true)
	{
		return;
	}

	switch (m_ModeSelectSceneUI->GetSelectSceneMode())
	{
	case ModeSelectSceneUI::SS_NONE:
		break;
	case ModeSelectSceneUI::SS_GAMESCENE:
		// シーン切り替えフラグを立てる
		m_IsSceneChange = true;
		// フェードを始める
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_SHADERDEMOSCENE:
		// シーン切り替えフラグを立てる
		m_IsSceneChange = true;
		// フェードを始める
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_AIDEMOSCENE:
		// シーン切り替えフラグを立てる
		m_IsSceneChange = true;
		// フェードを始める
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_GRAPHMAKESCENE:
		// シーン切り替えフラグを立てる
		m_IsSceneChange = true;
		// フェードを始める
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_MAX:
		break;
	default:
		break;
	}
}

void ModeSelectScene::CreateSceneFunction()
{
	// シーンレンダー作成
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void ModeSelectScene::UnloadSceneFunction()
{
	// シーンレンダーマネージャーを破棄
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
