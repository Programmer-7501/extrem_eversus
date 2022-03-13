
#include"../../../system/main.h"
#include "number_sprite_component.h"
#include"../../../directx/resource/texture_manager.h"

namespace NumberSpriteComponentData
{
	//! 数字テクスチャのカットサイズ
	static const Conv_XM::Vector2f TexCutSize = Conv_XM::Vector2f(0.1f, 1.0f);

	//! 数字テクスチャのファイルネーム
	static const char* NumberFileName = "asset/texture/number.png";
}

//! 使用する数字テクスチャ
ID3D11ShaderResourceView* NumberSpriteComponent::m_NumberTexture = nullptr;

void NumberSpriteComponent::LoadNumberTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	m_NumberTexture = textureManager.Load(NumberSpriteComponentData::NumberFileName);
}

void NumberSpriteComponent::UnloadNumberTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Unload(NumberSpriteComponentData::NumberFileName);
}

NumberSpriteComponent::NumberSpriteComponent(Actor * owner)
	:ChangeableSpriteComponent(owner)
	, m_DrawNumber(0)
{
	m_SpriteTextureConfig.TexCutSize = NumberSpriteComponentData::TexCutSize;
	m_SpriteTextureConfig.TexCut = Conv_XM::Vector2f(0.0f, 0.0f);
	SetSRV(m_NumberTexture);
}

NumberSpriteComponent::~NumberSpriteComponent()
{
}

void NumberSpriteComponent::SetNumber(int num)
{
	m_DrawNumber = num;
	m_IsUpdate = true;

	m_SpriteTextureConfig.TexCut = Conv_XM::Vector2f((num * NumberSpriteComponentData::TexCutSize.x), 1.0f);
}