
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
	// �A�N�^�[�̓ǂݍ��ݏ���
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// null�`�F�b�N
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::LoadData mobileSuit��nullptr");
			continue;
		}
		mobileSuit->LoadData();
	}
}

void MobileSuitManager::Init()
{
	// �A�N�^�[�̏���������
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// null�`�F�b�N
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Init mobileSuit��nullptr");
			continue;
		}
		mobileSuit->Init();
	}
}

void MobileSuitManager::Uninit()
{
	//�A�N�^�[�I������
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// null�`�F�b�N
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Uninit mobileSuit��nullptr");
			continue;
		}
		mobileSuit->Uninit();
	}

	// �A�N�^�[�폜
	for (auto* mobileSuit : m_MobileSuits)
	{
		// null�`�F�b�N
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Uninit mobileSuit��nullptr");
			continue;
		}
		delete mobileSuit;
		mobileSuit = nullptr;
	}
	//���g����ɂ���
	m_MobileSuits.clear();

}

void MobileSuitManager::ProcessInput()
{
	for (auto* const mobileSuit : m_MobileSuits)
	{
		// null�`�F�b�N
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::ProcessInput mobileSuit��nullptr");
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
		// null�`�F�b�N
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::Update mobileSuit��nullptr");
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
