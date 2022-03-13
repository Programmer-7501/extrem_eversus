
#include"../../../../system/main.h"
#include "radar.h"
#include"../../mobilesuit/mobilesuit.h"
#include"../../../../directx/resource/texture_manager.h"
#include"../../../component/sprite_component/sprite_component.h"
#include"../../../component/sprite_component/changeable_sprite_component.h"

namespace RadarData
{
	//! レーダーテクスチャファイルネーム
	static const char* k_RadarBackGroundTextureName = "asset/texture/radar.png";

	//! レーダー背景テクスチャポジション
	static const Conv_XM::Vector2f k_RadarBackGroundTexturePosition = Conv_XM::Vector2f(400.0f,100.0f);

	//! レーダー背景テクスチャサイズ
	static const Conv_XM::Vector2f k_RadarBackGroundTextureSize = Conv_XM::Vector2f(100.0f, 100.0f);

	//! レーダー駒テクスチャファイルネーム
	static const char* k_RadarPieceTextureName = "asset/texture/radar_piece.png";

	//! レーダー駒テクスチャサイズ
	static const Conv_XM::Vector2f k_RadarPieceTextureSize = Conv_XM::Vector2f(30.0f, 30.0f);
}

void Radar::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	// レーダーテクスチャ読み込み
	textureManager.Load(RadarData::k_RadarBackGroundTextureName);

	// レーダー駒テクスチャ読み込み
	textureManager.Load(RadarData::k_RadarPieceTextureName);
}

void Radar::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	// レーダーテクスチャ破棄
	textureManager.Unload(RadarData::k_RadarBackGroundTextureName);

	// レーダー駒テクスチャ破棄
	textureManager.Unload(RadarData::k_RadarPieceTextureName);
}

Radar::Radar()
	:m_PlayerMobileSuit(nullptr)
	, m_AllienMobileSuit(nullptr)
	, m_Enemy001MobileSuit(nullptr)
	, m_Enemy002MobileSuit(nullptr)
	, m_BackGroundSpriteComponent(nullptr)
	, m_PlayerPieceSpriteComponent(nullptr)
	, m_AllienPieceSpriteComponent(nullptr)
	, m_Enemy001PieceSpriteComponent(nullptr)
	, m_Enemy002PieceSpriteComponent(nullptr)
{

}

Radar::~Radar()
{
}

void Radar::LoadActor()
{
	// 背景画像スプライトコンポーネント作成
	m_BackGroundSpriteComponent = CreateComponent<SpriteComponent>();

	//駒スプライトコンポーネント作成
	m_PlayerPieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
	m_AllienPieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
	m_Enemy001PieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
	m_Enemy002PieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
}

void Radar::InitActor()
{
	// nullチェック
	if (m_BackGroundSpriteComponent == nullptr ||
		m_PlayerPieceSpriteComponent == nullptr ||
		m_AllienPieceSpriteComponent == nullptr ||
		m_Enemy001PieceSpriteComponent == nullptr ||
		m_Enemy002PieceSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Radar::InitActor nullptr");
		return;
	}

	TextureManager& textureManager = TextureManager::GetInstance();

	//　テクスチャ取得
	ID3D11ShaderResourceView* radarBackGroundSRV = textureManager.GetSRV(RadarData::k_RadarBackGroundTextureName);
	if (radarBackGroundSRV == nullptr)
	{
		Logger::GetInstance().SetLog("Radar::InitActor radarBackGroundSRVがnullptr");
		return;
	}

	m_BackGroundSpriteComponent->SetSRV(radarBackGroundSRV);
	m_BackGroundSpriteComponent->SetPosition(RadarData::k_RadarBackGroundTexturePosition);
	m_BackGroundSpriteComponent->SetSize(RadarData::k_RadarBackGroundTextureSize);

	//　テクスチャ取得
	ID3D11ShaderResourceView* radarPieceSRV = textureManager.GetSRV(RadarData::k_RadarPieceTextureName);
	if (radarBackGroundSRV == nullptr)
	{
		Logger::GetInstance().SetLog("Radar::InitActor radarPieceSRVがnullptr");
		return;
	}

	m_PlayerPieceSpriteComponent->SetSRV(radarPieceSRV);
	m_PlayerPieceSpriteComponent->SetSize(RadarData::k_RadarPieceTextureSize);

	m_AllienPieceSpriteComponent->SetSRV(radarPieceSRV);
	m_AllienPieceSpriteComponent->SetSize(RadarData::k_RadarPieceTextureSize);

	m_Enemy001PieceSpriteComponent->SetSRV(radarPieceSRV);
	m_Enemy001PieceSpriteComponent->SetSize(RadarData::k_RadarPieceTextureSize);

	m_Enemy002PieceSpriteComponent->SetSRV(radarPieceSRV);
	m_Enemy002PieceSpriteComponent->SetSize(RadarData::k_RadarPieceTextureSize);
}

void Radar::UpdateActor()
{
}
