
#include"../system/main.h"
#include "game_clear_scene.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/actor/ui/game_clear_scene/game_clear_scene_ui.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../system/fade.h"
#include"game_scene.h"
#include"demo_scene.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../framework/actor/camera/debug_camera.h"
#include"../particle/particle_manager.h"
#include"../system/audio_manager.h"

/**
* @namespace GameClearSceneData
* @brief ゲームクリアシーン情報
*/
namespace GameClearSceneData
{
	//! フェードイン時間
	static constexpr int k_FadeInTime = 60;

	//! フェードアウト時間
	static constexpr int k_FadeOutTime = 60;

	//! BGM
	static const char* BGM = "asset/sound/BGM/GameClearBGM.wav";
}

GameClearScene::GameClearScene()
	: m_IsSceneChange(false)
	, m_Fade(nullptr)
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::LoadData()
{
	// パーティクルマネージャーのデバックカウンターを非表示にする
	ParticleManager::GetInstance().SetDebugCountView(false);

	// シーンマネージャー取得
	SceneManager& sceneManager = SceneManager::GetInstance();

	// フェード取得
	m_Fade = sceneManager.GetFade();
	// フェード開始
	m_Fade->SetFade(Fade::FADE_OUT, GameClearSceneData::k_FadeOutTime);

	// オーディオマネージャーの取得
	AudioManager& audioManager = AudioManager::GetInstance();
	// BGMの読み込み
	audioManager.Load(GameClearSceneData::BGM);
	// BGMの再生
	audioManager.Play(GameClearSceneData::BGM,true);

	// アクターマネージャー作成
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if (otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData otherActorManagerがnullptr");
		return;
	}

	// デバッグカメラを作成
	DebugCamera* debugCamera = otherActorManager->CreateActor<DebugCamera>();
	if (debugCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData debugCameraがnullptr");
		return;
	}
	debugCamera->SetPosition(Conv_XM::Vector3f(0.0f, 5.0f, -10.0f));

	// ライトカメラを作成
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData lightCameraActorがnullptr");
		return;
	}
	// ライトカメラコンポーネント作成
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData lightCameraActorがnullptr");
		return;
	}
	// ライトカメラのパラメーター設定
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	lightCamera->SetTarget(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	lightCamera->SetUp(Conv_XM::Vector3f(0.0f, 0.0f, 1.0f));
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData m_SceneRenderManagerがnullptr");
		return;
	}
	// ライトカメラをシーンレンダーに設定
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);

	// UI作成
	GameClearSceneUI::LoadTexture();
	otherActorManager->CreateActor<GameClearSceneUI>();
}

void GameClearScene::UnloadData()
{
	// テクスチャの破棄
	GameClearSceneUI::UnloadTexture();
}

void GameClearScene::SetData()
{
}

void GameClearScene::UpdateScene()
{
	// インプットマネージャーを取得
	InputManager& inputManager = InputManager::GetInstance();

	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fadeがnullptr");
		return;
	}

	// 決定ボタンを押したら
	if (inputManager.GetIsTrigger(InputManager::IN_DECIDE) == true)
	{
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
	}

	// フェードし終えたらシーンを切り替える
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		SceneManager::GetInstance().SceneChange(SceneManager::SN_TITLE_SCENE);
		AudioManager& audioManager = AudioManager::GetInstance();
		audioManager.Stop(GameClearSceneData::BGM);
	}
}

void GameClearScene::CreateSceneFunction()
{
	// シーンレンダー作成
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void GameClearScene::UnloadSceneFunction()
{
	// シーンレンダーを破棄
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
