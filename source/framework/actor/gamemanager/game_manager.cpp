
#include"../../../system/main.h"
#include "game_manager.h"
#include"cost_manager.h"
#include"../mobilesuit/mobilesuit.h"
#include"../ui/timelimit_ui.h"
#include"../../component/sprite_component/number_sprite_component.h"
#include"../../../directx/resource/texture_manager.h"

namespace GameManagerData
{
	//! �f�t�H���g�̃��X�|�[������
	static constexpr int k_RespawnTime = 120;

	//! �����X�v���C�g�̃|�W�V����
	static Conv_XM::Vector2f k_NumberSpritePosition = Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, 400.0f);

	//! �����X�v���C�g�̃T�C�Y
	static Conv_XM::Vector2f k_NumberSpriteSize = Conv_XM::Vector2f(100.0f, 100.0f);

	//! Standby�e�N�X�`���t�@�C���l�[��
	static const char* k_StandbyTextureFileName = "asset/texture/standby.png";

	//! �����X�v���C�g�̃|�W�V����
	static Conv_XM::Vector2f k_StandbyTexturePosition = Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, 200.0f);

	//! �����X�v���C�g�̃T�C�Y
	static Conv_XM::Vector2f k_StandbyTextureSize = Conv_XM::Vector2f(400.0f, 200.0f);

	//! Go�e�N�X�`���t�@�C���l�[��
	static const char* k_GoTextureFileName = "asset/texture/go.png";

	//! �����X�v���C�g�̃|�W�V����
	static Conv_XM::Vector2f k_GoTexturePosition = Conv_XM::Vector2f(SystemData::k_ScreenWidth / 2.0f, 200.0f);

	//! �����X�v���C�g�̃T�C�Y
	static Conv_XM::Vector2f k_GoTextureSize = Conv_XM::Vector2f(400.0f, 200.0f);
}

void GameManager::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Load(GameManagerData::k_StandbyTextureFileName);
	textureManager.Load(GameManagerData::k_GoTextureFileName);
}

void GameManager::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Unload(GameManagerData::k_StandbyTextureFileName);
	textureManager.Unload(GameManagerData::k_GoTextureFileName);
}

GameManager::GameManager()
	:m_CostManager(nullptr)
	, m_MobileSuit{}
	, m_RespawnTime{}
	, m_CountFrame(0)
	, m_CountDownSpriteComponent(nullptr)
	, m_SceneChange(SCENE_CHANGE_NONE)
{
	for (int i = 0; i < MAXMOBILESUIT; i++)
	{
		m_MobileSuit[i] = nullptr;
		m_RespawnTime[i] = 0;
	}
}

GameManager::~GameManager()
{
}

void GameManager::LoadActor()
{
	// �����X�v���C�g�R���|�[�l���g�쐬
	m_CountDownSpriteComponent = CreateComponent<NumberSpriteComponent>();

	// Standby�X�v���C�g�R���|�[�l���g�쐬
	m_StandbySpriteComponent = CreateComponent<SpriteComponent>();

	// Go�X�v���C�g�R���|�[�l���g�쐬
	m_GoSpriteComponent = CreateComponent<SpriteComponent>();
}

void GameManager::InitActor()
{
	// null�`�F�b�N
	if (m_CountDownSpriteComponent == nullptr || m_StandbySpriteComponent == nullptr || m_GoSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameManager::InitActor nullptr");
		return;
	}

	// �J�E���g�_�E���X�v���C�g�ݒ�
	m_CountDownSpriteComponent->SetPosition(GameManagerData::k_NumberSpritePosition);
	m_CountDownSpriteComponent->SetSize(GameManagerData::k_NumberSpriteSize);
	m_CountDownSpriteComponent->SetNumber(3);

	// �C���X�^���X�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	// �X�^���o�C�X�v���C�g�ݒ�
	ID3D11ShaderResourceView* standbySRV = textureManager.GetSRV(GameManagerData::k_StandbyTextureFileName);
	if (standbySRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameManager::InitActor standbySRV��nullptr");
		return;
	}
	m_StandbySpriteComponent->SetSRV(standbySRV);
	m_StandbySpriteComponent->SetPosition(GameManagerData::k_StandbyTexturePosition);
	m_StandbySpriteComponent->SetSize(GameManagerData::k_StandbyTextureSize);

	// �S�[�X�v���C�g�ݒ�
	ID3D11ShaderResourceView* goSRV = textureManager.GetSRV(GameManagerData::k_GoTextureFileName);
	if (goSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameManager::InitActor goSRV��nullptr");
		return;
	}
	m_GoSpriteComponent->SetSRV(goSRV);
	m_GoSpriteComponent->SetPosition(GameManagerData::k_GoTexturePosition);
	m_GoSpriteComponent->SetSize(GameManagerData::k_GoTextureSize);
	m_GoSpriteComponent->SetVisible(false);
}

