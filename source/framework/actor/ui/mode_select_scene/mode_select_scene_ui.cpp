
#include"../../../../system/main.h"
#include "mode_select_scene_ui.h"
#include"../../../component/sprite_component/changeable_sprite_component.h"
#include"../../../../directx/resource/texture_manager.h"
#include"../../../../system/input_manager.h"
#include"../../../../system/audio_manager.h"
#include"../mode_select_scene/mode_select_scene_ui.h"

/**
* @namespace ModeSelectSceneUIData
* @brief モード選択シーンUI情報
*/
namespace ModeSelectSceneUIData
{
	//! 背景テクスチャネーム
	static const char* k_BackGroundTextureFileName = "asset/texture/game_select_scene_001.png";


	//! ゲームシーンへテクスチャネーム
	static const char* k_ToGameSceneTextureFileName = "asset/texture/gamescene.png";
	
	//! ゲームシーンへテクスチャポジション
	static const Conv_XM::Vector2f k_ToGameSceneTexturePosition = Conv_XM::Vector2f(480.0f, 50.0f);

	//! ゲームシーンへテクスチャサイズ
	static const Conv_XM::Vector2f k_ToGameSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);


	//! デモシーンへテクスチャネーム
	static const char* k_ToShaderDemoSceneTextureFileName = "asset/texture/shaderdemoscene.png";

	//! デモシーンへテクスチャポジション
	static const Conv_XM::Vector2f k_ToShaderDemoSceneTexturePosition = Conv_XM::Vector2f(480.0f, 200.0f);

	//! デモシーンへテクスチャサイズ
	static const Conv_XM::Vector2f k_ToShaderDemoSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);


	//! AIデモシーンへテクスチャネーム
	static const char* k_ToAIDemoSceneTextureFileName = "asset/texture/aidemoscene.png";

	//! AIデモシーンへテクスチャポジション
	static const Conv_XM::Vector2f k_ToAIDemoSceneTexturePosition = Conv_XM::Vector2f(480.0f, 350.0f);

	//! AIデモシーンへテクスチャサイズ
	static const Conv_XM::Vector2f k_ToAIDemoSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);


	//! グラフ作成シーンへテクスチャネーム
	static const char* k_ToGraphMakeSceneTextureFileName = "asset/texture/graphmakescene.png";

	//! AIデモシーンへテクスチャポジション
	static const Conv_XM::Vector2f k_ToGraphMakeSceneTexturePosition = Conv_XM::Vector2f(480.0f, 500.0f);

	//! AIデモシーンへテクスチャサイズ
	static const Conv_XM::Vector2f k_GraphMakeSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);



	//! オンテクスチャカラー
	static const Conv_XM::Vector4f k_OnTextureColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

	//! オフテクスチャカラー
	static const Conv_XM::Vector4f k_OffTextureColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 0.3f);


	//! 決定ボタンSE
	static const char* DecideSE = "asset/sound/SE/Decide002SE.wav";

	//! カーソルSE
	static const char* CursorSE = "asset/sound/SE/CursorSE.wav";
}

void ModeSelectSceneUI::LoadTexture()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// 背景テクスチャ破棄
	textureManager.Load(ModeSelectSceneUIData::k_BackGroundTextureFileName);

	// ゲームシーンへテクスチャ読み込み
	textureManager.Load(ModeSelectSceneUIData::k_ToGameSceneTextureFileName);

	// デモシーンへテクスチャ読み込み
	textureManager.Load(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureFileName);

	// AIデモシーンへテクスチャ読み込み
	textureManager.Load(ModeSelectSceneUIData::k_ToAIDemoSceneTextureFileName);

	// グラフ作成シーンへテクスチャ読み込み
	textureManager.Load(ModeSelectSceneUIData::k_ToGraphMakeSceneTextureFileName);
}

void ModeSelectSceneUI::UnloadTexture()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// 背景テクスチャ破棄
	textureManager.Unload(ModeSelectSceneUIData::k_BackGroundTextureFileName);

	// ゲームシーンへテクスチャ破棄
	textureManager.Unload(ModeSelectSceneUIData::k_ToGameSceneTextureFileName);

	// デモシーンへテクスチャ破棄
	textureManager.Unload(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureFileName);

	// AIデモシーンへテクスチャ破棄
	textureManager.Unload(ModeSelectSceneUIData::k_ToAIDemoSceneTextureFileName);

	// グラフ作成シーンへテクスチャ破棄
	textureManager.Unload(ModeSelectSceneUIData::k_ToGraphMakeSceneTextureFileName);
}

void ModeSelectSceneUI::LoadSound()
{
	// オーディオマネージャー取得
	AudioManager& audioManager = AudioManager::GetInstance();

	audioManager.Load(ModeSelectSceneUIData::CursorSE);
	audioManager.Load(ModeSelectSceneUIData::DecideSE);
}

void ModeSelectSceneUI::UnloadSound()
{
	// オーディオマネージャー取得
	AudioManager& audioManager = AudioManager::GetInstance();

	audioManager.Unload(ModeSelectSceneUIData::CursorSE);
	audioManager.Unload(ModeSelectSceneUIData::DecideSE);
}

ModeSelectSceneUI::ModeSelectSceneUI()
	: m_ModeSelect(SS_NONE)
	, m_SendMode(SS_NONE)
	, m_ToGameSceneSprite(nullptr)
	, m_ToShaderDemoSceneSprite(nullptr)
	, m_ToAIDemoSceneSprite(nullptr)
	, m_ToMakeGraphSceneSprite(nullptr)
{
}

