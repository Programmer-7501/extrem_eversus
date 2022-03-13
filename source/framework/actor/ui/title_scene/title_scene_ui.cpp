
#include"../../../../system/main.h"
#include "title_scene_ui.h"
#include"../../../component/sprite_component/changeable_sprite_component.h"
#include"../../../../directx/resource/texture_manager.h"

namespace TitleSceneUIData
{
	static const char* k_Title001TextureFileName = "asset/texture/title001.png";
	static const char* k_Title002TextureFileName = "asset/texture/title002.png";
	static const char* k_Title003TextureFileName = "asset/texture/title003.png";
	static const char* k_PressEnterStartTextureFileName = "asset/texture/title004.png";

}

ID3D11ShaderResourceView* TitleSceneUI::m_TitleSprite001SRV = nullptr;
ID3D11ShaderResourceView* TitleSceneUI::m_TitleSprite002SRV = nullptr;
ID3D11ShaderResourceView* TitleSceneUI::m_TitleSprite003SRV = nullptr;
ID3D11ShaderResourceView* TitleSceneUI::m_PressEnterButtonSRV = nullptr;

void TitleSceneUI::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	m_TitleSprite001SRV = textureManager.Load(TitleSceneUIData::k_Title001TextureFileName);
	m_TitleSprite002SRV = textureManager.Load(TitleSceneUIData::k_Title002TextureFileName);
	m_TitleSprite003SRV = textureManager.Load(TitleSceneUIData::k_Title003TextureFileName);
	m_PressEnterButtonSRV = textureManager.Load(TitleSceneUIData::k_PressEnterStartTextureFileName);
}

void TitleSceneUI::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Unload(TitleSceneUIData::k_Title001TextureFileName);
	textureManager.Unload(TitleSceneUIData::k_Title002TextureFileName);
	textureManager.Unload(TitleSceneUIData::k_Title003TextureFileName);
	textureManager.Unload(TitleSceneUIData::k_PressEnterStartTextureFileName);
}

TitleSceneUI::TitleSceneUI()
	:m_TitleSprite001(nullptr)
	, m_TitleSprite002(nullptr)
	, m_TitleSprite003(nullptr)
	, m_PressEnterButton(nullptr)
	, m_FrameCount(0)
{
}

void TitleSceneUI::LoadActor()
{
	// スプライトコンポーネント作成
	m_TitleSprite001 = CreateComponent<SpriteComponent>();
	m_TitleSprite002 = CreateComponent<SpriteComponent>();
	m_TitleSprite003 = CreateComponent<SpriteComponent>();
	m_PressEnterButton = CreateComponent<SpriteComponent>();
}

void TitleSceneUI::InitActor()
{
	if (m_TitleSprite001 == nullptr || m_TitleSprite002 == nullptr || m_TitleSprite003 == nullptr || m_PressEnterButton == nullptr)
	{
		Logger::GetInstance().SetLog("TitleSceneUI::InitActor nullptr");
		return;
	}
	m_TitleSprite001->SetPosition(Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, SystemData::k_ScreenHeight / 2.0f));
	m_TitleSprite001->SetSize(Conv_XM::Vector2f(600.0f, 300.0f));
	m_TitleSprite001->SetSRV(m_TitleSprite001SRV);
	m_TitleSprite002->SetPosition(Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, SystemData::k_ScreenHeight / 2.0f));
	m_TitleSprite002->SetSize(Conv_XM::Vector2f(600.0f, 300.0f));
	m_TitleSprite002->SetSRV(m_TitleSprite002SRV);
	m_TitleSprite003->SetPosition(Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, SystemData::k_ScreenHeight / 2.0f));
	m_TitleSprite003->SetSize(Conv_XM::Vector2f(600.0f, 300.0f));
	m_TitleSprite003->SetSRV(m_TitleSprite003SRV);

	m_PressEnterButton->SetPosition(Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, SystemData::k_ScreenHeight / 2.0f + 200.0f));
	m_PressEnterButton->SetSize(Conv_XM::Vector2f(512.0f, 128.0f));
	m_PressEnterButton->SetSRV(m_PressEnterButtonSRV);
	//m_PressEnterButton->SetPosition(Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, SystemData::k_ScreenHeight / 2.0f));
}

void TitleSceneUI::ProcessInputActor()
{
	if (m_TitleSprite001 == nullptr || m_TitleSprite002 == nullptr || m_TitleSprite003 == nullptr || m_PressEnterButton == nullptr)
	{
		Logger::GetInstance().SetLog("TitleSceneUI::ProcessInputActor nullptr");
		return;
	}
	if (m_FrameCount % 100 < 70)
	{
		m_PressEnterButton->SetVisible(true);
	}
	else
	{
		m_PressEnterButton->SetVisible(false);
	}

	m_FrameCount++;
}
