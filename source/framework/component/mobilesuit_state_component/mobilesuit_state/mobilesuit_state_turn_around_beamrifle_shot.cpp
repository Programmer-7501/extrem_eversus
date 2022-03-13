#include"../../../../system/main.h"
#include "mobilesuit_state_turn_around_beamrifle_shot.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../cameracomponent/lockon_camera_component.h"

namespace MobileSuitStateTurnAroundBeamRifleShotData
{
	//! ブーストダッシュ入力受付時間
	constexpr int k_BoostDashInputTime = 12;

	//! ブーストダッシュ初速
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! ブーストダッシュ時に使用される量
	constexpr float k_BoostDashUseBoostNum = 20.0f;

	//! 空中背面うちの場合の一フレームにおけるブースト使用量
	constexpr float k_TurnAroundAirUseBoostNum = 2.0f;
}

MobileSuitStateTurnAroundBeamRifleShot::MobileSuitStateTurnAroundBeamRifleShot(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_BoostDashInitialSpeed(MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInitialSpeed)
	, m_BoostDashCount(MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInputTime + 1)
	, m_FrameCount(0)
{
}

void MobileSuitStateTurnAroundBeamRifleShot::Init()
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

void MobileSuitStateTurnAroundBeamRifleShot::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateTurnAroundBeamRifleShot::ProcessInput nullptr");
		return;
	}


	bool isBeamRifleShot = m_MobileSuit->GetIsBeamRifleShot();
	// ビームライフルを撃ち終えたら
	if (isBeamRifleShot == false)
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
		// ブーストダッシュ入力待機状態で再びブーストダッシュ入力がされたら
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount <= MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInputTime)
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

			RotationActor(forward, myForward, m_Owner->GetInputDirection());
			Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
			m_RigidBodyComponent->PlusVelocity(dir * m_BoostDashInitialSpeed);

			m_MobileSuit->SetIsBoostDash(true);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
			m_BoostComponent->UseBoost(MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashUseBoostNum);
		}

		// ブーストダッシュ入力がない状態でジャンプボタンが押されたら
		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount > MobileSuitStateTurnAroundBeamRifleShotData::k_BoostDashInputTime)
		{
			// カウントをゼロにする
			m_BoostDashCount = 0;
		}
	}

	// 空中にいる場合ブーストを使用する
	if (m_RigidBodyComponent->GetIsGround() == false)
	{
		m_BoostComponent->UseBoost(MobileSuitStateTurnAroundBeamRifleShotData::k_TurnAroundAirUseBoostNum);
	}
}

void MobileSuitStateTurnAroundBeamRifleShot::Update()
{
	// カウントアップ
	m_FrameCount++;
	m_BoostDashCount++;
}

void MobileSuitStateTurnAroundBeamRifleShot::OnEnter()
{
	if (m_RigidBodyComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateTurnAroundBeamRifleShot::OnEnter m_RigidBodyComponentがnullptr");
		return;
	}

	// 速度を0にする
	m_RigidBodyComponent->SetVelocity(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	m_RigidBodyComponent->SetIsUseGravity(false);

	// 敵の方向に向ける
	RotateToEnemy();

	// 動かさない
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("beamrifle", 20);
}

void MobileSuitStateTurnAroundBeamRifleShot::OnExit()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateTurnAroundBeamRifleShot::OnEnter m_RigidBodyComponentがnullptr");
		return;
	}
	m_RigidBodyComponent->SetIsUseGravity(true);
}

void MobileSuitStateTurnAroundBeamRifleShot::RotateToEnemy()
{
	if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}

	Conv_XM::Vector3f enemyPosition, dif;
	if (m_MobileSuit->GetTargetNumber() == 0)
	{
		enemyPosition = m_EnemyMobileSuit001->GetPosition();
	}
	else
	{
		enemyPosition = m_EnemyMobileSuit002->GetPosition();
	}
	enemyPosition.y = 0.0f;

	Conv_XM::Vector3f myPosition = m_MobileSuit->GetPosition();
	myPosition.y = 0.0f;

	dif = enemyPosition - myPosition;
	dif = DirectX::XMVector3Normalize(dif);


	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, dif);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(Conv_XM::Vector3f::WorldForward, dif);
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
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, dif);

		//クォータニオンを求める
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	//求めたクォータニオンを合成する
	m_MobileSuit->SetQuaternion(NextVecQuaternion);
}
