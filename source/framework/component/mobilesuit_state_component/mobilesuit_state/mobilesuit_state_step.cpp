
#include"../../../../system/main.h"
#include "mobilesuit_state_step.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../cameracomponent/lockon_camera_component.h"

namespace MobileSuitStateStepData
{
	//! デフォルトのステップ速度
	constexpr float k_DefaultStepSpeed = 0.1f;

	//! デフォルトのステップフレーム
	constexpr int k_DefaultStepFrame = 20;

	//! デフォルトのブースト使用量
	constexpr float k_DefaultBoostDashUseBoostNum = 10.0f;
}

MobileSuitStateStep::MobileSuitStateStep(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_FrameCount(0)
{
}

void MobileSuitStateStep::Init()
{
	// データ取得取得
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateStep::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_BoostComponent == nullptr || m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}
	InputManager& inputManager = InputManager::GetInstance();

	// 規定時間を超えたら
	if (m_FrameCount > MobileSuitStateStepData::k_DefaultStepFrame)
	{
		if (m_RigidBodyComponent->GetIsGround() == false)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
	}

	// カメラの前ベクトルを取得
	Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
	forward.y = 0.0f;
	forward = DirectX::XMVector3Normalize(forward);

	// カメラの右ベクトルを取得
	Conv_XM::Vector3f right = m_LockOnCameraComponent->GetRight();
	right.y = 0.0f;
	right = DirectX::XMVector3Normalize(right);

	if (inputManager.GetIsPress(InputManager::IN_FRONT))
	{
		m_RigidBodyComponent->PlusVelocity(forward * MobileSuitStateStepData::k_DefaultStepSpeed);
	}
	else if (inputManager.GetIsPress(InputManager::IN_BACK))
	{
		m_RigidBodyComponent->PlusVelocity(forward * (-MobileSuitStateStepData::k_DefaultStepSpeed));
	}
	else if (inputManager.GetIsPress(InputManager::IN_RIGHT))
	{
		m_RigidBodyComponent->PlusVelocity(right * MobileSuitStateStepData::k_DefaultStepSpeed);
	}
	else if (inputManager.GetIsPress(InputManager::IN_LEFT))
	{
		m_RigidBodyComponent->PlusVelocity(right * (-MobileSuitStateStepData::k_DefaultStepSpeed));
	}
}

void MobileSuitStateStep::Update()
{
	// カウントアップ
	m_FrameCount++;
}

void MobileSuitStateStep::OnEnter()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateStep::OnEnter m_RigidBodyComponentがnullptr");
		return;
	}

	// 重力をオフにする
	m_RigidBodyComponent->SetIsUseGravity(false);

	// フレームカウント初期化
	m_FrameCount = 0;
}

void MobileSuitStateStep::OnExit()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateStep::OnExit m_RigidBodyComponentがnullptr");
		return;
	}

	// 重力をオンにする
	m_RigidBodyComponent->SetIsUseGravity(true);
}
