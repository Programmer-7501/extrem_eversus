
#include"../../../../system/main.h"
#include "debug_number_billboard_component.h"
#include"../../../../directx/resource/texture_manager.h"


namespace DebugNumberBillBoardComponentData
{
	//! 数字テクスチャのカットサイズ
	static const Conv_XM::Vector2f TexCutSize = Conv_XM::Vector2f(0.1f, 1.0f);

	//! 数字テクスチャのファイルネーム
	static const char* NumberFileName = "asset/texture/number.png";
}

void DebugNumberBillBoardComponent::LoadNumberTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Load(DebugNumberBillBoardComponentData::NumberFileName);
}

void DebugNumberBillBoardComponent::UnloadNumberTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Unload(DebugNumberBillBoardComponentData::NumberFileName);
}

DebugNumberBillBoardComponent::DebugNumberBillBoardComponent(Actor * owner)
	:ChangeableBillBoardComponent(owner)
	, m_DrawNumber(0)
{
	m_BillBoardTextureConfig.TexCutSize = DebugNumberBillBoardComponentData::TexCutSize;
	m_BillBoardTextureConfig.TexCut = Conv_XM::Vector2f(0.0f, 0.0f);
	TextureManager& textureManager = TextureManager::GetInstance();
	ID3D11ShaderResourceView* srv = textureManager.GetSRV(DebugNumberBillBoardComponentData::NumberFileName);
	SetSRV(srv);
}

DebugNumberBillBoardComponent::~DebugNumberBillBoardComponent()
{
}

void DebugNumberBillBoardComponent::SetNumber(int num)
{
	m_DrawNumber = num;
	m_IsUpdate = true;

	m_BillBoardTextureConfig.TexCut = Conv_XM::Vector2f((num * DebugNumberBillBoardComponentData::TexCutSize.x), 1.0f);
}