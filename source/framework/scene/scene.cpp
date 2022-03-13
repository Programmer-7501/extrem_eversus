

#include"../../system/main.h"
#include "scene.h"
#include"../scene_function/scene_render_manager.h"
#include"../scene_function/scene_collision_manager.h"
#include"../../particle/particle_manager.h"

Scene::Scene()
	: m_SceneRenderManager(nullptr)
	, m_SceneCollisionManager(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	// シーンに用いられる機能生成
	CreateSceneFunction();

	// データ読み込み
	LoadData();

	// オブジェクトマネージャー読み込み処理
	for (auto* const actorManager : m_ActorManager)
	{
		// nullチェック
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Init actorManagerがnullptr");
			continue;
		}
		actorManager->LoadData();
	}

	// データのセット
	SetData();

	// オブジェクトマネージャー初期化処理
	for (auto* const actorManager : m_ActorManager)
	{
		// nullチェック
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Init actorManagerがnullptr");
			continue;
		}
		actorManager->Init();
	}

	ParticleManager::GetInstance().UpdateParticleEmitterArrayConstantBuffer();
}

void Scene::Uninit()
{
	// オブジェクトマネージャー終了処理
	for (auto* const actorManager : m_ActorManager)
	{
		// nullチェック
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Uninit actorManagerがnullptr");
			continue;
		}
		actorManager->Uninit();
	}

	// オブジェクトマネージャー削除
	for (auto* actorManager : m_ActorManager)
	{
		// nullチェック
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Uninit actorManagerがnullptr");
			continue;
		}
		delete actorManager;
		actorManager = nullptr;
	}
	//中身を空にする
	m_ActorManager.clear();

	// シーン独自の破棄
	UnloadData();

	// シーンに用いられた機能の破棄
	UnloadSceneFunction();

	// パーティクルマネージャーリセット処理
	ParticleManager::GetInstance().Reset();
}

void Scene::Update()
{
	// オブジェクトマネージャー更新処理
	for (auto* const actorManager : m_ActorManager)
	{
		// nullチェック
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::ProcessInput actorManagerがnullptr");
			continue;
		}
		actorManager->ProcessInput();
	}

	// オブジェクトマネージャー更新処理
	for (auto* const actorManager : m_ActorManager)
	{
		// nullチェック
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::ProcessInput actorManagerがnullptr");
			continue;
		}
		actorManager->Update();
	}

	// コリジョンマネージャーの更新
	if (m_SceneCollisionManager)
	{
		m_SceneCollisionManager->Update();
	}

	// シーン独自の更新
	UpdateScene();

	// パーティクルの更新
	ParticleManager::GetInstance().Update();
}

void Scene::Draw() const
{
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Draw();
	}
}

void Scene::LoadData()
{
}

void Scene::UnloadData()
{
}

void Scene::SetData()
{
}

void Scene::UpdateScene()
{
}

void Scene::CreateSceneFunction()
{
}

void Scene::UnloadSceneFunction()
{
}
