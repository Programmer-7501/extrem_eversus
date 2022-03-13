
#include"../../../../system/main.h"
#include "game_over_scene_ui.h"
#include"../../../../directx/resource/texture_manager.h"
#include"../../../../system/input_manager.h"
#include"../../../component/sprite_component/sprite_component.h"

/**
* @namespace GameOverSceneUIData
* @brief ゲームオーバーシーンUI情報
*/
namespace GameOverSceneUIData
{
	//! 背景テクスチャネーム
	static const char* k_BackGroundTextureFileName = "asset/texture/game_over_background.png";

	//! 背景テクスチャポジション
	static const Conv_XM::Vector2f k_BackGroundTexturePosition = Conv_XM::Vector2f(480.0f, 270.0f);

	//! 背景テクスチャサイズ
	static const Conv_XM::Vector2f k_BackGroundTextureSize = Conv_XM::Vector2f(960.0f, 540.0f);

	//! YouLoseテクスチャファイルネーム
	static const char* k_YouLoseTextureFileName = "asset/texture/youlose.png";

	//! YouLoseテクスチャポジション
	static const Conv_XM::Vector2f k_YouLoseTexturePosition = Conv_XM::Vector2f(480.0f, 160.0f);

	//! YouLoseテクスチャサイズ
	static const Conv_XM::Vector2f k_YouLoseTextureSize = Conv_XM::Vector2f(512.0f, 256.0f);

	//! PressEnterテクスチャファイルネーム
	static const char* k_PressEnterTextureFileName = "asset/texture/title004.png";

	//! PressEnterテクスチャポジション
	static const Conv_XM::Vector2f k_PressEnterTexturePosition = Conv_XM::Vector2f(480.0f, 440.0f);

	//! PressEnterテクスチャサイズ
	static const Conv_XM::Vector2f k_PressEnterTextureSize = Conv_XM::Vector2f(512.0f, 128.0f);
}


void GameOverSceneUI::LoadTexture()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// 背景テクスチャ破棄
	textureManager.Load(GameOverSceneUIData::k_BackGroundTextureFileName);

	// YouLoseテクスチャ読み込み
	textureManager.Load(GameOverSceneUIData::k_YouLoseTextureFileName);

	// PressEnterテクスチャ読み込み
	textureManager.Load(GameOverSceneUIData::k_PressEnterTextureFileName);
}

void GameOverSceneUI::UnloadTexture()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// 背景テクスチャ破棄
	textureManager.Unload(GameOverSceneUIData::k_BackGroundTextureFileName);

	// YouLoseテクスチャ読み込み
	textureManager.Unload(GameOverSceneUIData::k_YouLoseTextureFileName);

	// PressEnterテクスチャ読み込み
	textureManager.Unload(GameOverSceneUIData::k_PressEnterTextureFileName);
}

GameOverSceneUI::GameOverSceneUI()
	: m_BackGroundSprite(nullptr)
	, m_YouLoseSprite(nullptr)
	, m_PressEnterSprite(nullptr)
	, m_FrameCount(0)
{
}

GameOverSceneUI::~GameOverSceneUI()
{
}

void GameOverSceneUI::LoadActor()
{
	// スプライトコンポーネント作成
	m_BackGroundSprite = CreateComponent<SpriteComponent>();

	// youloseスプライトコンポーネント作成
	m_YouLoseSprite = CreateComponent<SpriteComponent>();

	// PressEnterのスプライトコンポーネント作成
	m_PressEnterSprite = CreateComponent<SpriteComponent>();
}

void GameOverSceneUI::InitActor()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// テクスチャの取得
	ID3D11ShaderResourceView* toBackGroundSRV = textureManager.GetSRV(GameOverSceneUIData::k_BackGroundTextureFileName);
	if (toBackGroundSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toGameSRVがnullptr");
		return;
	}
	if (m_BackGroundSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_BackGroundSpriteがnullptr");
		return;
	}

	m_BackGroundSprite->SetSRV(toBackGroundSRV);
	// トランスフォーム入力
	m_BackGroundSprite->SetPosition(GameOverSceneUIData::k_BackGroundTexturePosition);
	m_BackGroundSprite->SetSize(GameOverSceneUIData::k_BackGroundTextureSize);

	// テクスチャの取得
	ID3D11ShaderResourceView* youLoseSRV = textureManager.GetSRV(GameOverSceneUIData::k_YouLoseTextureFileName);
	if (youLoseSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor youLoseSRVがnullptr");
		return;
	}
	if (m_YouLoseSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_YouLoseSpriteがnullptr");
		return;
	}

	m_YouLoseSprite->SetSRV(youLoseSRV);
	// トランスフォーム入力
	m_YouLoseSprite->SetPosition(GameOverSceneUIData::k_YouLoseTexturePosition);
	m_YouLoseSprite->SetSize(GameOverSceneUIData::k_YouLoseTextureSize);


	// テクスチャの取得
	ID3D11ShaderResourceView* pressEnterSRV = textureManager.GetSRV(GameOverSceneUIData::k_PressEnterTextureFileName);
	if (pressEnterSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor pressEnterSRVがnullptr");
		return;
	}
	if (m_PressEnterSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_PressEnterSpriteがnullptr");
		return;
	}

	m_PressEnterSprite->SetSRV(pressEnterSRV);
	// トランスフォーム入力
	m_PressEnterSprite->SetPosition(GameOverSceneUIData::k_PressEnterTexturePosition);
	m_PressEnterSprite->SetSize(GameOverSceneUIData::k_PressEnterTextureSize);
}

void GameOverSceneUI::UpdateActor()
{
	if (m_PressEnterSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::UpdateActor m_PressEnterSpriteがnullptr");
		return;
	}
	if (m_FrameCount % 100 < 80)
	{
		m_PressEnterSprite->SetVisible(true);
	}
	else
	{
		m_PressEnterSprite->SetVisible(false);
	}
	// カウントアップ
	m_FrameCount++;
}