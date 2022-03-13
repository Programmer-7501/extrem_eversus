
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
	// �A�N�^�[�̓ǂݍ��ݏ���
	for (auto* const actor : m_Actors)
	{
		// null�`�F�b�N
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::LoadData actor��nullptr");
			continue;
		}
		actor->LoadData();
	}
}

void OtherActorManager::Init()
{
	// �A�N�^�[�̏���������
	for (auto* const actor : m_Actors)
	{
		// null�`�F�b�N
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Init actor��nullptr");
			continue;
		}
		actor->Init();
	}
}

void OtherActorManager::Uninit()
{
	//�A�N�^�[�I������
	for (auto* const actor : m_Actors)
	{
		// null�`�F�b�N
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Uninit actor��nullptr");
			continue;
		}
		actor->Uninit();
	}

	// �A�N�^�[�폜
	for (auto* actor : m_Actors)
	{
		// null�`�F�b�N
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Uninit actor��nullptr");
			continue;
		}
		delete actor;
		actor = nullptr;
	}
	//���g����ɂ���
	m_Actors.clear();
}

void OtherActorManager::ProcessInput()
{
	for (auto* const actor : m_Actors)
	{
		// null�`�F�b�N
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::ProcessInput actor��nullptr");
			continue;
		}
		actor->ProcessInput();
	}
}

void OtherActorManager::Update()
{
	for (auto* const actor : m_Actors)
	{
		// null�`�F�b�N
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::Update actor��nullptr");
			continue;
		}
		actor->Update();
	}
}
