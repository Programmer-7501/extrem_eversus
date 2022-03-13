
#include"../../../system/main.h"
#include "cost_manager.h"
#include"../../component/sprite_component/gauge_sprite_component.h"
#include"../../component/mobilesuit_component/boost_component.h"
#include"../../../directx/resource/texture_manager.h"

namespace CostManagerData
{
	//! ゲージのテクスチャネーム
	static const char* k_TextureFileName = "asset/texture/white.png";

	//! デフォルトのコスト
	static const int k_DefaultCost = 6000;

	//! 自分のゲージポジション
	const Conv_XM::Vector2f k_MyGaugePosition = Conv_XM::Vector2f(10.0f, 30.0f);

	//! 相手のゲージポジション
	const Conv_XM::Vector2f k_EnemyGaugePosition = Conv_XM::Vector2f(10.0f, 60.0f);

	//! ゲージサイズ
	const Conv_XM::Vector2f k_GaugeSize = Conv_XM::Vector2f(300.0f, 25.0f);

	//! TEAMのテクスチャネーム
	static const char* k_TeamTextureFileName = "asset/texture/team.png";

	//! TEAMのテクスチャポジション
	const Conv_XM::Vector2f k_TeamTexturePosition = Conv_XM::Vector2f(30.0f, 10.0f);

	//! TEAMのテクスチャサイズ
	const Conv_XM::Vector2f k_TeamTextureSize = Conv_XM::Vector2f(50.0f, 25.0f);


	//! ENEMYのテクスチャネーム
	static const char* k_EnemyTextureFileName = "asset/texture/enemy.png";

	//! ENEMYのテクスチャポジション
	const Conv_XM::Vector2f k_EnemyTexturePosition = Conv_XM::Vector2f(32.5f, 80.0f);

	//! ENEMYのテクスチャサイズ
	const Conv_XM::Vector2f k_EnemyTextureSize = Conv_XM::Vector2f(50.0f, 25.0f);


}

void CostManager::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Load(CostManagerData::k_TextureFileName);

	textureManager.Load(CostManagerData::k_TeamTextureFileName);

	textureManager.Load(CostManagerData::k_EnemyTextureFileName);
}

void CostManager::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Unload(CostManagerData::k_TextureFileName);

	textureManager.Unload(CostManagerData::k_TeamTextureFileName);

	textureManager.Unload(CostManagerData::k_EnemyTextureFileName);
}

CostManager::CostManager()
	:m_MyCost(CostManagerData::k_DefaultCost)
	, m_EnemyCost(CostManagerData::k_DefaultCost)
	, m_MyGaugeSpriteComp(nullptr)
	, m_EnemyGaugeSpriteComp(nullptr)
	, m_TeamSpriteComponent(nullptr)
	, m_EnemySpriteComponent(nullptr)
{
}

CostManager::~CostManager()
{
}

void CostManager::Reset()
{
	m_MyCost = CostManagerData::k_DefaultCost;
	m_EnemyCost = CostManagerData::k_DefaultCost;
}

void CostManager::LoadActor()
{
	// ゲージスプライトコンポーネント作成
	m_MyGaugeSpriteComp = CreateComponent<GaugeSpriteComponent>();

	m_EnemyGaugeSpriteComp = CreateComponent<GaugeSpriteComponent>();

	// 陣営表示スプライトコンポーネント作成
	m_TeamSpriteComponent = CreateComponent<SpriteComponent>();

	m_EnemySpriteComponent = CreateComponent<SpriteComponent>();
}

void CostManager::InitActor()
{
	if (m_MyGaugeSpriteComp == nullptr || m_EnemyGaugeSpriteComp == nullptr || m_TeamSpriteComponent == nullptr || m_EnemySpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor nullptr");
		return;
	}

	TextureManager& textureManager = TextureManager::GetInstance();

	// テクスチャ取得
	ID3D11ShaderResourceView* gaugeSRV = textureManager.GetSRV(CostManagerData::k_TextureFileName);
	if (gaugeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor gaugeSRVがnullptr");
		return;
	}
	// 自分のゲージ詳細設定
	m_MyGaugeSpriteComp->SetStartPosition(CostManagerData::k_MyGaugePosition);
	m_MyGaugeSpriteComp->SetSize(CostManagerData::k_GaugeSize);
	m_MyGaugeSpriteComp->SetMaxLength(CostManagerData::k_GaugeSize.x);
	m_MyGaugeSpriteComp->SetDirection(GaugeSpriteComponent::RIGHT);
	m_MyGaugeSpriteComp->SetSRV(gaugeSRV);
	m_MyGaugeSpriteComp->SetColor(Conv_XM::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));

	// エネミーのゲージ詳細設定
	m_EnemyGaugeSpriteComp->SetStartPosition(CostManagerData::k_EnemyGaugePosition);
	m_EnemyGaugeSpriteComp->SetSize(CostManagerData::k_GaugeSize);
	m_EnemyGaugeSpriteComp->SetMaxLength(CostManagerData::k_GaugeSize.x);
	m_EnemyGaugeSpriteComp->SetDirection(GaugeSpriteComponent::RIGHT);
	m_EnemyGaugeSpriteComp->SetSRV(gaugeSRV);
	m_EnemyGaugeSpriteComp->SetColor(Conv_XM::Vector4f(1.0f, 0.0f, 0.0f, 1.0f));


	// 陣営表示スプライトコンポーネント詳細設定

	m_TeamSpriteComponent->SetPosition(CostManagerData::k_TeamTexturePosition);
	m_TeamSpriteComponent->SetSize(CostManagerData::k_TeamTextureSize);

	// テクスチャ取得
	ID3D11ShaderResourceView* teamSRV = textureManager.GetSRV(CostManagerData::k_TeamTextureFileName);
	if (teamSRV == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor teamSRVがnullptr");
		return;
	}
	m_TeamSpriteComponent->SetSRV(teamSRV);


	m_EnemySpriteComponent->SetPosition(CostManagerData::k_EnemyTexturePosition);
	m_EnemySpriteComponent->SetSize(CostManagerData::k_EnemyTextureSize);

	// テクスチャ取得
	ID3D11ShaderResourceView* enemySRV = textureManager.GetSRV(CostManagerData::k_EnemyTextureFileName);
	if (enemySRV == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor enemySRVがnullptr");
		return;
	}
	m_EnemySpriteComponent->SetSRV(enemySRV);
}

void CostManager::UpdateActor()
{
	if (m_MyGaugeSpriteComp == nullptr || m_EnemyGaugeSpriteComp == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor nullptr");
		return;
	}
	
	float myRatio = static_cast<float>(m_MyCost) / static_cast<float>(CostManagerData::k_DefaultCost);
	m_MyGaugeSpriteComp->SetRatio(myRatio);

	float enemyRatio = static_cast<float>(m_EnemyCost) / static_cast<float>(CostManagerData::k_DefaultCost);
	m_EnemyGaugeSpriteComp->SetRatio(enemyRatio);

}
