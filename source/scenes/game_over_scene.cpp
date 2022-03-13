
#include"../system/main.h"
#include "game_over_scene.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/actor/ui/game_over_scene/game_over_scene_ui.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../system/fade.h"
#include"game_scene.h"
#include"demo_scene.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../framework/actor/camera/debug_camera.h"
#include"../particle/particle_manager.h"
#include"../system/audio_manager.h"

/**
* @namespace GameOverSceneData
* @brief �Q�[���I�[�o�[�V�[�����
*/
namespace GameOverSceneData
{
	//! �t�F�[�h�C������
	static constexpr int k_FadeInTime = 60;

	//! �t�F�[�h�A�E�g����
	static constexpr int k_FadeOutTime = 60;

	//! BGM
	static const char* BGM = "asset/sound/BGM/GameOverBGM.wav";
}

GameOverScene::GameOverScene()
	: m_IsSceneChange(false)
	, m_Fade(nullptr)
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::LoadData()
{
	// �p�[�e�B�N���}�l�[�W���[�̃f�o�b�N�J�E���^�[���\���ɂ���
	ParticleManager::GetInstance().SetDebugCountView(false);

	// �V�[���}�l�[�W���[�擾
	SceneManager& sceneManager = SceneManager::GetInstance();

	// �t�F�[�h�擾
	m_Fade = sceneManager.GetFade();
	// �t�F�[�h�J�n
	m_Fade->SetFade(Fade::FADE_OUT, GameOverSceneData::k_FadeOutTime);

	// �I�[�f�B�I�}�l�[�W���[�̎擾
	AudioManager& audioManager = AudioManager::GetInstance();
	// BGM�̓ǂݍ���
	audioManager.Load(GameOverSceneData::BGM);
	// BGM�̍Đ�
	audioManager.Play(GameOverSceneData::BGM, true);

	// �A�N�^�[�}�l�[�W���[�쐬
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if (otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData otherActorManager��nullptr");
		return;
	}

	// �f�o�b�O�J�������쐬
	DebugCamera* debugCamera = otherActorManager->CreateActor<DebugCamera>();
	if (debugCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData debugCamera��nullptr");
		return;
	}
	debugCamera->SetPosition(Conv_XM::Vector3f(0.0f, 5.0f, -10.0f));

	// ���C�g�J�������쐬
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData lightCameraActor��nullptr");
		return;
	}
	// ���C�g�J�����R���|�[�l���g�쐬
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData lightCameraActor��nullptr");
		return;
	}
	// ���C�g�J�����̃p�����[�^�[�ݒ�
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	lightCamera->SetTarget(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	lightCamera->SetUp(Conv_XM::Vector3f(0.0f, 0.0f, 1.0f));
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameOverScene::LoadData m_SceneRenderManager��nullptr");
		return;
	}
	// ���C�g�J�������V�[�������_�[�ɐݒ�
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);

	// UI�쐬
	GameOverSceneUI::LoadTexture();
	otherActorManager->CreateActor<GameOverSceneUI>();
}

void GameOverScene::UnloadData()
{
	// �e�N�X�`���j��
	GameOverSceneUI::UnloadTexture();
}

void GameOverScene::SetData()
{
}

void GameOverScene::UpdateScene()
{
	// �C���v�b�g�}�l�[�W���[���擾
	InputManager& inputManager = InputManager::GetInstance();

	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fade��nullptr");
		return;
	}

	// ����{�^������������
	if (inputManager.GetIsTrigger(InputManager::IN_DECIDE) == true)
	{
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
	}

	// �t�F�[�h���I������V�[����؂�ւ���
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		SceneManager::GetInstance().SceneChange(SceneManager::SN_TITLE_SCENE);
		AudioManager& audioManager = AudioManager::GetInstance();
		audioManager.Stop(GameOverSceneData::BGM);
	}

}

void GameOverScene::CreateSceneFunction()
{
	// �V�[�������_�[�쐬
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void GameOverScene::UnloadSceneFunction()
{
	// �V�[�������_�[��j��
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
