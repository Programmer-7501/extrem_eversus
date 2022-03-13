

#include"../../system/main.h"
#include "scene.h"
#include"../scene_function/scene_render_manager.h"
#include"../scene_function/scene_collision_manager.h"
#include"../../particle/particle_manager.h"

Scene::Scene()
	: m_SceneRenderManager(nullptr)
	, m_SceneCollisionManager(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	// �V�[���ɗp������@�\����
	CreateSceneFunction();

	// �f�[�^�ǂݍ���
	LoadData();

	// �I�u�W�F�N�g�}�l�[�W���[�ǂݍ��ݏ���
	for (auto* const actorManager : m_ActorManager)
	{
		// null�`�F�b�N
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Init actorManager��nullptr");
			continue;
		}
		actorManager->LoadData();
	}

	// �f�[�^�̃Z�b�g
	SetData();

	// �I�u�W�F�N�g�}�l�[�W���[����������
	for (auto* const actorManager : m_ActorManager)
	{
		// null�`�F�b�N
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Init actorManager��nullptr");
			continue;
		}
		actorManager->Init();
	}

	ParticleManager::GetInstance().UpdateParticleEmitterArrayConstantBuffer();
}

void Scene::Uninit()
{
	// �I�u�W�F�N�g�}�l�[�W���[�I������
	for (auto* const actorManager : m_ActorManager)
	{
		// null�`�F�b�N
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Uninit actorManager��nullptr");
			continue;
		}
		actorManager->Uninit();
	}

	// �I�u�W�F�N�g�}�l�[�W���[�폜
	for (auto* actorManager : m_ActorManager)
	{
		// null�`�F�b�N
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::Uninit actorManager��nullptr");
			continue;
		}
		delete actorManager;
		actorManager = nullptr;
	}
	//���g����ɂ���
	m_ActorManager.clear();

	// �V�[���Ǝ��̔j��
	UnloadData();

	// �V�[���ɗp����ꂽ�@�\�̔j��
	UnloadSceneFunction();

	// �p�[�e�B�N���}�l�[�W���[���Z�b�g����
	ParticleManager::GetInstance().Reset();
}

void Scene::Update()
{
	// �I�u�W�F�N�g�}�l�[�W���[�X�V����
	for (auto* const actorManager : m_ActorManager)
	{
		// null�`�F�b�N
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::ProcessInput actorManager��nullptr");
			continue;
		}
		actorManager->ProcessInput();
	}

	// �I�u�W�F�N�g�}�l�[�W���[�X�V����
	for (auto* const actorManager : m_ActorManager)
	{
		// null�`�F�b�N
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::ProcessInput actorManager��nullptr");
			continue;
		}
		actorManager->Update();
	}

	// �R���W�����}�l�[�W���[�̍X�V
	if (m_SceneCollisionManager)
	{
		m_SceneCollisionManager->Update();
	}

	// �V�[���Ǝ��̍X�V
	UpdateScene();

	// �p�[�e�B�N���̍X�V
	ParticleManager::GetInstance().Update();
}

void Scene::Draw() const
{
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Draw();
	}
}

void Scene::LoadData()
{
}

void Scene::UnloadData()
{
}

void Scene::SetData()
{
}

void Scene::UpdateScene()
{
}

void Scene::CreateSceneFunction()
{
}

void Scene::UnloadSceneFunction()
{
}
