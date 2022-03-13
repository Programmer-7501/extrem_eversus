
#include"../../../system/main.h"
#include "arousal_gauge_ui.h"
#include"../../component/sprite_component/changeable_sprite_component.h"
#include"../../component/sprite_component/gauge_sprite_component.h"
#include"../../component/mobilesuit_component/arousal_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../../directx/resource/texture_manager.h"

namespace ArousalGaugeUIData
{
	//! ブーストが半分以上の時の色
	const Conv_XM::Vector4f k_DefaultBoostColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

	//! ブーストが半分以下の時の色
	const Conv_XM::Vector4f k_HalfBoostColor = Conv_XM::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);

	//! ブーストが1/4の時の色
	const Conv_XM::Vector4f k_QuarterBoostColor = Conv_XM::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

	//! ポジション
	const Conv_XM::Vector2f k_GaugePosition = Conv_XM::Vector2f(450.0f, 500.0f);
	//! サイズ
	const Conv_XM::Vector2f k_GaugeSize = Conv_XM::Vector2f(100.0f, 25.0f);

	//! ブーストゲージテクスチャファイル名
	static const char* k_ArousalGaugeTextureFileName = "asset/texture/white.png";
}

void ArousalGaugeUI::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Load(ArousalGaugeUIData::k_ArousalGaugeTextureFileName);
}

void ArousalGaugeUI::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Unload(ArousalGaugeUIData::k_ArousalGaugeTextureFileName);
}

ArousalGaugeUI::ArousalGaugeUI()
{
}

ArousalGaugeUI::~ArousalGaugeUI()
{
}

void ArousalGaugeUI::LoadActor()
{
	m_GaugeSpriteComponent = CreateComponent<GaugeSpriteComponent>();

}

void ArousalGaugeUI::InitActor()
{
	if (m_GaugeSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("ArousalGaugeUI::InitActor m_GaugeSpriteComponentがnullptr");
		return;
	}

	TextureManager& textureManager = TextureManager::GetInstance();
	ID3D11ShaderResourceView* srv = textureManager.GetSRV(ArousalGaugeUIData::k_ArousalGaugeTextureFileName);
	if (m_GaugeSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("ArousalGaugeUI::InitActor srvがnullptr");
		return;
	}
	m_GaugeSpriteComponent->SetSRV(srv);

	// ゲージスプライトコンポーネント設定
	m_GaugeSpriteComponent->SetStartPosition(ArousalGaugeUIData::k_GaugePosition);
	m_GaugeSpriteComponent->SetSize(ArousalGaugeUIData::k_GaugeSize);
	m_GaugeSpriteComponent->SetMaxLength(ArousalGaugeUIData::k_GaugeSize.x);
	m_GaugeSpriteComponent->SetDirection(GaugeSpriteComponent::LEFT);

	// 覚醒コンポーネント取得
	m_ArousalComponent = m_MobileSuit->GetComponent<ArousalComponent>();

	if (m_ArousalComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor m_BoostComponentがnullptr");
		return;
	}
}

void ArousalGaugeUI::UpdateActor()
{
	if (m_ArousalComponent == nullptr || m_GaugeSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::UpdateActor nullptr");
		return;
	}

	// 覚醒割合取得
	float arousalRatio = m_ArousalComponent->GetArousalRatio();

	// 割合設定
	m_GaugeSpriteComponent->SetRatio(arousalRatio);

	// 色設定
	if (arousalRatio < 0.25f)
	{
		m_GaugeSpriteComponent->SetColor(ArousalGaugeUIData::k_QuarterBoostColor);
	}
	else if (arousalRatio < 0.5f)
	{
		m_GaugeSpriteComponent->SetColor(ArousalGaugeUIData::k_HalfBoostColor);
	}
	else
	{
		m_GaugeSpriteComponent->SetColor(ArousalGaugeUIData::k_DefaultBoostColor);
	}
}
