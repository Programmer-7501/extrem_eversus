
#include"../../../system/main.h"
#include "game_manager.h"
#include"cost_manager.h"
#include"../mobilesuit/mobilesuit.h"
#include"../ui/timelimit_ui.h"
#include"../../component/sprite_component/number_sprite_component.h"
#include"../../../directx/resource/texture_manager.h"

namespace GameManagerData
{
	//! デフォルトのリスポーン時間
	static constexpr int k_RespawnTime = 120;

	//! 数字スプライトのポジション
	static Conv_XM::Vector2f k_NumberSpritePosition = Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, 400.0f);

	//! 数字スプライトのサイズ
	static Conv_XM::Vector2f k_NumberSpriteSize = Conv_XM::Vector2f(100.0f, 100.0f);

	//! Standbyテクスチャファイルネーム
	static const char* k_StandbyTextureFileName = "asset/texture/standby.png";

	//! 数字スプライトのポジション
	static Conv_XM::Vector2f k_StandbyTexturePosition = Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, 200.0f);

	//! 数字スプライトのサイズ
	static Conv_XM::Vector2f k_StandbyTextureSize = Conv_XM::Vector2f(400.0f, 200.0f);

	//! Goテクスチャファイルネーム
	static const char* k_GoTextureFileName = "asset/texture/go.png";

	//! 数字スプライトのポジション
	static Conv_XM::Vector2f k_GoTexturePosition = Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, 200.0f);

	//! 数字スプライトのサイズ
	static Conv_XM::Vector2f k_GoTextureSize = Conv_XM::Vector2f(400.0f, 200.0f);
}

void GameManager::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Load(GameManagerData::k_StandbyTextureFileName);
	textureManager.Load(GameManagerData::k_GoTextureFileName);
}

void GameManager::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Unload(GameManagerData::k_StandbyTextureFileName);
	textureManager.Unload(GameManagerData::k_GoTextureFileName);
}

GameManager::GameManager()
	:m_CostManager(nullptr)
	, m_MobileSuit{}
	, m_RespawnTime{}
	, m_CountFrame(0)
	, m_CountDownSpriteComponent(nullptr)
	, m_SceneChange(SCENE_CHANGE_NONE)
{
	for (int i = 0; i < MAXMOBILESUIT; i++)
	{
		m_MobileSuit[i] = nullptr;
		m_RespawnTime[i] = 0;
	}
}

GameManager::~GameManager()
{
}

void GameManager::LoadActor()
{
	// 数字スプライトコンポーネント作成
	m_CountDownSpriteComponent = CreateComponent<NumberSpriteComponent>();

	// Standbyスプライトコンポーネント作成
	m_StandbySpriteComponent = CreateComponent<SpriteComponent>();

	// Goスプライトコンポーネント作成
	m_GoSpriteComponent = CreateComponent<SpriteComponent>();
}

void GameManager::InitActor()
{
	// nullチェック
	if (m_CountDownSpriteComponent == nullptr || m_StandbySpriteComponent == nullptr || m_GoSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameManager::InitActor nullptr");
		return;
	}

	// カウントダウンスプライト設定
	m_CountDownSpriteComponent->SetPosition(GameManagerData::k_NumberSpritePosition);
	m_CountDownSpriteComponent->SetSize(GameManagerData::k_NumberSpriteSize);
	m_CountDownSpriteComponent->SetNumber(3);

	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// スタンバイスプライト設定
	ID3D11ShaderResourceView* standbySRV = textureManager.GetSRV(GameManagerData::k_StandbyTextureFileName);
	if (standbySRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameManager::InitActor standbySRVがnullptr");
		return;
	}
	m_StandbySpriteComponent->SetSRV(standbySRV);
	m_StandbySpriteComponent->SetPosition(GameManagerData::k_StandbyTexturePosition);
	m_StandbySpriteComponent->SetSize(GameManagerData::k_StandbyTextureSize);

	// ゴースプライト設定
	ID3D11ShaderResourceView* goSRV = textureManager.GetSRV(GameManagerData::k_GoTextureFileName);
	if (goSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameManager::InitActor goSRVがnullptr");
		return;
	}
	m_GoSpriteComponent->SetSRV(goSRV);
	m_GoSpriteComponent->SetPosition(GameManagerData::k_GoTexturePosition);
	m_GoSpriteComponent->SetSize(GameManagerData::k_GoTextureSize);
	m_GoSpriteComponent->SetVisible(false);
}

void GameManager::UpdateActor()
{
	// タイムリミットになったら負け
	if (m_TimeLimitUI->GetTimeLeft() <= 0)
	{
		m_SceneChange = SCENE_CHANGE_LOSE;
	}
	// リスポーン
	RespawnManager();

	// スタート開始時
	GameStartManager();

	// カウントアップ
	m_CountFrame++;
}

void GameManager::GameStartManager()
{
	if (m_CountDownSpriteComponent == nullptr || m_StandbySpriteComponent == nullptr || m_GoSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateComponent::GameStartManager nullptr");
		return;
	}

	if (m_CountFrame < 60)
	{
		m_CountDownSpriteComponent->SetNumber(3);
	}
	else if (m_CountFrame < 60 * 2)
	{
		m_CountDownSpriteComponent->SetNumber(2);
	}
	else if (m_CountFrame < 60 * 3)
	{
		m_CountDownSpriteComponent->SetNumber(1);
	}
	else if (m_CountFrame < 60 * 4)
	{
		m_StandbySpriteComponent->SetVisible(false);
		m_GoSpriteComponent->SetVisible(true);
		m_CountDownSpriteComponent->SetNumber(0);
	}
	else
	{
		// スプライト非表示
		m_GoSpriteComponent->SetVisible(false);
		m_CountDownSpriteComponent->SetVisible(false);

		// モビルスーツを動かす
		for (int i = 0; i < MAXMOBILESUIT; i++)
		{
			if (m_MobileSuit[i] == nullptr)
			{
				Logger::GetInstance().SetLog("MobileSuitStateComponent::GameStartManager m_MobileSuitがnullptr");
				return;
			}
			m_MobileSuit[i]->SetIsMoveFlag(true);
		}

		//　タイム計測開始
		m_TimeLimitUI->SetMesureStartFlag(true);
	}

}

void GameManager::RespawnManager()
{
	if (m_CostManager == nullptr || m_SceneChange != SCENE_CHANGE_NONE)
	{
		return;
	}


	// 敵のコストがゼロになったら
	if (m_CostManager->GetEnemyCost() <= 0)
	{
		m_SceneChange = SCENE_CHANGE_WIN;
	}
	// 味方のコストがゼロになったら
	if (m_CostManager->GetMyCost() <= 0)
	{
		m_SceneChange = SCENE_CHANGE_LOSE;
	}


	for (int i = 0; i < MAXMOBILESUIT; i++)
	{
		if (m_MobileSuit[i] == nullptr)
		{
			continue;
		}

		// モビルスーツが非アクティブなら
		if (m_MobileSuit[i]->GetActive() == false)
		{
			// リスポーン時間を増やす
			m_RespawnTime[i]++;

			// リスポーン時間が超えたら
			if (m_RespawnTime[i] > GameManagerData::k_RespawnTime)
			{
				m_MobileSuit[i]->SetActive(true);
				m_MobileSuit[i]->Reset();
				m_MobileSuit[i]->SetPosition(Conv_XM::Vector3f(rand() % 40, 20.0f, rand() % 40));
				m_RespawnTime[i] = 0;
			}
		}

	}
}
