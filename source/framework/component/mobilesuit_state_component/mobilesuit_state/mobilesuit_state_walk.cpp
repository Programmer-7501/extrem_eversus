
#include"../../../../system/main.h"
#include "mobilesuit_state_walk.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mesh_component/animation_model_component.h"


namespace StateWalkData
{
	//! デフォルトの歩く速さ
	constexpr float k_DefaultWalkSpeed = 0.02f;

	//! デフォルトジャンプ速度
	constexpr float k_DefaultJumpSpeed = 2.0f;

	//! デフォルトの回転上限(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;

	//! ステップ入力待機時間
	constexpr int k_StepInputTime = 16;
}

MobileSuitStateWalk::MobileSuitStateWalk(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_WalkSpeed(StateWalkData::k_DefaultWalkSpeed)
	, m_JumpSpeed(StateWalkData::k_DefaultJumpSpeed)
	, m_FrameCount(0)
{
}

void MobileSuitStateWalk::Init()
{
	// データ取得取得
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateWalk::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_LockOnCameraComponent == nullptr || m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}

	//! AIの場合
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		// カメラの前ベクトルを取得
		Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
		forward.y = 0.0f;
		forward = DirectX::XMVector3Normalize(forward);


		// y軸で右に90度回転させたクォータニオンを作成
		Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
		Conv_XM::Vector3f right = DirectX::XMVector3Rotate(forward, q1);

		// モビルスーツアクターの前ベクトルを取得
		Conv_XM::Vector3f myForward = m_MobileSuit->GetForward();


		// 特殊射撃入力
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) &&
			m_CannonBulletManager->GetCurrentBullet() > 0)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP))// ブーストジャンプ状態に移行
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateJumpStandby");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI))// 射撃入力
		{
			m_MobileSuit->BeamRifleShot();
		}
		else if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK))
		{
			RotationActor(forward, myForward, m_Owner->GetInputDirection(), StateWalkData::k_DefaultRotationLimit);
			// 前ベクトルと目標ベクトルのなす角を算出
			float cos = Conv_XM::Vector2f::Dot(Conv_XM::Vector2f::Forward, m_Owner->GetInputDirection());
			float rad;
			if (m_Owner->GetInputDirection().x > 0)
			{
				rad = acosf(cos);
			}
			else
			{
				rad = -acosf(cos);
			}

			q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, rad);

			//正規化する
			Conv_XM::Vector3f nextVec = DirectX::XMVector3Rotate(forward, q1);
			nextVec = DirectX::XMVector3Normalize(nextVec);

			Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
			m_RigidBodyComponent->PlusVelocity(nextVec * m_WalkSpeed);
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
	}
}

void MobileSuitStateWalk::Update()
{
	// カウントアップ
	m_FrameCount++;
}

void MobileSuitStateWalk::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter m_AnimationComponentがnullptr");
		return;
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("walk", 20);
	// フレームカウントを初期化
	m_FrameCount = 0;
}

void MobileSuitStateWalk::OnExit()
{
}