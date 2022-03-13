
#include"../../../../system/main.h"
#include "mobilesuit_state_boost_jump.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"

namespace StateBoostJumpData
{
	//! デフォルトのジャンプ速度
	constexpr float k_BoostJumpSpeed = 0.08f;

	//! ブースト消費量
	constexpr float k_DefaultBoostJumpUseBoostNum = 3.5f;

	//! ブーストダッシュ入力受付時間
	constexpr int k_BoostDashInputTime = 16;

	//! ブーストダッシュ速度
	constexpr float k_BoostDashSpeed = 1.5f;

	//! ブーストダッシュ時のブースト使用料
	constexpr float k_DefaultBoostDashUseBoostNum = 20.0f;

	//! デフォルトの回転上限(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;

	//! デフォルトの上昇速度上限値
	constexpr float k_DefaultRiseSpeedLimit = 0.4f;
}

MobileSuitStateBoostJump::MobileSuitStateBoostJump(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_BoostJumpSpeed(StateBoostJumpData::k_BoostJumpSpeed)
	, m_CountFrame(0)
	, m_BoostDashSpeed(StateBoostJumpData::k_BoostDashSpeed)
	, m_BoostDashUseBoostNum(StateBoostJumpData::k_DefaultBoostDashUseBoostNum)
	, m_IsPressJumpButton(true)
{
}

void MobileSuitStateBoostJump::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateBoostJump::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_BoostComponent == nullptr || m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBoostJump::ProcessInput nullptr");
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

	// y軸で右に90度回転させたクォータニオンを作成
	Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
	Conv_XM::Vector3f right = DirectX::XMVector3Rotate(forward, q1);

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
		// ジャンプボタンが入力され続けたら上昇
		if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_JUMP))
		{
			// 速度を与える
			m_RigidBodyComponent->PlusVelocity(Conv_XM::Vector3f(0.0f, m_BoostJumpSpeed, 0.0f));
			if (m_RigidBodyComponent->GetVelocity().y > StateBoostJumpData::k_DefaultRiseSpeedLimit)
			{
				m_RigidBodyComponent->SetVelocityY(StateBoostJumpData::k_DefaultRiseSpeedLimit);
			}

			m_BoostComponent->UseBoost(StateBoostJumpData::k_DefaultBoostJumpUseBoostNum);

			// 向きを回転させる
			RotationActor(forward, myForward, m_Owner->GetInputDirection(), StateBoostJumpData::k_DefaultRotationLimit);
		}
		else
		{
			m_IsPressJumpButton = false;
		}
		if (m_CountFrame > StateBoostJumpData::k_BoostDashInputTime && m_IsPressJumpButton == false)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}

		// 時間以内にジャンプボタンが押されたら
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_CountFrame < StateBoostJumpData::k_BoostDashInputTime)
		{
			RotationActor(forward, myForward, m_Owner->GetInputDirection());
			Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
			m_RigidBodyComponent->PlusVelocity(dir * m_BoostDashSpeed);

			m_MobileSuit->SetIsBoostDash(true);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
			m_BoostComponent->UseBoost(m_BoostDashUseBoostNum);
		}

		// メイン入力があったら
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))
		{
			m_MobileSuit->BeamRifleShot();
		}
	}
}

void MobileSuitStateBoostJump::Update()
{
	// カウントアップ
	m_CountFrame++;
}

void MobileSuitStateBoostJump::OnEnter()
{
	// カウント初期化
	m_CountFrame = 0;

	if (m_RigidBodyComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBoostJump::OnEnter nullptr");
		return;
	}
	// 設置判定を消す
	m_RigidBodyComponent->PlusVelocity(Conv_XM::Vector3f(0.0f, 0.1f, 0.0f));
	m_RigidBodyComponent->SetIsGround(false);

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("idle", 5);

	// ボタンフラグリセット
	m_IsPressJumpButton = true;
}

void MobileSuitStateBoostJump::OnExit()
{

}
