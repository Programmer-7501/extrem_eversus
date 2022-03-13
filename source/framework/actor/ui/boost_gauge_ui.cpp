
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
	//! �u�[�X�g�������ȏ�̎��̐F
	const Conv_XM::Vector4f k_DefaultBoostColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

	//! �u�[�X�g�������ȉ��̎��̐F
	const Conv_XM::Vector4f k_HalfBoostColor = Conv_XM::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);

	//! �u�[�X�g��1/4�̎��̐F
	const Conv_XM::Vector4f k_QuarterBoostColor = Conv_XM::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

	//! �|�W�V����
	const Conv_XM::Vector2f k_GaugePosition = Conv_XM::Vector2f(510.0f, 500.0f);
	//! �T�C�Y
	const Conv_XM::Vector2f k_GaugeSize = Conv_XM::Vector2f(100.0f, 25.0f);

	//! �|�W�V����
	const Conv_XM::Vector2f k_BoostLetterPosition = Conv_XM::Vector2f(510.0f, 530.0f);
	//! �T�C�Y
	const Conv_XM::Vector2f k_BoostLetterSize = Conv_XM::Vector2f(640.0f, 128.0f) * 0.25f;

	//! �u�[�X�g�Q�[�W�e�N�X�`���t�@�C����
	static const char* k_BoostGaugeTextureFileName = "asset/texture/white.png";

	//! �u�[�X�g\�e�N�X�`���t�@�C����
	static const char* k_BoostTextureFileName = "asset/texture/boost.png";

	//! �|�W�V����
	const Conv_XM::Vector2f k_BoostPosition = Conv_XM::Vector2f(510.0f, 525.0f);
	//! �T�C�Y
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
	// �X�v���C�g�R���|�[�l���g�쐬
	m_GaugeSpriteComponent = CreateComponent<GaugeSpriteComponent>();

	// �X�v���C�g�R���|�[�l���g�쐬
	m_BoostSpriteComponent = CreateComponent<SpriteComponent>();
}

void BoostGaugeUI::InitActor()
{
	if (m_GaugeSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor m_GaugeSpriteComponent��nullptr");
		return;
	}

	// �e�N�X�`���ݒ�
	TextureManager& textureManager = TextureManager::GetInstance();
	ID3D11ShaderResourceView* gaugeSRV = textureManager.GetSRV(BoostGaugeUIData::k_BoostGaugeTextureFileName);
	if (gaugeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor gaugeSRV��nullptr");
		return;
	}
	m_GaugeSpriteComponent->SetSRV(gaugeSRV);

	// �Q�[�W�X�v���C�g�R���|�[�l���g�ݒ�
	m_GaugeSpriteComponent->SetStartPosition(BoostGaugeUIData::k_GaugePosition);
	m_GaugeSpriteComponent->SetSize(BoostGaugeUIData::k_GaugeSize);
	m_GaugeSpriteComponent->SetMaxLength(BoostGaugeUIData::k_GaugeSize.x);
	m_GaugeSpriteComponent->SetDirection(GaugeSpriteComponent::RIGHT);

	m_BoostComponent = m_MobileSuit->GetComponent<BoostComponent>();

	if (m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor m_BoostComponent��nullptr");
		return;
	}

	// �u�[�X�gUI�\���X�v���C�g
	if (m_BoostSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor m_BoostSpriteComponent��nullptr");
		return;
	}

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* boostSRV = textureManager.GetSRV(BoostGaugeUIData::k_BoostTextureFileName);
	if (boostSRV == nullptr)
	{
		Logger::GetInstance().SetLog("BoostGaugeUI::InitActor boostSRV��nullptr");
		return;
	}
	m_BoostSpriteComponent->SetSRV(boostSRV);

	// �Q�[�W�X�v���C�g�R���|�[�l���g�ݒ�
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

	// �u�[�X�g�����擾
	float boostRatio = m_BoostComponent->GetBoostRatio();

	// �u�[�X�g�����ݒ�
	m_GaugeSpriteComponent->SetRatio(boostRatio);

	// �F�ݒ�
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
