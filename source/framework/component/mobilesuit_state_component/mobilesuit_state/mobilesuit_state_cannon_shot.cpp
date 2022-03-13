
#include"../../../../system/main.h"
#include "mobilesuit_state_cannon_shot.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor/bullet/cannon.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../mesh_component/animation_model_component.h"


namespace MobileSuitStateCannonShotData
{
	//! キャノンを打つまでのカウント
	constexpr int k_DefaultCannonShotCount = 10;

	//! キャノンを終えるカウント
	constexpr int k_DefaultCannonShotFinishCount = 90;

	//! ビームライフルを打つ動作の全体フレーム
	constexpr int k_DefaultCannonShotOverallFrame = 120;

	//! ブーストダッシュ入力受付時間
	constexpr int k_BoostDashInputTime = 12;

	//! ブーストダッシュ初速
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! ブーストダッシュ時のブースト使用量
	constexpr float k_BoostDashUseBoost = 20.0f;

	//! 一フレームにおけるブースト使用量
	constexpr float k_CannonUseBoostNum = 2.0f;
}

MobileSuitStateCannonShot::MobileSuitStateCannonShot(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_BoostDashInitialSpeed(MobileSuitStateCannonShotData::k_BoostDashInitialSpeed)
	, m_BoostDashCount(MobileSuitStateCannonShotData::k_BoostDashInputTime + 1)
	, m_FrameCount(0)
	, m_IsShotFlag(false)
	, m_CannonVelocity(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f))
{
}

void MobileSuitStateCannonShot::Init()
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
}

void MobileSuitStateCannonShot::ProcessInput()
{
	if (m_MobileSuit == nullptr || m_Owner == nullptr || m_LockOnCameraComponent == nullptr || m_RigidBodyComponent == nullptr || m_CannonBulletManager == nullptr || m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::ProcessInput nullptr");
		return;
	}

	if (m_FrameCount > 40)
	{
		m_AnimationComponent->StopModel(true, 40);
	}


	// ビームライフルを打つカウントになったら
	if (m_FrameCount > MobileSuitStateCannonShotData::k_DefaultCannonShotCount && m_IsShotFlag == false)
	{
		// エネミーに向ける
		RotateToEnemy();

		// ビームを打つ
		if (m_TargetNum == 0)
		{
			m_CannonBullet = m_CannonBulletManager->UseBullet(m_EnemyMobileSuit001, m_MobileSuit->GetPosition(), m_CannonVelocity);
		}
		else
		{
			m_CannonBullet = m_CannonBulletManager->UseBullet(m_EnemyMobileSuit002, m_MobileSuit->GetPosition(), m_CannonVelocity);
		}

		// ショットを撃ったフラグを立てる
		m_IsShotFlag = true;
	}

	// キャノンの動作のフレーム数を超えたら
	if (m_FrameCount > MobileSuitStateCannonShotData::k_DefaultCannonShotOverallFrame)
	{
		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}
	}


	//! AIの場合
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		InputManager& inputManager = InputManager::GetInstance();

		// ブーストダッシュ入力待機状態で再びブーストダッシュ入力がされたら
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount <= MobileSuitStateCannonShotData::k_BoostDashInputTime)
		{
			// ブーストダッシュにキャンセル
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

			if (inputManager.GetIsPress(InputManager::IN_FRONT))
			{
				RotationActor(forward, myForward, FORWARD);
				m_RigidBodyComponent->PlusVelocity(forward * m_BoostDashInitialSpeed);
			}
			else if (inputManager.GetIsPress(InputManager::IN_BACK))
			{
				RotationActor(forward, myForward, BACK);
				m_RigidBodyComponent->PlusVelocity(forward * (-m_BoostDashInitialSpeed));
			}
			else if (inputManager.GetIsPress(InputManager::IN_RIGHT))
			{
				RotationActor(forward, myForward, RIGHT);
				m_RigidBodyComponent->PlusVelocity(right * m_BoostDashInitialSpeed);
			}
			else if (inputManager.GetIsPress(InputManager::IN_LEFT))
			{
				RotationActor(forward, myForward, LEFT);
				m_RigidBodyComponent->PlusVelocity(right * (-m_BoostDashInitialSpeed));
			}

			m_MobileSuit->SetIsBoostDash(true);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
			m_BoostComponent->UseBoost(MobileSuitStateCannonShotData::k_BoostDashUseBoost);
		}

		// ブーストダッシュ入力がない状態でジャンプボタンが押されたら
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount > MobileSuitStateCannonShotData::k_BoostDashInputTime)
		{
			// カウントをゼロにする
			m_BoostDashCount = 0;
		}
	}

	// 空中なら
	if (m_RigidBodyComponent->GetIsGround() == false)
	{
		// ブーストを使用する
		m_BoostComponent->UseBoost(MobileSuitStateCannonShotData::k_CannonUseBoostNum);
	}
}

