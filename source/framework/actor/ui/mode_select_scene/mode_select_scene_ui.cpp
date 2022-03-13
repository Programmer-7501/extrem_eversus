
#include"../../../../system/main.h"
#include "mode_select_scene_ui.h"
#include"../../../component/sprite_component/changeable_sprite_component.h"
#include"../../../../directx/resource/texture_manager.h"
#include"../../../../system/input_manager.h"
#include"../../../../system/audio_manager.h"
#include"../mode_select_scene/mode_select_scene_ui.h"

/**
* @namespace ModeSelectSceneUIData
* @brief ���[�h�I���V�[��UI���
*/
namespace ModeSelectSceneUIData
{
	//! �w�i�e�N�X�`���l�[��
	static const char* k_BackGroundTextureFileName = "asset/texture/game_select_scene_001.png";


	//! �Q�[���V�[���փe�N�X�`���l�[��
	static const char* k_ToGameSceneTextureFileName = "asset/texture/gamescene.png";
	
	//! �Q�[���V�[���փe�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_ToGameSceneTexturePosition = Conv_XM::Vector2f(480.0f, 50.0f);

	//! �Q�[���V�[���փe�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_ToGameSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);


	//! �f���V�[���փe�N�X�`���l�[��
	static const char* k_ToShaderDemoSceneTextureFileName = "asset/texture/shaderdemoscene.png";

	//! �f���V�[���փe�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_ToShaderDemoSceneTexturePosition = Conv_XM::Vector2f(480.0f, 200.0f);

	//! �f���V�[���փe�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_ToShaderDemoSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);


	//! AI�f���V�[���փe�N�X�`���l�[��
	static const char* k_ToAIDemoSceneTextureFileName = "asset/texture/aidemoscene.png";

	//! AI�f���V�[���փe�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_ToAIDemoSceneTexturePosition = Conv_XM::Vector2f(480.0f, 350.0f);

	//! AI�f���V�[���փe�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_ToAIDemoSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);


	//! �O���t�쐬�V�[���փe�N�X�`���l�[��
	static const char* k_ToGraphMakeSceneTextureFileName = "asset/texture/graphmakescene.png";

	//! AI�f���V�[���փe�N�X�`���|�W�V����
	static const Conv_XM::Vector2f k_ToGraphMakeSceneTexturePosition = Conv_XM::Vector2f(480.0f, 500.0f);

	//! AI�f���V�[���փe�N�X�`���T�C�Y
	static const Conv_XM::Vector2f k_GraphMakeSceneTextureSize = Conv_XM::Vector2f(640.0f, 128.0f);



	//! �I���e�N�X�`���J���[
	static const Conv_XM::Vector4f k_OnTextureColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

	//! �I�t�e�N�X�`���J���[
	static const Conv_XM::Vector4f k_OffTextureColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 0.3f);


	//! ����{�^��SE
	static const char* DecideSE = "asset/sound/SE/Decide002SE.wav";

	//! �J�[�\��SE
	static const char* CursorSE = "asset/sound/SE/CursorSE.wav";
}

void ModeSelectSceneUI::LoadTexture()
{
	// �C���X�^���X�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	// �w�i�e�N�X�`���j��
	textureManager.Load(ModeSelectSceneUIData::k_BackGroundTextureFileName);

	// �Q�[���V�[���փe�N�X�`���ǂݍ���
	textureManager.Load(ModeSelectSceneUIData::k_ToGameSceneTextureFileName);

	// �f���V�[���փe�N�X�`���ǂݍ���
	textureManager.Load(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureFileName);

	// AI�f���V�[���փe�N�X�`���ǂݍ���
	textureManager.Load(ModeSelectSceneUIData::k_ToAIDemoSceneTextureFileName);

	// �O���t�쐬�V�[���փe�N�X�`���ǂݍ���
	textureManager.Load(ModeSelectSceneUIData::k_ToGraphMakeSceneTextureFileName);
}

