
#include"../../../system/main.h"
#include "directional_light.h"
#include"../../component/light_component/directional_light_component.h"
#include"../../component/cameracomponent/light_camera_component.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

DirectionalLight::DirectionalLight()
	: m_LightCameraComponent(nullptr)
	, m_DirectionalLightComponent(nullptr)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetChaseTarget(Actor * actor)
{
	if (actor == nullptr || m_LightCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("DirectionalLight::SetChaseTarget nullptr");
		return;
	}
	m_LightCameraComponent->SetChaseTarget(actor);
}

void DirectionalLight::SetEuler(float x, float y, float z)
{
	if (m_DirectionalLightComponent == nullptr || m_LightCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("DirectionalLight::SetChaseTarget nullptr");
		return;
	}

	m_DirectionalLightComponent->SetEuler(x, y, z);
	m_LightCameraComponent->SetEuler(x, y, z);
}

void DirectionalLight::SetLightParameter(const CBufferData::DirectionalLight & directionalLight)
{
	m_DirectionalLightComponent->SetLight(directionalLight);
}

void DirectionalLight::LoadActor()
{
	//! ライトカメラコンポーネント作成
	m_LightCameraComponent = CreateComponent<LightCameraComponent>();

	//! ディレクショナルライトコンポーネント
	m_DirectionalLightComponent = CreateComponent<DirectionalLightComponent>();
}

void DirectionalLight::InitActor()
{
	CBufferData::DirectionalLight directionalLight = {};
	directionalLight.Ambient = Conv_XM::Vector3f(0.3f, 0.3f, 0.3f);
	directionalLight.Color = Conv_XM::Vector3f(0.8f, 0.8f, 0.8f);
	m_DirectionalLightComponent->SetLight(directionalLight);

	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("DirectionalLight::InitActor currentSceneがnullptr");
		return;
	}
	SceneRenderManager* sceneRenderManager = currentScene->GetSceneRenderManager();
	if (sceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("DirectionalLight::InitActor sceneRenderManagerがnullptr");
		return;
	}
	sceneRenderManager->SetLightCameraComponent(m_LightCameraComponent);
}
