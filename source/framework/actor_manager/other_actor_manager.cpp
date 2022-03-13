
#include"../../system/main.h"
#include"other_actor_manager.h"


OtherActorManager::OtherActorManager()
{
}

OtherActorManager::~OtherActorManager()
{
}

void OtherActorManager::LoadData()
{
	// アクターの読み込み処理
	for (auto* const actor : m_Actors)
	{
		// nullチェック
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::LoadData actorがnullptr");
			continue;
		}
		actor->LoadData();
	}
}

void OtherActorManager::Init()
{
	// アクターの初期化処理
	for (auto* const actor : m_Actors)
	{
		// nullチェック
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Init actorがnullptr");
			continue;
		}
		actor->Init();
	}
}

void OtherActorManager::Uninit()
{
	//アクター終了処理
	for (auto* const actor : m_Actors)
	{
		// nullチェック
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Uninit actorがnullptr");
			continue;
		}
		actor->Uninit();
	}

	// アクター削除
	for (auto* actor : m_Actors)
	{
		// nullチェック
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Uninit actorがnullptr");
			continue;
		}
		delete actor;
		actor = nullptr;
	}
	//中身を空にする
	m_Actors.clear();
}

void OtherActorManager::ProcessInput()
{
	for (auto* const actor : m_Actors)
	{
		// nullチェック
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::ProcessInput actorがnullptr");
			continue;
		}
		actor->ProcessInput();
	}
}

void OtherActorManager::Update()
{
	for (auto* const actor : m_Actors)
	{
		// nullチェック
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Update actorがnullptr");
			continue;
		}
		actor->Update();
	}
}
