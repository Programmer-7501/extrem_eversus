
#include"../system/main.h"
#include "mode_select_scene.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/actor/ui/mode_select_scene/mode_select_scene_ui.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../system/fade.h"
#include"game_scene.h"
#include"demo_scene.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../framework/actor/camera/debug_camera.h"
#include"../particle/particle_manager.h"
#include"../system/audio_manager.h"

/**
* @namespace ModeSelectSceneData
* @brief ���[�h�I���V�[�����
*/
namespace ModeSelectSceneData
{
	//! �t�F�[�h�C������
	static constexpr int k_FadeInTime = 60;

	//! �t�F�[�h�A�E�g����
	static constexpr int k_FadeOutTime = 60;
}

ModeSelectScene::ModeSelectScene()
	: m_IsSceneChange(false)
	, m_Fade(nullptr)
	, m_ModeSelectSceneUI(nullptr)
{
}

ModeSelectScene::~ModeSelectScene()
{
}

void ModeSelectScene::LoadData()
{
	// �I�[�f�B�I���擾
	AudioManager& audioManager = AudioManager::GetInstance();
	// �I�[�f�B�I�f�[�^�ǂݍ���
	audioManager.Load(ModeSelectSceneData::BGM);
	// �Đ�
	audioManager.Play(ModeSelectSceneData::BGM, true);

	// �p�[�e�B�N���}�l�[�W���[�擾
	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �f�o�b�O�J�E���^�[��\��
	particleManager.SetDebugCountView(false);

	// �V�[���}�l�[�W���[�擾
	SceneManager& sceneManager = SceneManager::GetInstance();

	// �t�F�[�h�擾
	m_Fade = sceneManager.GetFade();
	// �t�F�[�h�J�n
	m_Fade->SetFade(Fade::FADE_OUT, ModeSelectSceneData::k_FadeOutTime);

	// �A�N�^�[�}�l�[�W���[�쐬
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if(otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData otherActorManager��nullptr");
		return;
	}

	// UI�e�N�X�`���ǂݍ���
	ModeSelectSceneUI::LoadTexture();
	// UI�T�E���h�ǂݍ���
	ModeSelectSceneUI::LoadSound();
	// UI�쐬
	m_ModeSelectSceneUI = otherActorManager->CreateActor<ModeSelectSceneUI>();
	if (m_ModeSelectSceneUI == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData m_ModeSelectSceneUI��nullptr");
		return;
	}

	// �f�o�b�O�J�������쐬
	DebugCamera* debugCamera = otherActorManager->CreateActor<DebugCamera>();
	if (debugCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData debugCamera��nullptr");
		return;
	}
	debugCamera->SetPosition(Conv_XM::Vector3f(0.0f, 5.0f, -10.0f));

	// ���C�g�J�������쐬
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActor��nullptr");
		return;
	}
	// ���C�g�R���|�[�l���g�쐬
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActor��nullptr");
		return;
	}
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	lightCamera->SetTarget(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	lightCamera->SetUp(Conv_XM::Vector3f(0.0f, 0.0f, 1.0f));
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManager��nullptr");
		return;
	}
	// �V�[�������_�[�ɓo�^
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);
}

void ModeSelectScene::UnloadData()
{
	// �e�N�X�`���j��
	ModeSelectSceneUI::UnloadTexture();
	// �T�E���h�j��
	ModeSelectSceneUI::UnloadSound();
	// �I�[�f�B�I���擾
	AudioManager& audioManager = AudioManager::GetInstance();
	// �I�[�f�B�I�f�[�^�ǂݍ���
	audioManager.Unload(ModeSelectSceneData::BGM);

}

void ModeSelectScene::SetData()
{
}

void ModeSelectScene::UpdateScene()
{
	// �t�F�[�h�A�V�[���؂�ւ�
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fade��nullptr");
		return;
	}

	// �t�F�[�h���I������V�[����؂�ւ���
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		// �I�[�f�B�I���擾
		AudioManager& audioManager = AudioManager::GetInstance();
		// ��~
		audioManager.Stop(ModeSelectSceneData::BGM);

		switch (m_ModeSelectSceneUI->GetSelectSceneMode())
		{
		case ModeSelectSceneUI::SS_NONE:
			break;
		case ModeSelectSceneUI::SS_GAMESCENE:
			// �V�[����؂�ւ���
			SceneManager::GetInstance().SceneChange(SceneManager::SN_GAME_SCENE);
			break;
		case ModeSelectSceneUI::SS_SHADERDEMOSCENE:
			// �V�[����؂�ւ���
			SceneManager::GetInstance().SceneChange(SceneManager::SN_DEMO_SCENE);
			break;
		case ModeSelectSceneUI::SS_AIDEMOSCENE:
			// �V�[����؂�ւ���
			SceneManager::GetInstance().SceneChange(SceneManager::SN_AI_TEST_SCENE);
			break;
		case ModeSelectSceneUI::SS_GRAPHMAKESCENE:
			// �V�[����؂�ւ���
			SceneManager::GetInstance().SceneChange(SceneManager::SN_MAKE_NODE_SCENE);
			break;
		case ModeSelectSceneUI::SS_MAX:
			break;
		default:
			break;
		}
	}


	if (m_IsSceneChange == true)
	{
		return;
	}

	switch (m_ModeSelectSceneUI->GetSelectSceneMode())
	{
	case ModeSelectSceneUI::SS_NONE:
		break;
	case ModeSelectSceneUI::SS_GAMESCENE:
		// �V�[���؂�ւ��t���O�𗧂Ă�
		m_IsSceneChange = true;
		// �t�F�[�h���n�߂�
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_SHADERDEMOSCENE:
		// �V�[���؂�ւ��t���O�𗧂Ă�
		m_IsSceneChange = true;
		// �t�F�[�h���n�߂�
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_AIDEMOSCENE:
		// �V�[���؂�ւ��t���O�𗧂Ă�
		m_IsSceneChange = true;
		// �t�F�[�h���n�߂�
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_GRAPHMAKESCENE:
		// �V�[���؂�ւ��t���O�𗧂Ă�
		m_IsSceneChange = true;
		// �t�F�[�h���n�߂�
		m_Fade->SetFade(Fade::FADE_IN, ModeSelectSceneData::k_FadeInTime);
		break;
	case ModeSelectSceneUI::SS_MAX:
		break;
	default:
		break;
	}
}

void ModeSelectScene::CreateSceneFunction()
{
	// �V�[�������_�[�쐬
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void ModeSelectScene::UnloadSceneFunction()
{
	// �V�[�������_�[�}�l�[�W���[��j��
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
