
#include"../../../../system/main.h"
#include "mobilesuit_state_boost_dash.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mesh_component/animation_model_component.h"

namespace StateBoostDashData
{
	//! デフォルトのブーストダッシュスピード
	constexpr float k_DefaultBoostDashSpeed = 0.06f;

	//! デフォルトのブーストダッシュ時間
	constexpr int k_DefaultBoostDashTime = 24;

	//! デフォルトの回転上限(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;

	//! デフォルトのブースト使用量
	constexpr float k_DefaultBoostDashUseBoostNum = 2.0f;
}

MobileSuitStateBoostDash::MobileSuitStateBoostDash(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_BoostDashSpeed(StateBoostDashData::k_DefaultBoostDashSpeed)
	, m_BoostTime(StateBoostDashData::k_DefaultBoostDashTime)
	, m_FrameCount(0)
	, m_IsPressJump(true)
	, m_IsPressMove(true)
{

}

void MobileSuitStateBoostDash::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateBoostDash::ProcessInput()
{
	// nullチェック
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_MobileSuit == nullptr || m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}

	// ブーストがないなら
	if (m_BoostComponent->GetIsOverHeat() == true)
	{
		m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
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
	myForward.y = 0.0f;
	myForward = DirectX::XMVector3Normalize(myForward);

	//! AIの場合
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_JUMP) == false)
		{
			// ジャンプボタンを押し続けなかった
			m_IsPressJump = false;
		}

		if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK) == false)
		{
			// 移動ボタンを押し続けなかった
			m_IsPressMove = false;
		}

		bool isContinueBoostDash = m_IsPressJump || m_IsPressMove;

		if (isContinueBoostDash == true)// ブーストダッシュを続ける
		{
			// 続けている間はカウントリセット
			m_FrameCount = 0;
			// リジッドボディに速度を与える
			m_RigidBodyComponent->PlusVelocity(myForward * StateBoostDashData::k_DefaultBoostDashSpeed);
			m_RigidBodyComponent->SetVelocityY(0.0f);

			// ブーストを使用する
			m_BoostComponent->UseBoost(StateBoostDashData::k_DefaultBoostDashUseBoostNum);

			// ブーストジャンプ状態に移行
			if (m_IsPressJump == false && m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_JUMP) == true)
			{
				m_Owner->ChangeMobileSuitState("MobileSuitStateBoostJump");
			}
		}
		else// ブーストダッシュをやめる
		{
			// ブーストジャンプ状態に移行
			if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP))
			{
				m_Owner->ChangeMobileSuitState("MobileSuitStateBoostJump");
			}
			// ブーストダッシュ時間を超えた場合
			if (m_FrameCount > StateBoostDashData::k_DefaultBoostDashTime)
			{
				m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
			}
			else //　ブーストダッシュ時間中は速度を加える
			{
				// リジッドボディに速度を与える
				m_RigidBodyComponent->PlusVelocity(myForward * StateBoostDashData::k_DefaultBoostDashSpeed);
				m_RigidBodyComponent->SetVelocityY(0.0f);

				// ブーストを使用する
				m_BoostComponent->UseBoost(StateBoostDashData::k_DefaultBoostDashUseBoostNum);
			}
		}

		RotationActor(forward, myForward, m_Owner->GetInputDirection(), StateBoostDashData::k_DefaultRotationLimit);

		// 特殊射撃入力
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) &&
			m_CannonBulletManager->GetCurrentBullet() > 0)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))// メイン入力があったら
		{
			m_MobileSuit->BeamRifleShot();
		}
	}
}

void MobileSuitStateBoostDash::Update()
{
	// フレームカウントアップ
	m_FrameCount++;
}

void MobileSuitStateBoostDash::OnEnter()
{
	if (m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter m_RigidBodyComponentがnullptr");
		return;
	}

	// ブーストダッシュフラグをtrueに
	m_MobileSuit->SetIsBoostDash(true);
	m_MobileSuit->ResetBeamRifleCoolTime();

	// 重力をオフにする
	m_RigidBodyComponent->SetIsUseGravity(false);

	// 接地状態を解除する
	m_RigidBodyComponent->SetIsGround(false);

	// フレームカウントを初期化
	m_FrameCount = 0;

	// ボタンフラグを初期化
	m_IsPressJump = true;
	m_IsPressMove = true;

	// 動かさない
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("boostdash", 20);
}

void MobileSuitStateBoostDash::OnExit()
{
	if (m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnExit m_RigidBodyComponentがnullptr");
		return;
	}

	// ブーストダッシュフラグをfalseに
	m_MobileSuit->SetIsBoostDash(false);

	// 重力をオンにする
	m_RigidBodyComponent->SetIsUseGravity(true);
}

