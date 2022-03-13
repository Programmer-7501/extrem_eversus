
#include"../../../../system/main.h"
#include "game_clear_scene_ui.h"
#include"../../../../directx/resource/texture_manager.h"
#include"../../../../system/input_manager.h"
#include"../../../component/sprite_component/sprite_component.h"


/**
* @namespace GameOverSceneUIData
* @brief ゲームオーバーシーンUI情報
*/
namespace GameClearSceneUIData
{
	//! 背景テクスチャネーム
	static const char* k_BackGroundTextureFileName = "asset/texture/game_clear_background.png";

	//! 背景テクスチャポジション
	static const Conv_XM::Vector2f k_BackGroundTexturePosition = Conv_XM::Vector2f(480.0f, 270.0f);

	//! 背景テクスチャサイズ
	static const Conv_XM::Vector2f k_BackGroundTextureSize = Conv_XM::Vector2f(960.0f, 540.0f);

	//! YouLoseテクスチャファイルネーム
	static const char* k_YouWinTextureFileName = "asset/texture/youwin.png";

	//! YouLoseテクスチャポジション
	static const Conv_XM::Vector2f k_YouWinTexturePosition = Conv_XM::Vector2f(480.0f, 160.0f);

	//! YouLoseテクスチャサイズ
	static const Conv_XM::Vector2f k_YouWinTextureSize = Conv_XM::Vector2f(512.0f, 256.0f);

	//! PressEnterテクスチャファイルネーム
	static const char* k_PressEnterTextureFileName = "asset/texture/title004.png";

	//! PressEnterテクスチャポジション
	static const Conv_XM::Vector2f k_PressEnterTexturePosition = Conv_XM::Vector2f(480.0f, 440.0f);

	//! PressEnterテクスチャサイズ
	static const Conv_XM::Vector2f k_PressEnterTextureSize = Conv_XM::Vector2f(512.0f, 128.0f);
}

void GameClearSceneUI::LoadTexture()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// 背景テクスチャ破棄
	textureManager.Load(GameClearSceneUIData::k_BackGroundTextureFileName);

	// YouLoseテクスチャ読み込み
	textureManager.Load(GameClearSceneUIData::k_YouWinTextureFileName);

	// PressEnterテクスチャ読み込み
	textureManager.Load(GameClearSceneUIData::k_PressEnterTextureFileName);
}

void GameClearSceneUI::UnloadTexture()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// 背景テクスチャ破棄
	textureManager.Unload(GameClearSceneUIData::k_BackGroundTextureFileName);

	// YouLoseテクスチャ読み込み
	textureManager.Unload(GameClearSceneUIData::k_YouWinTextureFileName);

	// PressEnterテクスチャ読み込み
	textureManager.Unload(GameClearSceneUIData::k_PressEnterTextureFileName);
}

GameClearSceneUI::GameClearSceneUI()
	: m_BackGroundSprite(nullptr)
	, m_YouWinSprite(nullptr)
	, m_PressEnterSprite(nullptr)
	, m_FrameCount(0)
{
}

GameClearSceneUI::~GameClearSceneUI()
{
}

void GameClearSceneUI::LoadActor()
{
	// スプライトコンポーネント作成
	m_BackGroundSprite = CreateComponent<SpriteComponent>();

	// youloseスプライトコンポーネント作成
	m_YouWinSprite = CreateComponent<SpriteComponent>();

	// PressEnterのスプライトコンポーネント作成
	m_PressEnterSprite = CreateComponent<SpriteComponent>();
}

void GameClearSceneUI::InitActor()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// テクスチャの取得
	ID3D11ShaderResourceView* toBackGroundSRV = textureManager.GetSRV(GameClearSceneUIData::k_BackGroundTextureFileName);
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
	m_BackGroundSprite->SetPosition(GameClearSceneUIData::k_BackGroundTexturePosition);
	m_BackGroundSprite->SetSize(GameClearSceneUIData::k_BackGroundTextureSize);

	// テクスチャの取得
	ID3D11ShaderResourceView* youLoseSRV = textureManager.GetSRV(GameClearSceneUIData::k_YouWinTextureFileName);
	if (youLoseSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor youLoseSRVがnullptr");
		return;
	}
	if (m_YouWinSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_YouLoseSpriteがnullptr");
		return;
	}

	m_YouWinSprite->SetSRV(youLoseSRV);
	// トランスフォーム入力
	m_YouWinSprite->SetPosition(GameClearSceneUIData::k_YouWinTexturePosition);
	m_YouWinSprite->SetSize(GameClearSceneUIData::k_YouWinTextureSize);


	// テクスチャの取得
	ID3D11ShaderResourceView* pressEnterSRV = textureManager.GetSRV(GameClearSceneUIData::k_PressEnterTextureFileName);
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
	m_PressEnterSprite->SetPosition(GameClearSceneUIData::k_PressEnterTexturePosition);
	m_PressEnterSprite->SetSize(GameClearSceneUIData::k_PressEnterTextureSize);
}

void GameClearSceneUI::UpdateActor()
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
