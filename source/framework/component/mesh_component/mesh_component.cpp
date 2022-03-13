
#include"../../../system/main.h"
#include "mesh_component.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

MeshComponent::MeshComponent(Actor * owner)
	: Component(owner)
	, m_Visible(true)
	, m_RenderingWay(RENDERING_DEFERRED)
	, m_IsShadowInput(false)
	, m_Material{ {0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f},0.5f,0.5f,{0.0f,0.0f} }
{

}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Init()
{
	// �f�[�^�ǂݍ���
	if (LoadData() == false)
	{
		Logger::GetInstance().SetLog("MeshComponent::Init Init�����s");
		return;
	}

	// �V�[�������_�[�}�l�[�W���[�ɓo�^
	if (RegisterSceneRenderManager() == false)
	{
		Logger::GetInstance().SetLog("MeshComponent::Init RegisterSceneRenderManager�����s");
		return;
	}
}

void MeshComponent::ShadowDraw() const
{
}

bool MeshComponent::RegisterSceneRenderManager()
{
	// �V�[�������_�[�}�l�[�W���[�ɓo�^
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init currentScene��nullptr");
		return false;
	}
	SceneRenderManager* sceneRenderManager = currentScene->GetSceneRenderManager();
	if (sceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init sceneRenderManager��nullptr");
		return false;
	}
	sceneRenderManager->AddMeshComponent(this);

	return true;
}

bool MeshComponent::LoadData()
{
	return false;
}
