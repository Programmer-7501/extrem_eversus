
#include"../../system/main.h"
#include "mobilesuit_manager.h"

MobileSuitManager::MobileSuitManager()
{
}

MobileSuitManager::~MobileSuitManager()
{
}

void MobileSuitManager::LoadData()
{
	// アクターの読み込み処理
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// nullチェック
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::LoadData mobileSuitがnullptr");
			continue;
		}
		mobileSuit->LoadData();
	}
}

void MobileSuitManager::Init()
{
	// アクターの初期化処理
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// nullチェック
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Init mobileSuitがnullptr");
			continue;
		}
		mobileSuit->Init();
	}
}

void MobileSuitManager::Uninit()
{
	//アクター終了処理
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// nullチェック
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Uninit mobileSuitがnullptr");
			continue;
		}
		mobileSuit->Uninit();
	}

	// アクター削除
	for (auto* mobileSuit : m_MobileSuits)
	{
		// nullチェック
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Uninit mobileSuitがnullptr");
			continue;
		}
		delete mobileSuit;
		mobileSuit = nullptr;
	}
	//中身を空にする
	m_MobileSuits.clear();

}

void MobileSuitManager::ProcessInput()
{
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// nullチェック
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::ProcessInput mobileSuitがnullptr");
			continue;
		}
		if (mobileSuit->GetActive() == false)
		{
			continue;
		}
		mobileSuit->ProcessInput();
	}
}

void MobileSuitManager::Update()
{
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// nullチェック
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Update mobileSuitがnullptr");
			continue;
		}
		if (mobileSuit->GetActive() == false)
		{
			continue;
		}
		mobileSuit->Update();
	}
}

void MobileSuitManager::SpawnMobileSuit(int spawnNum)
{
	m_MobileSuits[spawnNum]->SetPosition(Conv_XM::Vector3f(rand() % 20, 20.0f, rand() % 20));
	m_MobileSuits[spawnNum]->SetActive(true);
}