void GameManager::UpdateActor()
{
	// �^�C�����~�b�g�ɂȂ����畉��
	if (m_TimeLimitUI->GetTimeLeft() <= 0)
	{
		m_SceneChange = SCENE_CHANGE_LOSE;
	}
	// ���X�|�[��
	RespawnManager();

	// �X�^�[�g�J�n��
	GameStartManager();

	// �J�E���g�A�b�v
	m_CountFrame++;
}

void GameManager::GameStartManager()
{
	if (m_CountDownSpriteComponent == nullptr || m_StandbySpriteComponent == nullptr || m_GoSpriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateComponent::GameStartManager nullptr");
		return;
	}

	if (m_CountFrame < 60)
	{
		m_CountDownSpriteComponent->SetNumber(3);
	}
	else if (m_CountFrame < 60 * 2)
	{
		m_CountDownSpriteComponent->SetNumber(2);
	}
	else if (m_CountFrame < 60 * 3)
	{
		m_CountDownSpriteComponent->SetNumber(1);
	}
	else if (m_CountFrame < 60 * 4)
	{
		m_StandbySpriteComponent->SetVisible(false);
		m_GoSpriteComponent->SetVisible(true);
		m_CountDownSpriteComponent->SetNumber(0);
	}
	else
	{
		// �X�v���C�g��\��
		m_GoSpriteComponent->SetVisible(false);
		m_CountDownSpriteComponent->SetVisible(false);

		// ���r���X�[�c�𓮂���
		for (int i = 0; i < MAXMOBILESUIT; i++)
		{
			if (m_MobileSuit[i] == nullptr)
			{
				Logger::GetInstance().SetLog("MobileSuitStateComponent::GameStartManager m_MobileSuit��nullptr");
				return;
			}
			m_MobileSuit[i]->SetIsMoveFlag(true);
		}

		//�@�^�C���v���J�n
		m_TimeLimitUI->SetMesureStartFlag(true);
	}

}

void GameManager::RespawnManager()
{
	if (m_CostManager == nullptr || m_SceneChange != SCENE_CHANGE_NONE)
	{
		return;
	}


	// �G�̃R�X�g���[���ɂȂ�����
	if (m_CostManager->GetEnemyCost() <= 0)
	{
		m_SceneChange = SCENE_CHANGE_WIN;
	}
	// �����̃R�X�g���[���ɂȂ�����
	if (m_CostManager->GetMyCost() <= 0)
	{
		m_SceneChange = SCENE_CHANGE_LOSE;
	}


	for (int i = 0; i < MAXMOBILESUIT; i++)
	{
		if (m_MobileSuit[i] == nullptr)
		{
			continue;
		}

		// ���r���X�[�c����A�N�e�B�u�Ȃ�
		if (m_MobileSuit[i]->GetActive() == false)
		{
			// ���X�|�[�����Ԃ𑝂₷
			m_RespawnTime[i]++;

			// ���X�|�[�����Ԃ���������
			if (m_RespawnTime[i] > GameManagerData::k_RespawnTime)
			{
				m_MobileSuit[i]->SetActive(true);
				m_MobileSuit[i]->Reset();
				m_MobileSuit[i]->SetPosition(Conv_XM::Vector3f(rand() % 40, 20.0f, rand() % 40));
				m_RespawnTime[i] = 0;
			}
		}

	}
}
