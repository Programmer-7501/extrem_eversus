
#include"../../../../system/main.h"
#include "mobilesuit_state_bazooka.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../../actor_manager/bullet_manager.h"

namespace MobileSuitStateBazookaData
{
	//! デフォルトのステート時間
	static constexpr int k_DefaultTime = 60;

	//! ブーストダッシュ入力受付時間
	constexpr int k_BoostDashInputTime = 12;

	//! ブーストダッシュ初速
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! ブーストダッシュ時に使用される量
	constexpr float k_BoostDashUseBoostNum = 20.0f;

	//! 空中背面うちの場合の一フレームにおけるブースト使用量
	constexpr float k_TurnAroundAirUseBoostNum = 2.0f;

	//! バズーカを打つまでのカウント
	constexpr int k_DefaultBazookaShotCount = 10;
}

MobileSuitStateBazooka::MobileSuitStateBazooka(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
	, m_BoostDashCount(0)
	, m_BoostDashInitialSpeed(MobileSuitStateBazookaData::k_BoostDashInitialSpeed)
	, m_IsShotFlag(false)
	, m_BazookaSpeed(0)
{
}

void MobileSuitStateBazooka::Init()
{
	// データ取得取得
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateDown::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateBazooka::ProcessInput()
{

	//　硬直時間を過ぎたら
	if (m_CountFrame > MobileSuitStateBazookaData::k_DefaultTime)
	{
		if (m_Owner == nullptr || m_RigidBodyComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateBazooka::Update nullptr");
			return;
		}

		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}

	}

	// ビームライフルを打つカウントになったら
	if (m_CountFrame > MobileSuitStateBazookaData::k_DefaultBazookaShotCount && m_IsShotFlag == false)
	{
		// ビームを打つ
		if (m_TargetNum == 0)
		{
			m_BazookaBulletManager->UseBullet(m_EnemyMobileSuit001, m_MobileSuit->GetPosition(), m_MobileSuit->GetForward());
		}
		else
		{
			m_BazookaBulletManager->UseBullet(m_EnemyMobileSuit002, m_MobileSuit->GetPosition(), m_MobileSuit->GetForward());
		}

		// ショットを撃ったフラグを立てる
		m_IsShotFlag = true;
	}



	// ブーストダッシュ入力待機状態で再びブーストダッシュ入力がされたら
	if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount <= MobileSuitStateBazookaData::k_BoostDashInputTime)
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
		m_BoostComponent->UseBoost(MobileSuitStateBazookaData::k_BoostDashUseBoostNum);
	}

	// ブーストダッシュ入力がない状態でジャンプボタンが押されたら
	if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount > MobileSuitStateBazookaData::k_BoostDashInputTime)
	{
		// カウントをゼロにする
		m_BoostDashCount = 0;
	}


	// 空中にいる場合ブーストを使用する
	if (m_RigidBodyComponent->GetIsGround() == false)
	{
		m_BoostComponent->UseBoost(MobileSuitStateBazookaData::k_TurnAroundAirUseBoostNum);
	}
}

void MobileSuitStateBazooka::Update()
{
	// カウントアップ
	m_CountFrame++;
	m_BoostDashCount++;
}

void MobileSuitStateBazooka::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBazooka::OnEnter m_AnimationComponentがnullptr");
		return;
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("beamrifle", 20);

	// カウント初期化
	m_CountFrame = 0;
	m_BoostDashCount = MobileSuitStateBazookaData::k_BoostDashInputTime + 1;
	m_IsShotFlag = false;

	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBazooka::OnEnter nullptr");
		return;
	}

	m_RigidBodyComponent->SetDirection(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	m_RigidBodyComponent->SetIsUseGravity(false);


	// 敵の方向に向ける
	RotateToEnemy();
}

void MobileSuitStateBazooka::OnExit()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBazooka::OnEnter nullptr");
		return;
	}
	m_RigidBodyComponent->SetIsUseGravity(true);
}