ModeSelectSceneUI::~ModeSelectSceneUI()
{
}

void ModeSelectSceneUI::LoadActor()
{
	// スプライトコンポーネント作成
	m_BackGroundSprite = CreateComponent<SpriteComponent>();

	// 変更可能スプライトコンポーネント作成
	m_ToGameSceneSprite = CreateComponent<ChangeableSpriteComponent>();

	// 変更可能スプライトコンポーネント作成
	m_ToShaderDemoSceneSprite = CreateComponent<ChangeableSpriteComponent>();

	m_ToAIDemoSceneSprite = CreateComponent<ChangeableSpriteComponent>();

	m_ToMakeGraphSceneSprite = CreateComponent<ChangeableSpriteComponent>();
}

void ModeSelectSceneUI::InitActor()
{
	// インスタンス取得
	TextureManager& textureManager = TextureManager::GetInstance();

	// テクスチャの取得
	ID3D11ShaderResourceView* toBackGroundSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_BackGroundTextureFileName);
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
	m_BackGroundSprite->SetPosition(Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, SystemData::k_ScreenHeight / 2.0f));
	m_BackGroundSprite->SetSize(Conv_XM::Vector2f(SystemData::k_ScreenWidth, SystemData::k_ScreenHeight));




	// テクスチャの取得
	ID3D11ShaderResourceView* toGameSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToGameSceneTextureFileName);
	if (toGameSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toGameSRVがnullptr");
		return;
	}
	if (m_ToGameSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToGameSceneSpriteがnullptr");
		return;
	}
	m_ToGameSceneSprite->SetSRV(toGameSRV);
	// トランスフォーム入力
	m_ToGameSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToGameSceneTexturePosition);
	m_ToGameSceneSprite->SetSize(ModeSelectSceneUIData::k_ToGameSceneTextureSize);



	// テクスチャの取得
	ID3D11ShaderResourceView* toDemoSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureFileName);
	if (toDemoSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toDemoSRVがnullptr");
		return;
	}
	if (m_ToShaderDemoSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToShaderDemoSceneSpriteがnullptr");
		return;
	}
	m_ToShaderDemoSceneSprite->SetSRV(toDemoSRV);
	// トランスフォーム入力
	m_ToShaderDemoSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToShaderDemoSceneTexturePosition);
	m_ToShaderDemoSceneSprite->SetSize(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureSize);





	// テクスチャの取得
	ID3D11ShaderResourceView* toAIDemoSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToAIDemoSceneTextureFileName);
	if (toAIDemoSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toAIDemoSRVがnullptr");
		return;
	}
	if (m_ToAIDemoSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToAIDemoSceneSpriteがnullptr");
		return;
	}
	m_ToAIDemoSceneSprite->SetSRV(toAIDemoSRV);
	// トランスフォーム入力
	m_ToAIDemoSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToAIDemoSceneTexturePosition);
	m_ToAIDemoSceneSprite->SetSize(ModeSelectSceneUIData::k_ToAIDemoSceneTextureSize);


	// テクスチャの取得
	ID3D11ShaderResourceView* toGraphMakeSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToGraphMakeSceneTextureFileName);
	if (toGraphMakeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toGraphMakeSRVがnullptr");
		return;
	}
	if (m_ToMakeGraphSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToMakeGraphSceneSpriteがnullptr");
		return;
	}
	m_ToMakeGraphSceneSprite->SetSRV(toGraphMakeSRV);
	// トランスフォーム入力
	m_ToMakeGraphSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToGraphMakeSceneTexturePosition);
	m_ToMakeGraphSceneSprite->SetSize(ModeSelectSceneUIData::k_GraphMakeSceneTextureSize);

	// モード初期化
	m_ModeSelect = SS_GAMESCENE;

	// 初期カラー
	m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
	m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
	m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
	m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
}

void ModeSelectSceneUI::ProcessInputActor()
{
	// インスタンス取得
	InputManager& inputManager = InputManager::GetInstance();

	if (m_ToGameSceneSprite == nullptr || m_ToShaderDemoSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::ProcessInputActor nullptr");
		return;
	}

	if (m_SendMode != SS_NONE)
	{
		return;
	}

	bool isDecide = false;
	bool isCursor = false;

	switch (m_ModeSelect)
	{
	case ModeSelectSceneUI::SS_NONE:
		break;
	case ModeSelectSceneUI::SS_GAMESCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_BACK))
		{
			m_ModeSelect = SS_SHADERDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_GAMESCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_SHADERDEMOSCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_FRONT))
		{
			m_ModeSelect = SS_GAMESCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_BACK))
		{
			m_ModeSelect = SS_AIDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_SHADERDEMOSCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_AIDEMOSCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_FRONT))
		{
			m_ModeSelect = SS_SHADERDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_BACK))
		{
			m_ModeSelect = SS_GRAPHMAKESCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_AIDEMOSCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_GRAPHMAKESCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_FRONT))
		{
			m_ModeSelect = SS_AIDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_GRAPHMAKESCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_MAX:
		break;
	default:
		break;
	}

	// オーディオマネージャー取得
	AudioManager& audioManager = AudioManager::GetInstance();
	if (isDecide == true)
	{
		audioManager.Play(ModeSelectSceneUIData::DecideSE, false);
	}
	if (isCursor == true)
	{
		audioManager.Play(ModeSelectSceneUIData::CursorSE, false);
	}

}