
#include"../../../system/main.h"
#include "boost_gauge_ui.h"
#include"../../component/sprite_component/changeable_sprite_component.h"
#include"../../component/sprite_component/gauge_sprite_component.h"
#include"../../component/sprite_component/sprite_component.h"
#include"../../component/mobilesuit_component/boost_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../../directx/resource/texture_manager.h"

namespace BoostGaugeUIData
{
	//! ブーストが半分以上の時の色
	const Conv_XM::Vector4f k_DefaultBoostColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

	//! ブーストが半分以下の時の色
	const Conv_XM::Vector4f k_HalfBoostColor = Conv_XM::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);

	//! ブーストが1/4の時の色
	const Conv_XM::Vector4f k_QuarterBoostColor = Conv_XM::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

	//! ポジション
	const Conv_XM::Vector2f k_GaugePosition = Conv_XM::Vector2f(510.0f, 500.0f);
	//! サイズ
	const Conv_XM::Vector2f k_GaugeSize = Conv_XM::Vector2f(100.0f, 25.0f);

	//! ポジション
	const Conv_XM::Vector2f k_BoostLetterPosition = Conv_XM::Vector2f(510.0f, 530.0f);
	//! サイズ
	const Conv_XM::Vector2f k_BoostLetterSize = Conv_XM::Vector2f(640.0f, 128.0f) * 0.25f;

	//! ブーストゲージテクスチャファイル名
	static const char* k_BoostGaugeTextureFileName = "asset/texture/white.png";

	//! ブースト\テクスチャファイル名
	static const char* k_BoostTextureFileName = "asset/texture/boost.png";

	//! ポジション
	const Conv_XM::Vector2f k_BoostPosition = Conv_XM::Vector2f(510.0f, 525.0f);
	//! サイズ
	const Conv_XM::Vector2f k_BoostSize = Conv_XM::Vector2f(60.0f, 15.0f);
}

void BoostGaugeUI::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Load(BoostGaugeUIData::k_BoostGaugeTextureFileName);
	textureManager.Load(BoostGaugeUIData::k_BoostTextureFileName);
}

void BoostGaugeUI::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Unload(BoostGaugeUIData::k_BoostGaugeTextureFileName);
	textureManager.Unload(BoostGaugeUIData::k_BoostTextureFileName);
}

BoostGaugeUI::BoostGaugeUI()
{
}

BoostGaugeUI::~BoostGaugeUI()
{
}

void BoostGaugeUI::LoadActor()
{
	// スプライトコンポーネント作成
	m_GaugeSpriteComponent = CreateComponent<GaugeSpriteComponent>();

	// スプライトコンポーネント作成
	m_BoostSpriteComponent = CreateComponent<SpriteComponent>();
}

void BoostGaugeUI::InitActor()
{
	if (m_GaugeSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor m_GaugeSpriteComponentがnullptr");
		return;
	}

	// テクスチャ設定
	TextureManager& textureManager = TextureManager::GetInstance();
	ID3D11ShaderResourceView* gaugeSRV = textureManager.GetSRV(BoostGaugeUIData::k_BoostGaugeTextureFileName);
	if (gaugeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor gaugeSRVがnullptr");
		return;
	}
	m_GaugeSpriteComponent->SetSRV(gaugeSRV);

	// ゲージスプライトコンポーネント設定
	m_GaugeSpriteComponent->SetStartPosition(BoostGaugeUIData::k_GaugePosition);
	m_GaugeSpriteComponent->SetSize(BoostGaugeUIData::k_GaugeSize);
	m_GaugeSpriteComponent->SetMaxLength(BoostGaugeUIData::k_GaugeSize.x);
	m_GaugeSpriteComponent->SetDirection(GaugeSpriteComponent::RIGHT);

	m_BoostComponent = m_MobileSuit->GetComponent<BoostComponent>();

	if (m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor m_BoostComponentがnullptr");
		return;
	}

	// ブーストUI表示スプライト
	if (m_BoostSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor m_BoostSpriteComponentがnullptr");
		return;
	}

	// テクスチャ設定
	ID3D11ShaderResourceView* boostSRV = textureManager.GetSRV(BoostGaugeUIData::k_BoostTextureFileName);
	if (boostSRV == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor boostSRVがnullptr");
		return;
	}
	m_BoostSpriteComponent->SetSRV(boostSRV);

	// ゲージスプライトコンポーネント設定
	m_BoostSpriteComponent->SetPosition(BoostGaugeUIData::k_BoostPosition);
	m_BoostSpriteComponent->SetSize(BoostGaugeUIData::k_BoostSize);
}

void BoostGaugeUI::UpdateActor()
{
	if (m_BoostComponent == nullptr || m_GaugeSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::UpdateActor nullptr");
		return;
	}

	// ブースト割合取得
	float boostRatio = m_BoostComponent->GetBoostRatio();

	// ブースト割合設定
	m_GaugeSpriteComponent->SetRatio(boostRatio);

	// 色設定
	if (boostRatio < 0.25f)
	{
		m_GaugeSpriteComponent->SetColor(BoostGaugeUIData::k_QuarterBoostColor);
	}
	else if (boostRatio < 0.5f)
	{
		m_GaugeSpriteComponent->SetColor(BoostGaugeUIData::k_HalfBoostColor);
	}
	else
	{
		m_GaugeSpriteComponent->SetColor(BoostGaugeUIData::k_DefaultBoostColor);
	}
}