void MobileSuitStateCannonShot::Update()
{
	// カウントアップ
	m_FrameCount++;
	m_BoostDashCount++;
}

void MobileSuitStateCannonShot::OnEnter()
{
	// カウント初期化
	m_FrameCount = 0;
	// ショットフラグ初期化
	m_IsShotFlag = false;
	// ブーストダッシュ入力時間初期化
	m_BoostDashCount = MobileSuitStateCannonShotData::k_BoostDashInputTime + 1;

	if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr || m_RigidBodyComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnEnter nullptr");
		return;
	}
	// どちらを狙っているか
	m_TargetNum = m_MobileSuit->GetTargetNumber();

	if (m_TargetNum == 0)
	{
		// 差をとる
		m_CannonVelocity = m_EnemyMobileSuit001->GetPosition() - m_MobileSuit->GetPosition();
		// 正規化する
		m_CannonVelocity = DirectX::XMVector3Normalize(m_CannonVelocity);
	}
	else
	{
		// 差をとる
		m_CannonVelocity = m_EnemyMobileSuit002->GetPosition() - m_MobileSuit->GetPosition();
		// 正規化する
		m_CannonVelocity = DirectX::XMVector3Normalize(m_CannonVelocity);
	}


	// 速度を0にする
	m_RigidBodyComponent->SetVelocity(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	m_RigidBodyComponent->SetIsUseGravity(false);

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("cannon", 10);
}

void MobileSuitStateCannonShot::OnExit()
{
	// カウント初期化
	m_FrameCount = 0;
	// ショットフラグ初期化
	m_IsShotFlag = false;
	// ブーストダッシュ入力時間初期化
	m_BoostDashCount = MobileSuitStateCannonShotData::k_BoostDashInputTime + 1;

	// 重力をオンにする
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}
	m_RigidBodyComponent->SetIsUseGravity(true);

	// 前に向ける
	RotateToForward();

	if (m_CannonBullet == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}
	// 非アクティブ状態にする
	m_CannonBullet->SetActive(false);

	m_AnimationComponent->StopModel(false);
}

void MobileSuitStateCannonShot::RotateToEnemy()
{
	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}

	//Conv_XM::Vector3f actorForward = m_MobileSuit->GetForward();
	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, m_CannonVelocity);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(Conv_XM::Vector3f::WorldForward, m_CannonVelocity);
	float angle = xmangle.x;
	
	Conv_XM::Vector4f NextVecQuaternion;

	// 同じ方角なら
	if (dot > 0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionIdentity();
	}
	else if (dot < -0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis((DirectX::XMVECTOR)Conv_XM::Vector3f::WorldUp, DirectX::XM_PI);
	}
	else
	{
		//外積を求め回転軸を求める
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, m_CannonVelocity);

		//クォータニオンを求める
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	//求めたクォータニオンを合成する
	Conv_XM::Vector4f myQuaternion = m_MobileSuit->GetQuaternion();
	Conv_XM::Vector4f q = DirectX::XMQuaternionMultiply(myQuaternion, NextVecQuaternion);
	m_MobileSuit->SetQuaternion(NextVecQuaternion);
}

void MobileSuitStateCannonShot::RotateToForward()
{
	if (m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}

	Conv_XM::Vector3f nextVec = m_LockOnCameraComponent->GetForward();
	nextVec.y = 0.0f;
	nextVec = DirectX::XMVector3Normalize(nextVec);

	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, nextVec);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(Conv_XM::Vector3f::WorldForward, nextVec);
	float angle = xmangle.x;

	Conv_XM::Vector4f NextVecQuaternion;

	// 同じ方角なら
	if (dot > 0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionIdentity();
	}
	else if (dot < -0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis((DirectX::XMVECTOR)Conv_XM::Vector3f::WorldUp, DirectX::XM_PI);
	}
	else
	{
		//外積を求め回転軸を求める
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, nextVec);

		//クォータニオンを求める
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	//求めたクォータニオンをセットする
	m_MobileSuit->SetQuaternion(NextVecQuaternion);
}
