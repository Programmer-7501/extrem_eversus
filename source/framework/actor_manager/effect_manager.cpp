
#include"../../system/main.h"
#include "effect_manager.h"
#include"../../particle/particle_manager.h"

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

class Effect* EffectManager::UseEffect(const Conv_XM::Vector3f & position)
{
	class Effect* retEffect = nullptr;

	for (auto* const effect : m_Effects)
	{
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::UseBullet effect��nullptr");
			continue;
		}

		if (effect->GetActive() == false)
		{
			// �|�W�V��������
			effect->SetPosition(position);
			// �A�N�e�B�u����
			effect->SetActive(true);
			// ���^�[���G�t�F�N�g����
			retEffect = effect;
			break;
		}
	}

	return retEffect;
}

void EffectManager::LoadData()
{
	// �A�N�^�[�̓ǂݍ��ݏ���
	for (auto* const effect : m_Effects)
	{
		// null�`�F�b�N
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::LoadData effect��nullptr");
			continue;
		}
		effect->LoadData();
	}
}

void EffectManager::Init()
{
	// �A�N�^�[�̏���������
	for (auto* const effect : m_Effects)
	{
		// null�`�F�b�N
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Init effect��nullptr");
			continue;
		}
		effect->Init();
		effect->SetActive(false);
	}
}

void EffectManager::Uninit()
{
	//�A�N�^�[�I������
	for (auto* const effect : m_Effects)
	{
		// null�`�F�b�N
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit effect��nullptr");
			continue;
		}
		effect->Uninit();
	}

	// �A�N�^�[�폜
	for (auto* effect : m_Effects)
	{
		// null�`�F�b�N
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit effect��nullptr");
			continue;
		}
		delete effect;
		effect = nullptr;
	}
	//���g����ɂ���
	m_Effects.clear();
}

void EffectManager::ProcessInput()
{
	for (auto* const effect : m_Effects)
	{
		// null�`�F�b�N
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::ProcessInput effect��nullptr");
			continue;
		}
		// ��A�N�e�B�u�Ȃ�X�V���Ȃ�
		if (effect->GetActive() == false)
		{
			continue;
		}
		effect->ProcessInput();
	}
}

void EffectManager::Update()
{
	for (auto* const effect : m_Effects)
	{
		// null�`�F�b�N
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Update effect��nullptr");
			continue;
		}
		// ��A�N�e�B�u�Ȃ�X�V���Ȃ�
		if (effect->GetActive() == false)
		{
			continue;
		}
		effect->Update();
	}
}
