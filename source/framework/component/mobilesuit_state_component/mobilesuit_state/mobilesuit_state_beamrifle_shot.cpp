
#include"../../../../system/main.h"
#include "mobilesuit_state_beamrifle_shot.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"

namespace MobileSuitStateBeamRifleShotData
{
	//! ビームライフルを打つまでのカウント
	constexpr int k_DefaultBeamRifleShotCount = 10;

	//! ビームライフルを打つ動作の全体フレーム
	constexpr int k_DefaultBeamRifleShotOverallFrame = 40;

	//! ブーストダッシュ入力受付時間
	constexpr int k_BoostDashInputTime = 12;

	//! ブーストダッシュ初速
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! デフォルトの歩く速さ
	constexpr float k_DefaultWalkSpeed = 0.02f;

	//! デフォルトの回転上限(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;
}

MobileSuitStateBeamRifleShot::MobileSuitStateBeamRifleShot(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_WalkSpeed(MobileSuitStateBeamRifleShotData::k_DefaultWalkSpeed)
	, m_BoostDashInitialSpeed(MobileSuitStateBeamRifleShotData::k_BoostDashInitialSpeed)
	, m_BoostDashCount(MobileSuitStateBeamRifleShotData::k_BoostDashInputTime + 1)
	, m_FrameCount(0)
	, m_IsShotFlag(false)
	, m_EnemyMobileSuit001(nullptr)
	, m_EnemyMobileSuit002(nullptr)
	, m_TargetNum(0)
	, m_BeamVelocity(Conv_XM::Vector3f(0.0f,0.0f,0.0f))
{

}

void MobileSuitStateBeamRifleShot::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetDataが失敗");
		return;
	}

	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::Init m_MobileSuitがnullptr");
		return;
	}

	// エネミーを入力
	m_EnemyMobileSuit001 = m_MobileSuit->GetEnemyMobileSuit001();
	m_EnemyMobileSuit002 = m_MobileSuit->GetEnemyMobileSuit002();
}

void MobileSuitStateBeamRifleShot::ProcessInput()
{
	if (m_MobileSuit == nullptr || m_Owner == nullptr || m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::ProcessInput nullptr");
		return;
	}

	// カメラの前ベクトルを取得
	Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
	forward.y = 0.0f;
	forward = DirectX::XMVector3Normalize(forward);

	// カメラの右ベクトルを取得
	Conv_XM::Vector3f right = m_LockOnCameraComponent->GetRight();
	right.y = 0.0f;
	right = DirectX::XMVector3Normalize(right);

	// モビルスーツアクターの前ベクトルを取得
	Conv_XM::Vector3f myForward = m_MobileSuit->GetForward();

	// ビームライフルを打つカウントになったら
	if (m_FrameCount > MobileSuitStateBeamRifleShotData::k_DefaultBeamRifleShotCount && m_IsShotFlag == false)
	{
		// ビームを打つ
		if (m_TargetNum == 0)
		{
			m_BeamRifleBulletManager->UseBullet(m_EnemyMobileSuit001, m_MobileSuit->GetPosition(), m_BeamVelocity);
		}
		else
		{
			m_BeamRifleBulletManager->UseBullet(m_EnemyMobileSuit002, m_MobileSuit->GetPosition(), m_BeamVelocity);
		}

		// ショットを撃ったフラグを立てる
		m_IsShotFlag = true;
	}

	// ビームライフルの動作のフレーム数を超えたら
	if (m_FrameCount > MobileSuitStateBeamRifleShotData::k_DefaultBeamRifleShotOverallFrame)
	{
		m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
	}


	//! AIの場合
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		InputManager& inputManager = InputManager::GetInstance();

		// ブーストダッシュ入力待機状態で再びブーストダッシュ入力がされたら
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount <= MobileSuitStateBeamRifleShotData::k_BoostDashInputTime)
		{
			// ブーストダッシュにキャンセル
			m_RigidBodyComponent->PlusVelocity(m_BoostDashInitialSpeed * forward);
			m_MobileSuit->SetIsBoostDash(true);
			// ブーストダッシュ状態にする
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
		}

		// ブーストダッシュ入力がない状態でジャンプボタンが押されたら
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount > MobileSuitStateBeamRifleShotData::k_BoostDashInputTime)
		{
			// カウントをゼロにする
			m_BoostDashCount = 0;
		}

		if (inputManager.GetIsPress(InputManager::IN_FRONT))
		{
			RotationActor(forward, myForward, FORWARD, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(forward * m_WalkSpeed);
		}
		else if (inputManager.GetIsPress(InputManager::IN_BACK))
		{
			RotationActor(forward, myForward, BACK, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(forward * (-m_WalkSpeed));
		}
		else if (inputManager.GetIsPress(InputManager::IN_RIGHT))
		{
			RotationActor(forward, myForward, RIGHT, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(right * m_WalkSpeed);
		}
		else if (inputManager.GetIsPress(InputManager::IN_LEFT))
		{
			RotationActor(forward, myForward, LEFT, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(right * (-m_WalkSpeed));
		}
	}
}

void MobileSuitStateBeamRifleShot::Update()
{
	// カウントアップ
	m_FrameCount++;
	m_BoostDashCount++;
}

void MobileSuitStateBeamRifleShot::OnEnter()
{
	// カウント初期化
	m_FrameCount = 0;
	// ショットフラグ初期化
	m_IsShotFlag = false;
	// ブーストダッシュ入力時間初期化
	m_BoostDashCount = MobileSuitStateBeamRifleShotData::k_BoostDashInputTime + 1;

	if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnEnter nullptr");
		return;
	}
	// どちらを狙っているか
	m_TargetNum = m_MobileSuit->GetTargetNumber();

	if (m_TargetNum == 0)
	{
		// 差をとる
		m_BeamVelocity = m_EnemyMobileSuit001->GetPosition() - m_MobileSuit->GetPosition();
		// 正規化する
		m_BeamVelocity = DirectX::XMVector3Normalize(m_BeamVelocity);
	}
	else
	{
		// 差をとる
		m_BeamVelocity = m_EnemyMobileSuit002->GetPosition() - m_MobileSuit->GetPosition();
		// 正規化する
		m_BeamVelocity = DirectX::XMVector3Normalize(m_BeamVelocity);
	}


}

void MobileSuitStateBeamRifleShot::OnExit()
{
	// カウント初期化
	m_FrameCount = 0;
	// ショットフラグ初期化
	m_IsShotFlag = false;
	// ブーストダッシュ入力時間初期化
	m_BoostDashCount = MobileSuitStateBeamRifleShotData::k_BoostDashInputTime + 1;
}
