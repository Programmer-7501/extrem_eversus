
#include"../../../../system/main.h"
#include "mobilesuit_state_nkakutou_third.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../../actor/weapon/beam_saber.h"

namespace MobileSuitStateNKakutouThirdData
{
	//! 攻撃全体フレーム
	static constexpr int k_AmountFrame = 60;

	//! デフォルトのブースト速度
	static constexpr float k_DefaultBoostSpeed = 0.05f;

	//! デフォルトのブースト使用量
	static constexpr float k_DefaultBoostUseNum = 0.1f;
}

MobileSuitStateNKakutouThird::MobileSuitStateNKakutouThird(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_CountFrame(0)
	, m_BoostSpeed(MobileSuitStateNKakutouThirdData::k_DefaultBoostSpeed)
	, m_UseBoostNum(MobileSuitStateNKakutouThirdData::k_DefaultBoostUseNum)
{
}

void MobileSuitStateNKakutouThird::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateNKakutouSecond::Init GetDataが失敗");
		return;
	}
}

void MobileSuitStateNKakutouThird::ProcessInput()
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
	if (m_CountFrame >= MobileSuitStateNKakutouThirdData::k_AmountFrame)
	{
		m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
	}
}

void MobileSuitStateNKakutouThird::Update()
{
	// カウントアップ
	m_CountFrame++;
}

void MobileSuitStateNKakutouThird::OnEnter()
{
	if (m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateNKakutouThird::OnEnter nullptr");
		return;
	}
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("nkaku_03", 5);

	m_CountFrame = 0;

	m_RigidBodyComponent->SetDirection(Conv_XM::Vector3f(0.0f, 0.1f, 0.0f));
	m_RigidBodyComponent->SetIsGround(false);
	m_RigidBodyComponent->SetIsUseGravity(false);

	// 敵の方向に向ける
	RotateToEnemy();

	m_BeamSaber->StartNKakutou003();
}

void MobileSuitStateNKakutouThird::OnExit()
{
	m_RigidBodyComponent->SetIsUseGravity(true);

	m_BeamSaber->Reset();
}

void MobileSuitStateNKakutouThird::RotateToEnemy()
{
}
