
#include"../../../../system/main.h"
#include "radar.h"
#include"../../mobilesuit/mobilesuit.h"
#include"../../../../directx/resource/texture_manager.h"
#include"../../../component/sprite_component/sprite_component.h"
#include"../../../component/sprite_component/changeable_sprite_component.h"

namespace RadarData
{
	//! ���[�_�[�e�N�X�`���t�@�C���l�[��
	static const char* k_RadarBackGroundTextureName = "asset/texture/radar.png";

	//! ���[�_�[�w�i�e�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_RadarBackGroundTexturePosition = Conv_XM::Vector2f(400.0f,100.0f);

	//! ���[�_�[�w�i�e�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_RadarBackGroundTextureSize = Conv_XM::Vector2f(100.0f, 100.0f);

	//! ���[�_�[��e�N�X�`���t�@�C���l�[��
	static const char* k_RadarPieceTextureName = "asset/texture/radar_piece.png";

	//! ���[�_�[��e�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_RadarPieceTextureSize = Conv_XM::Vector2f(30.0f, 30.0f);
}

void Radar::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	// ���[�_�[�e�N�X�`���ǂݍ���
	textureManager.Load(RadarData::k_RadarBackGroundTextureName);

	// ���[�_�[��e�N�X�`���ǂݍ���
	textureManager.Load(RadarData::k_RadarPieceTextureName);
}

void Radar::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	// ���[�_�[�e�N�X�`���j��
	textureManager.Unload(RadarData::k_RadarBackGroundTextureName);

	// ���[�_�[��e�N�X�`���j��
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
	// �w�i�摜�X�v���C�g�R���|�[�l���g�쐬
	m_BackGroundSpriteComponent = CreateComponent<SpriteComponent>();

	//��X�v���C�g�R���|�[�l���g�쐬
	m_PlayerPieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
	m_AllienPieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
	m_Enemy001PieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
	m_Enemy002PieceSpriteComponent = CreateComponent<ChangeableSpriteComponent>();
}

void Radar::InitActor()
{
	// null�`�F�b�N
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

	//�@�e�N�X�`���擾
	ID3D11ShaderResourceView* radarBackGroundSRV = textureManager.GetSRV(RadarData::k_RadarBackGroundTextureName);
	if (radarBackGroundSRV == nullptr)
	{
		Logger::GetInstance().SetLog("Radar::InitActor radarBackGroundSRV��nullptr");
		return;
	}

	m_BackGroundSpriteComponent->SetSRV(radarBackGroundSRV);
	m_BackGroundSpriteComponent->SetPosition(RadarData::k_RadarBackGroundTexturePosition);
	m_BackGroundSpriteComponent->SetSize(RadarData::k_RadarBackGroundTextureSize);

	//�@�e�N�X�`���擾
	ID3D11ShaderResourceView* radarPieceSRV = textureManager.GetSRV(RadarData::k_RadarPieceTextureName);
	if (radarBackGroundSRV == nullptr)
	{
		Logger::GetInstance().SetLog("Radar::InitActor radarPieceSRV��nullptr");
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