void ModeSelectSceneUI::UnloadTexture()
{
	// �C���X�^���X�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	// �w�i�e�N�X�`���j��
	textureManager.Unload(ModeSelectSceneUIData::k_BackGroundTextureFileName);

	// �Q�[���V�[���փe�N�X�`���j��
	textureManager.Unload(ModeSelectSceneUIData::k_ToGameSceneTextureFileName);

	// �f���V�[���փe�N�X�`���j��
	textureManager.Unload(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureFileName);

	// AI�f���V�[���փe�N�X�`���j��
	textureManager.Unload(ModeSelectSceneUIData::k_ToAIDemoSceneTextureFileName);

	// �O���t�쐬�V�[���փe�N�X�`���j��
	textureManager.Unload(ModeSelectSceneUIData::k_ToGraphMakeSceneTextureFileName);
}

void ModeSelectSceneUI::LoadSound()
{
	// �I�[�f�B�I�}�l�[�W���[�擾
	AudioManager& audioManager = AudioManager::GetInstance();

	audioManager.Load(ModeSelectSceneUIData::CursorSE);
	audioManager.Load(ModeSelectSceneUIData::DecideSE);
}

void ModeSelectSceneUI::UnloadSound()
{
	// �I�[�f�B�I�}�l�[�W���[�擾
	AudioManager& audioManager = AudioManager::GetInstance();

	audioManager.Unload(ModeSelectSceneUIData::CursorSE);
	audioManager.Unload(ModeSelectSceneUIData::DecideSE);
}

ModeSelectSceneUI::ModeSelectSceneUI()
	: m_ModeSelect(SS_NONE)
	, m_SendMode(SS_NONE)
	, m_ToGameSceneSprite(nullptr)
	, m_ToShaderDemoSceneSprite(nullptr)
	, m_ToAIDemoSceneSprite(nullptr)
	, m_ToMakeGraphSceneSprite(nullptr)
{
}

ModeSelectSceneUI::~ModeSelectSceneUI()
{
}

void ModeSelectSceneUI::LoadActor()
{
	// �X�v���C�g�R���|�[�l���g�쐬
	m_BackGroundSprite = CreateComponent<SpriteComponent>();

	// �ύX�\�X�v���C�g�R���|�[�l���g�쐬
	m_ToGameSceneSprite = CreateComponent<ChangeableSpriteComponent>();

	// �ύX�\�X�v���C�g�R���|�[�l���g�쐬
	m_ToShaderDemoSceneSprite = CreateComponent<ChangeableSpriteComponent>();

	m_ToAIDemoSceneSprite = CreateComponent<ChangeableSpriteComponent>();

	m_ToMakeGraphSceneSprite = CreateComponent<ChangeableSpriteComponent>();
}

