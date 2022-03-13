
#include"../../../system/main.h"
#include "debug_camera.h"
#include"../../component/cameracomponent/camera_component.h"
#include"../../../system/input_manager.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"

namespace DebugCameraData
{
	static constexpr float k_CameraSpeed = 0.1f;
	static constexpr float k_AngleSpeed = 0.05f;
	static constexpr float k_TargetDistance = 5.0f;
}

DebugCamera::DebugCamera()
{
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::InitActor()
{
	m_CameraComponent = CreateComponent<CameraComponent>();
	if (m_CameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("DebugCamera::InitActor m_MainCameraComponent��nullptr");
		return;
	}

	m_CameraComponent->SetPosition(m_Position);
	Conv_XM::Vector3f target = m_Position;
	target.z += 5.0f;
	m_CameraComponent->SetTarget(target);

	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("DebugCamera::InitActor currentScene��nullptr");
		return;
	}
	SceneRenderManager* sceneRenderManager = currentScene->GetSceneRenderManager();
	if (sceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("DebugCamera::InitActor sceneRenderManager��nullptr");
		return;
	}
	sceneRenderManager->SetMainCameraComponent(m_CameraComponent);
}

void DebugCamera::ProcessInputActor()
{
	// null�`�F�b�N
	if (m_CameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("DebugCamera::ProcessInputActor m_MainCameraComponent��nullptr");
		return;
	}

	// �C���v�b�g�}�l�[�W���[�擾
	InputManager& inputManager = InputManager::GetInstance();


	// ��]�v�Z

	Conv_XM::Vector4f plusQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
	Conv_XM::Vector4f resultQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

	if (inputManager.GetIsPress(InputManager::IN_TURN_RIGHT))
	{
		plusQuaternion = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DebugCameraData::k_AngleSpeed);
		resultQuaternion = DirectX::XMQuaternionMultiply(plusQuaternion, resultQuaternion);
	}
	if (inputManager.GetIsPress(InputManager::IN_TURN_LEFT))
	{
		plusQuaternion = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, -DebugCameraData::k_AngleSpeed);
		resultQuaternion = DirectX::XMQuaternionMultiply(plusQuaternion, resultQuaternion);
	}
	if (inputManager.GetIsPress(InputManager::IN_TURN_UP))
	{
		plusQuaternion = DirectX::XMQuaternionRotationAxis(GetRight(), -DebugCameraData::k_AngleSpeed);
		resultQuaternion = DirectX::XMQuaternionMultiply(plusQuaternion, resultQuaternion);
	}
	if (inputManager.GetIsPress(InputManager::IN_TURN_DOWN))
	{
		plusQuaternion = DirectX::XMQuaternionRotationAxis(GetRight(), DebugCameraData::k_AngleSpeed);
		resultQuaternion = DirectX::XMQuaternionMultiply(plusQuaternion, resultQuaternion);
	}

	// ��]����
	Conv_XM::Vector4f myQuaternion = GetQuaternion();
	myQuaternion = DirectX::XMQuaternionMultiply(myQuaternion, resultQuaternion);
	SetQuaternion(myQuaternion);


	// ���W�v�Z

	Conv_XM::Vector3f forward, right, up;
	forward = GetForward();
	right = GetRight();
	up = GetUp();

	if (inputManager.GetIsPress(InputManager::IN_FRONT))
	{
		m_Position += DebugCameraData::k_CameraSpeed * forward;
	}
	if (inputManager.GetIsPress(InputManager::IN_BACK))
	{
		m_Position -= DebugCameraData::k_CameraSpeed * forward;
	}
	if (inputManager.GetIsPress(InputManager::IN_RIGHT))
	{
		m_Position += DebugCameraData::k_CameraSpeed * right;
	}
	if (inputManager.GetIsPress(InputManager::IN_LEFT))
	{
		m_Position -= DebugCameraData::k_CameraSpeed * right;
	}
	if (inputManager.GetIsPress(InputManager::IN_UP))
	{
		m_Position += DebugCameraData::k_CameraSpeed * up;
	}
	if (inputManager.GetIsPress(InputManager::IN_DOWN))
	{
		m_Position -= DebugCameraData::k_CameraSpeed * up;
	}

	// �|�W�V�����A�^�[�Q�b�g�A��x�N�g���Z�b�g
	m_CameraComponent->SetPosition(m_Position);
	Conv_XM::Vector3f target = m_Position;
	target += DebugCameraData::k_TargetDistance * GetForward();
	m_CameraComponent->SetTarget(target);
	m_CameraComponent->SetUp(GetUp());
}
