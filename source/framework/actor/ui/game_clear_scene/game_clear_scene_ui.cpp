
#include"../../../../system/main.h"
#include "game_clear_scene_ui.h"
#include"../../../../directx/resource/texture_manager.h"
#include"../../../../system/input_manager.h"
#include"../../../component/sprite_component/sprite_component.h"


/**
* @namespace GameOverSceneUIData
* @brief �Q�[���I�[�o�[�V�[��UI���
*/
namespace GameClearSceneUIData
{
	//! �w�i�e�N�X�`���l�[��
	static const char* k_BackGroundTextureFileName = "asset/texture/game_clear_background.png";

	//! �w�i�e�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_BackGroundTexturePosition = Conv_XM::Vector2f(480.0f, 270.0f);

	//! �w�i�e�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_BackGroundTextureSize = Conv_XM::Vector2f(960.0f, 540.0f);

	//! YouLose�e�N�X�`���t�@�C���l�[��
	static const char* k_YouWinTextureFileName = "asset/texture/youwin.png";

	//! YouLose�e�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_YouWinTexturePosition = Conv_XM::Vector2f(480.0f, 160.0f);

	//! YouLose�e�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_YouWinTextureSize = Conv_XM::Vector2f(512.0f, 256.0f);

	//! PressEnter�e�N�X�`���t�@�C���l�[��
	static const char* k_PressEnterTextureFileName = "asset/texture/title004.png";

	//! PressEnter�e�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_PressEnterTexturePosition = Conv_XM::Vector2f(480.0f, 440.0f);

	//! PressEnter�e�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_PressEnterTextureSize = Conv_XM::Vector2f(512.0f, 128.0f);
}

void GameClearSceneUI::LoadTexture()
{
	// �C���X�^���X�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	// �w�i�e�N�X�`���j��
	textureManager.Load(GameClearSceneUIData::k_BackGroundTextureFileName);

	// YouLose�e�N�X�`���ǂݍ���
	textureManager.Load(GameClearSceneUIData::k_YouWinTextureFileName);

	// PressEnter�e�N�X�`���ǂݍ���
	textureManager.Load(GameClearSceneUIData::k_PressEnterTextureFileName);
}

void GameClearSceneUI::UnloadTexture()
{
	// �C���X�^���X�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	// �w�i�e�N�X�`���j��
	textureManager.Unload(GameClearSceneUIData::k_BackGroundTextureFileName);

	// YouLose�e�N�X�`���ǂݍ���
	textureManager.Unload(GameClearSceneUIData::k_YouWinTextureFileName);

	// PressEnter�e�N�X�`���ǂݍ���
	textureManager.Unload(GameClearSceneUIData::k_PressEnterTextureFileName);
}

GameClearSceneUI::GameClearSceneUI()
	: m_BackGroundSprite(nullptr)
	, m_YouWinSprite(nullptr)
	, m_PressEnterSprite(nullptr)
	, m_FrameCount(0)
{
}

GameClearSceneUI::~GameClearSceneUI()
{
}

void GameClearSceneUI::LoadActor()
{
	// �X�v���C�g�R���|�[�l���g�쐬
	m_BackGroundSprite = CreateComponent<SpriteComponent>();

	// youlose�X�v���C�g�R���|�[�l���g�쐬
	m_YouWinSprite = CreateComponent<SpriteComponent>();

	// PressEnter�̃X�v���C�g�R���|�[�l���g�쐬
	m_PressEnterSprite = CreateComponent<SpriteComponent>();
}

void GameClearSceneUI::InitActor()
{
	// �C���X�^���X�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* toBackGroundSRV = textureManager.GetSRV(GameClearSceneUIData::k_BackGroundTextureFileName);
	if (toBackGroundSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toGameSRV��nullptr");
		return;
	}
	if (m_BackGroundSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_BackGroundSprite��nullptr");
		return;
	}

	m_BackGroundSprite->SetSRV(toBackGroundSRV);
	// �g�����X�t�H�[������
	m_BackGroundSprite->SetPosition(GameClearSceneUIData::k_BackGroundTexturePosition);
	m_BackGroundSprite->SetSize(GameClearSceneUIData::k_BackGroundTextureSize);

	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* youLoseSRV = textureManager.GetSRV(GameClearSceneUIData::k_YouWinTextureFileName);
	if (youLoseSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor youLoseSRV��nullptr");
		return;
	}
	if (m_YouWinSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_YouLoseSprite��nullptr");
		return;
	}

	m_YouWinSprite->SetSRV(youLoseSRV);
	// �g�����X�t�H�[������
	m_YouWinSprite->SetPosition(GameClearSceneUIData::k_YouWinTexturePosition);
	m_YouWinSprite->SetSize(GameClearSceneUIData::k_YouWinTextureSize);


	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* pressEnterSRV = textureManager.GetSRV(GameClearSceneUIData::k_PressEnterTextureFileName);
	if (pressEnterSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor pressEnterSRV��nullptr");
		return;
	}
	if (m_PressEnterSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_PressEnterSprite��nullptr");
		return;
	}

	m_PressEnterSprite->SetSRV(pressEnterSRV);
	// �g�����X�t�H�[������
	m_PressEnterSprite->SetPosition(GameClearSceneUIData::k_PressEnterTexturePosition);
	m_PressEnterSprite->SetSize(GameClearSceneUIData::k_PressEnterTextureSize);
}

void GameClearSceneUI::UpdateActor()
{
	if (m_PressEnterSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::UpdateActor m_PressEnterSprite��nullptr");
		return;
	}
	if (m_FrameCount % 100 < 80)
	{
		m_PressEnterSprite->SetVisible(true);
	}
	else
	{
		m_PressEnterSprite->SetVisible(false);
	}
	// �J�E���g�A�b�v
	m_FrameCount++;
}
