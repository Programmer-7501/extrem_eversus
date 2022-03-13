
#include"../../../system/main.h"
#include "other_ui.h"
#include"../../component/sprite_component/sprite_component.h"
#include"../../component/mobilesuit_component/life_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../component/sprite_component/changeable_sprite_component.h"
#include"../../../directx/resource/texture_manager.h"


ID3D11ShaderResourceView* OtherUI::m_BeamRifleSRV = nullptr;
ID3D11ShaderResourceView* OtherUI::m_CannonSRV = nullptr;

void OtherUI::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	m_BeamRifleSRV = textureManager.Load("asset/texture/beamrifle.png");
	m_CannonSRV = textureManager.Load("asset/texture/cannon.png");
}

void OtherUI::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Unload("asset/texture/beamrifle.png");
	textureManager.Unload("asset/texture/cannon.png");
}

OtherUI::OtherUI()
{
}

void OtherUI::LoadActor()
{
	// スプライトコンポーネント作成
	m_BeamRifle = CreateComponent<SpriteComponent>();
	m_Cannon = CreateComponent<SpriteComponent>();
}

void OtherUI::InitActor()
{
	m_BeamRifle->SetPosition(Conv_XM::Vector2f(700.0f, 300.0f));
	m_BeamRifle->SetSize(Conv_XM::Vector2f(128.0f, 64.0f));
	m_BeamRifle->SetSRV(m_BeamRifleSRV);
	m_Cannon->SetPosition(Conv_XM::Vector2f(700.0f, 400.0f));
	m_Cannon->SetSize(Conv_XM::Vector2f(128.0f, 64.0f));
	m_Cannon->SetSRV(m_CannonSRV);
}
