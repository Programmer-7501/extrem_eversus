
#include"../../../../system/main.h"
#include "mobilesuit_state_nkakutou_second.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../../actor/weapon/beam_saber.h"

namespace MobileSuitStateNKakutouSecondData
{
	//! 攻撃全体フレーム
	static constexpr int k_AmountFrame = 60;

	//! デフォルトのブースト速度
	static constexpr float k_DefaultBoostSpeed = 0.05f;

	//! デフォルトのブースト使用量
	static constexpr float k_DefaultBoostUseNum = 0.1f;
}

MobileSuitStateNKakutouSecond::MobileSuitStateNKakutouSecond(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_CountFrame(0)
	, m_BoostSpeed(MobileSuitStateNKakutouSecondData::k_DefaultBoostSpeed)
	, m_UseBoostNum(MobileSuitStateNKakutouSecondData::k_DefaultBoostUseNum)
	, m_IsGoNextKakutou(false)
{
}

void MobileSuitStateNKakutouSecond::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateNKakutouSecond::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateNKakutouSecond::ProcessInput()
{
	if (m_Owner == nullptr || m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}

	// 敵の方向を向く
	RotateToEnemy();

	Conv_XM::Vector3f enemyPosition, dif;
	if (m_MobileSuit->GetTargetNumber() == 0)
	{
		enemyPosition = m_EnemyMobileSuit001->GetPosition();
	}
	else
	{
		enemyPosition = m_EnemyMobileSuit002->GetPosition();
	}

	Conv_XM::Vector3f myPosition = m_MobileSuit->GetPosition();

	dif = enemyPosition - myPosition;
	dif = DirectX::XMVector3Normalize(dif);

	m_RigidBodyComponent->SetDirection(dif * m_BoostSpeed);
	// ブーストを消費する
	m_BoostComponent->UseBoost(m_UseBoostNum);

	// 全体フレームを超えたら
		// 全体フレームを超えたら
	if (m_CountFrame >= MobileSuitStateNKakutouSecondData::k_AmountFrame)
	{
		if (m_IsGoNextKakutou == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateNKakutouThird");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}
	}


	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		InputManager& inputManager = InputManager::GetInstance();

		if (inputManager.GetIsTrigger(InputManager::IN_KAKUTOU) == true)
		{
			m_IsGoNextKakutou = true;
		}
	}
}

void MobileSuitStateNKakutouSecond::Update()
{
	// カウントアップ
	m_CountFrame++;
}

void MobileSuitStateNKakutouSecond::OnEnter()
{
	if (m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateNKakutouFirst::OnEnter nullptr");
		return;
	}
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("nkaku_02", 5);

	m_CountFrame = 0;

	m_RigidBodyComponent->SetDirection(Conv_XM::Vector3f(0.0f, 0.1f, 0.0f));
	m_RigidBodyComponent->SetIsGround(false);
	m_RigidBodyComponent->SetIsUseGravity(false);

	// 敵の方向に向ける
	RotateToEnemy();

	// フラグリセット
	m_IsGoNextKakutou = false;

	m_BeamSaber->StartNKakutou002();
}

void MobileSuitStateNKakutouSecond::OnExit()
{
	m_RigidBodyComponent->SetIsUseGravity(true);

	m_BeamSaber->Reset();
}

void MobileSuitStateNKakutouSecond::RotateToEnemy()
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
