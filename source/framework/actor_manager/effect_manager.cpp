
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
			Logger::GetInstance().SetLog("BulletManager::UseBullet effectがnullptr");
			continue;
		}

		if (effect->GetActive() == false)
		{
			// ポジション入力
			effect->SetPosition(position);
			// アクティブ処理
			effect->SetActive(true);
			// リターンエフェクト入力
			retEffect = effect;
			break;
		}
	}

	return retEffect;
}

void EffectManager::LoadData()
{
	// アクターの読み込み処理
	for (auto* const effect : m_Effects)
	{
		// nullチェック
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::LoadData effectがnullptr");
			continue;
		}
		effect->LoadData();
	}
}

void EffectManager::Init()
{
	// アクターの初期化処理
	for (auto* const effect : m_Effects)
	{
		// nullチェック
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Init effectがnullptr");
			continue;
		}
		effect->Init();
		effect->SetActive(false);
	}
}

void EffectManager::Uninit()
{
	//アクター終了処理
	for (auto* const effect : m_Effects)
	{
		// nullチェック
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit effectがnullptr");
			continue;
		}
		effect->Uninit();
	}

	// アクター削除
	for (auto* effect : m_Effects)
	{
		// nullチェック
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Uninit effectがnullptr");
			continue;
		}
		delete effect;
		effect = nullptr;
	}
	//中身を空にする
	m_Effects.clear();
}

void EffectManager::ProcessInput()
{
	for (auto* const effect : m_Effects)
	{
		// nullチェック
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::ProcessInput effectがnullptr");
			continue;
		}
		// 非アクティブなら更新しない
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
		// nullチェック
		if (effect == nullptr)
		{
			Logger::GetInstance().SetLog("BulletManager::Update effectがnullptr");
			continue;
		}
		// 非アクティブなら更新しない
		if (effect->GetActive() == false)
		{
			continue;
		}
		effect->Update();
	}
}
