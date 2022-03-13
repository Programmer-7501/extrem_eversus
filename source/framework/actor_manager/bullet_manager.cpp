
#include"../../system/main.h"
#include "bullet_manager.h"
#include"../../particle/particle_manager.h"
#include"../../framework/actor/mobilesuit/mobilesuit.h"

namespace BulletManagerData
{
	//! デフォルトのリロードカウント
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
	// リターンバレット変数
	Bullet* retBullet = nullptr;

	// 残弾数がないならリターン
	if (m_CurrentBullet <= 0)
	{
		return retBullet;
	}


	for (auto* const bullet : m_Bullets)
	{
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::UseBullet bulletがnullptr");
			continue;
		}

		if (bullet->GetActive() == false)
		{
			// ポジション入力
			bullet->SetPosition(position);
			// 速度入力
			bullet->SetVelocity(velocity);
			// ターゲット入力
			bullet->SetTargetMobileSuit(target);
			// アクティブ処理
			bullet->SetActive(true);
			// 残弾数を減らす
			m_CurrentBullet--;
			// リターンバレット入力
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
		// リロードカウントアップ
		m_ReloadCount++;
		// リロードカウントがリロードタイムを超えたら
		if (m_ReloadCount > m_ReloadTime)
		{
			// 現在の残弾数を増やす
			m_CurrentBullet++;
			// リロードカウントを初期化
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
		// nullチェック
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::SetLandingEffectManager bulletがnullptr");
			continue;
		}
		bullet->SetLandingEffectManager(effectManager);
	}
}

void BulletManager::LoadData()
{
	// アクターの読み込み処理
	for (auto* const bullet : m_Bullets)
	{
		// nullチェック
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::LoadData bulletがnullptr");
			continue;
		}
		bullet->LoadData();
	}
}

void BulletManager::Init()
{
	// アクターの初期化処理
	for (auto* const bullet : m_Bullets)
	{
		// nullチェック
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Init bulletがnullptr");
			continue;
		}
		bullet->Init();
		bullet->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
		bullet->SetActive(false);
	}
}

void BulletManager::Uninit()
{
	//アクター終了処理
	for (auto* const bullet : m_Bullets)
	{
		// nullチェック
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit bulletがnullptr");
			continue;
		}
		bullet->Uninit();
	}

	// アクター削除
	for (auto* bullet : m_Bullets)
	{
		// nullチェック
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit bulletがnullptr");
			continue;
		}
		delete bullet;
		bullet = nullptr;
	}
	//中身を空にする
	m_Bullets.clear();
}

void BulletManager::ProcessInput()
{
	for (auto* const bullet : m_Bullets)
	{
		// nullチェック
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::ProcessInput bulletがnullptr");
			continue;
		}
		// 非アクティブなら更新しない
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
		// nullチェック
		if (bullet == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Update bulletがnullptr");
			continue;
		}
		// 非アクティブなら更新しない
		if (bullet->GetActive() == false)
		{
			continue;
		}
		bullet->Update();
	}

	// リロード処理
	if (m_CurrentBullet != m_MaxBullet)
	{
		Reload();
	}
}
