
#include"../../system/main.h"
#include "bullet_manager.h"
#include"../../particle/particle_manager.h"
#include"../../framework/actor/mobilesuit/mobilesuit.h"

namespace BulletManagerData
{
	//! �f�t�H���g�̃����[�h�J�E���g
	static constexpr int k_DefaultReloadTime = 60 * 3;
}

BulletManager::BulletManager()
	: m_MaxBullet(0)
	, m_CurrentBullet(0)
	, m_ReloadMethod(RM_ONEBYONE)
	, m_ReloadCount(0)
	, m_ReloadTime(BulletManagerData::k_DefaultReloadTime)
{
}

BulletManager::~BulletManager()
{
}

class Bullet* BulletManager::UseBullet(class MobileSuit* target, const Conv_XM::Vector3f & position, const Conv_XM::Vector3f & velocity)
{
	// ���^�[���o���b�g�ϐ�
	Bullet* retBullet = nullptr;

	// �c�e�����Ȃ��Ȃ烊�^�[��
	if (m_CurrentBullet <= 0)
	{
		return retBullet;
	}


	for (auto* const bullet : m_Bullets)
	{
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::UseBullet bullet��nullptr");
			continue;
		}

		if (bullet->GetActive() == false)
		{
			// �|�W�V��������
			bullet->SetPosition(position);
			// ���x����
			bullet->SetVelocity(velocity);
			// �^�[�Q�b�g����
			bullet->SetTargetMobileSuit(target);
			// �A�N�e�B�u����
			bullet->SetActive(true);
			// �c�e�������炷
			m_CurrentBullet--;
			// ���^�[���o���b�g����
			retBullet = bullet;
			break;
		}
	}

	return retBullet;
}

void BulletManager::Reload()
{
	switch (m_ReloadMethod)
	{
	case BulletManager::RM_ONEBYONE:
		// �����[�h�J�E���g�A�b�v
		m_ReloadCount++;
		// �����[�h�J�E���g�������[�h�^�C���𒴂�����
		if (m_ReloadCount > m_ReloadTime)
		{
			// ���݂̎c�e���𑝂₷
			m_CurrentBullet++;
			// �����[�h�J�E���g��������
			m_ReloadCount = 0;
		}
		break;
	case BulletManager::RM_ALLUSE:
		break;
	case BulletManager::RM_NONE_RELOAD:
		break;
	case BulletManager::RM_MAX:
		break;
	default:
		break;
	}
}

void BulletManager::Reset()
{
	switch (m_ReloadMethod)
	{
	case BulletManager::RM_ONEBYONE:
		m_ReloadCount = 0;
		m_CurrentBullet = m_MaxBullet;
		break;
	case BulletManager::RM_ALLUSE:
		break;
	case BulletManager::RM_NONE_RELOAD:
		break;
	case BulletManager::RM_MAX:
		break;
	default:
		break;
	}
}

void BulletManager::SetLandingEffectManager(EffectManager * effectManager)
{
	for (auto* const bullet : m_Bullets)
	{
		// null�`�F�b�N
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::SetLandingEffectManager bullet��nullptr");
			continue;
		}
		bullet->SetLandingEffectManager(effectManager);
	}
}

void BulletManager::LoadData()
{
	// �A�N�^�[�̓ǂݍ��ݏ���
	for (auto* const bullet : m_Bullets)
	{
		// null�`�F�b�N
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::LoadData bullet��nullptr");
			continue;
		}
		bullet->LoadData();
	}
}

void BulletManager::Init()
{
	// �A�N�^�[�̏���������
	for (auto* const bullet : m_Bullets)
	{
		// null�`�F�b�N
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Init bullet��nullptr");
			continue;
		}
		bullet->Init();
		bullet->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
		bullet->SetActive(false);
	}
}

void BulletManager::Uninit()
{
	//�A�N�^�[�I������
	for (auto* const bullet : m_Bullets)
	{
		// null�`�F�b�N
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit bullet��nullptr");
			continue;
		}
		bullet->Uninit();
	}

	// �A�N�^�[�폜
	for (auto* bullet : m_Bullets)
	{
		// null�`�F�b�N
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit bullet��nullptr");
			continue;
		}
		delete bullet;
		bullet = nullptr;
	}
	//���g����ɂ���
	m_Bullets.clear();
}

void BulletManager::ProcessInput()
{
	for (auto* const bullet : m_Bullets)
	{
		// null�`�F�b�N
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::ProcessInput bullet��nullptr");
			continue;
		}
		// ��A�N�e�B�u�Ȃ�X�V���Ȃ�
		if (bullet->GetActive() == false)
		{
			continue;
		}
		bullet->ProcessInput();
	}
}

void BulletManager::Update()
{
	for (auto* const bullet : m_Bullets)
	{
		// null�`�F�b�N
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Update bullet��nullptr");
			continue;
		}
		// ��A�N�e�B�u�Ȃ�X�V���Ȃ�
		if (bullet->GetActive() == false)
		{
			continue;
		}
		bullet->Update();
	}

	// �����[�h����
	if (m_CurrentBullet != m_MaxBullet)
	{
		Reload();
	}
}