void ModeSelectSceneUI::InitActor()
{
	// �C���X�^���X�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* toBackGroundSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_BackGroundTextureFileName);
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
	m_BackGroundSprite->SetPosition(Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, SystemData::k_ScreenHeight / 2.0f));
	m_BackGroundSprite->SetSize(Conv_XM::Vector2f(SystemData::k_ScreenWidth, SystemData::k_ScreenHeight));




	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* toGameSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToGameSceneTextureFileName);
	if (toGameSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toGameSRV��nullptr");
		return;
	}
	if (m_ToGameSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToGameSceneSprite��nullptr");
		return;
	}
	m_ToGameSceneSprite->SetSRV(toGameSRV);
	// �g�����X�t�H�[������
	m_ToGameSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToGameSceneTexturePosition);
	m_ToGameSceneSprite->SetSize(ModeSelectSceneUIData::k_ToGameSceneTextureSize);



	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* toDemoSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureFileName);
	if (toDemoSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toDemoSRV��nullptr");
		return;
	}
	if (m_ToShaderDemoSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToShaderDemoSceneSprite��nullptr");
		return;
	}
	m_ToShaderDemoSceneSprite->SetSRV(toDemoSRV);
	// �g�����X�t�H�[������
	m_ToShaderDemoSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToShaderDemoSceneTexturePosition);
	m_ToShaderDemoSceneSprite->SetSize(ModeSelectSceneUIData::k_ToShaderDemoSceneTextureSize);





	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* toAIDemoSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToAIDemoSceneTextureFileName);
	if (toAIDemoSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toAIDemoSRV��nullptr");
		return;
	}
	if (m_ToAIDemoSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToAIDemoSceneSprite��nullptr");
		return;
	}
	m_ToAIDemoSceneSprite->SetSRV(toAIDemoSRV);
	// �g�����X�t�H�[������
	m_ToAIDemoSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToAIDemoSceneTexturePosition);
	m_ToAIDemoSceneSprite->SetSize(ModeSelectSceneUIData::k_ToAIDemoSceneTextureSize);


	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* toGraphMakeSRV = textureManager.GetSRV(ModeSelectSceneUIData::k_ToGraphMakeSceneTextureFileName);
	if (toGraphMakeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor toGraphMakeSRV��nullptr");
		return;
	}
	if (m_ToMakeGraphSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::LoadActor m_ToMakeGraphSceneSprite��nullptr");
		return;
	}
	m_ToMakeGraphSceneSprite->SetSRV(toGraphMakeSRV);
	// �g�����X�t�H�[������
	m_ToMakeGraphSceneSprite->SetPosition(ModeSelectSceneUIData::k_ToGraphMakeSceneTexturePosition);
	m_ToMakeGraphSceneSprite->SetSize(ModeSelectSceneUIData::k_GraphMakeSceneTextureSize);

	// ���[�h������
	m_ModeSelect = SS_GAMESCENE;

	// �����J���[
	m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
	m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
	m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
	m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
}

void ModeSelectSceneUI::ProcessInputActor()
{
	// �C���X�^���X�擾
	InputManager& inputManager = InputManager::GetInstance();

	if (m_ToGameSceneSprite == nullptr || m_ToShaderDemoSceneSprite == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectSceneUI::ProcessInputActor nullptr");
		return;
	}

	if (m_SendMode != SS_NONE)
	{
		return;
	}

	bool isDecide = false;
	bool isCursor = false;

	switch (m_ModeSelect)
	{
	case ModeSelectSceneUI::SS_NONE:
		break;
	case ModeSelectSceneUI::SS_GAMESCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_BACK))
		{
			m_ModeSelect = SS_SHADERDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_GAMESCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_SHADERDEMOSCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_FRONT))
		{
			m_ModeSelect = SS_GAMESCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_BACK))
		{
			m_ModeSelect = SS_AIDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_SHADERDEMOSCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_AIDEMOSCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_FRONT))
		{
			m_ModeSelect = SS_SHADERDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_BACK))
		{
			m_ModeSelect = SS_GRAPHMAKESCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_AIDEMOSCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_GRAPHMAKESCENE:
		if (inputManager.GetIsTrigger(InputManager::IN_FRONT))
		{
			m_ModeSelect = SS_AIDEMOSCENE;

			m_ToGameSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToShaderDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);
			m_ToAIDemoSceneSprite->SetColor(ModeSelectSceneUIData::k_OnTextureColor);
			m_ToMakeGraphSceneSprite->SetColor(ModeSelectSceneUIData::k_OffTextureColor);

			isCursor = true;
		}
		if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
		{
			m_SendMode = SS_GRAPHMAKESCENE;
			isDecide = true;
		}
		break;
	case ModeSelectSceneUI::SS_MAX:
		break;
	default:
		break;
	}

	// �I�[�f�B�I�}�l�[�W���[�擾
	AudioManager& audioManager = AudioManager::GetInstance();
	if (isDecide == true)
	{
		audioManager.Play(ModeSelectSceneUIData::DecideSE, false);
	}
	if (isCursor == true)
	{
		audioManager.Play(ModeSelectSceneUIData::CursorSE, false);
	}

}