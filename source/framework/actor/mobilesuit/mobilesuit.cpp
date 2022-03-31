
#include"../../../system/main.h"
#include "mobilesuit.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../component/mobilesuit_component/boost_component.h"
#include"../../component/mobilesuit_component/life_component.h"
#include"../../component/mobilesuit_component/arousal_component.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../component/collision_component/aabb_collision_component.h"
#include"../../component/mesh_component/debug/aabb_debug_view_component.h"
#include"../../actor_manager/effect_manager.h"
#include"../../../system/audio_manager.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state_component.h"

namespace MobileSuitData
{
	//! �f�t�H���g�̃_�E���J�E���g
	static constexpr int k_DefaultDownCount = 60 * 3;

	//! �f�t�H���g�̃N�[���^�C��
	static constexpr int DownCoolTime = 60 * 2;

	//! �f�t�H���g�̃R�X�g
	static constexpr int k_DefaultCost = 2000;

	//! �f�t�H���g�̃��C�t
	static constexpr int k_DefaultLife = 600;

	//! ����SE
	static const char* ExplosionSE = "asset/sound/SE/ExplosionSE.wav";
}

void MobileSuit::LoadSound()
{
	// �I�[�f�B�I
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Load(MobileSuitData::ExplosionSE);
}

void MobileSuit::UnloadSound()
{
	// �I�[�f�B�I
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Unload(MobileSuitData::ExplosionSE);
}

MobileSuit::MobileSuit()
	:m_RigidBodyComponent(nullptr)
	, m_BoostComponent(nullptr)
	, m_LifeComponent(nullptr)
	, m_ArousalComponent(nullptr)
	, m_OBBCollisionComponent(nullptr)
	, m_AABBCollisionComponent(nullptr)
	, m_StateComponent(nullptr)
	, m_IsBoostDash(false)
	, m_BeamRifleBulletManager(nullptr)
	, m_CannonManager(nullptr)
	, m_BazookaManager(nullptr)
	, m_AlliesMobileSuit(nullptr)
	, m_EnemyMobileSuit001(nullptr)
	, m_EnemyMobileSuit002(nullptr)
	, m_TargetNum(0)
	, m_BeamRifleCountFrame(0)
	, m_IsBeamRifleShot(false)
	, m_IsFinishBeamRifleShot(false)
	, m_BeamRifleDirection(0.0f, 0.0f, 0.0f)
	, m_BeamRifleCoolTime(0)
	, m_MobileSuitEffectManager(nullptr)
	, m_IsLockOn(false)
	, m_DownFlag(false)
	, m_DownNum(0)
	, m_DownCount(0)
	, m_IsDamage(false)
	, m_ColedType(ACTORTYPE_NONE)
	, m_CostManager(nullptr)
	, m_MyCost(MobileSuitData::k_DefaultCost)
{
}

MobileSuit::~MobileSuit()
{

}

void MobileSuit::Reset()
{
	m_LifeComponent->SetLife(MobileSuitData::k_DefaultLife);
}

void MobileSuit::BeamRifleShot()
{

}

void MobileSuit::ResetBeamRifleCoolTime()
{
}

void MobileSuit::SetIsMoveFlag(bool flag)
{
	UNREFERENCED_PARAMETER(flag);
}

void MobileSuit::LoadActor()
{
	//�@���W�b�h�{�f�B�R���|�[�l���g�쐬
	m_RigidBodyComponent = CreateComponent<RigidBodyComponent>();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_RigidBodyComponent��nullptr");
		return;
	}
	m_RigidBodyComponent->SetIsMobileSuit(true);

	// �u�[�X�g�R���|�[�l���g�쐬
	m_BoostComponent = CreateComponent<BoostComponent>();
	if (m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_BoostComponent��nullptr");
		return;
	}

	// ���C�t�R���|�[�l���g�쐬
	m_LifeComponent = CreateComponent<LifeComponent>();
	if (m_LifeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_LifeComponent��nullptr");
		return;
	}

	// �o���R���|�[�l���g�쐬
	m_ArousalComponent = CreateComponent<ArousalComponent>();
	if (m_ArousalComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_ArousalComponent��nullptr");
		return;
	}

	// OBB�R���W�����R���|�[�l���g�쐬
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_OBBCollisionComponent��nullptr");
		return;
	}

	// OBB�f�o�b�O�r���[�쐬
	CreateComponent<OBBDebugViewComponent>();


	// AABB�R���W�����R���|�[�l���g�쐬
	m_AABBCollisionComponent = CreateComponent<AABBCollisionComponent>();
	if (m_AABBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_AABBCollisionComponent��nullptr");
		return;
	}
	// AABB�f�o�b�O�r���[�쐬
	CreateComponent<AABBDebugViewComponent>();
}

void MobileSuit::InitActor()
{
	if (m_LifeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_LifeComponent��nullptr");
		return;
	}

	m_LifeComponent->SetMaxLife(MobileSuitData::k_DefaultLife);
}

void MobileSuit::UpdateActor()
{
	if (m_LifeComponent->GetLife() <= 0.0f)
	{
		SetActive(false);
		m_MobileSuitEffectManager->UseEffect(m_Position);
		m_StateComponent->ChangeMobileSuitState("MobileSuitStateAirIdle");
		m_RigidBodyComponent->SetIsGround(false);
		// ����炷
		AudioManager& audioManager = AudioManager::GetInstance();
		audioManager.Play("asset/sound/SE/ExplosionSE.wav", false);

		m_IsBoostDash = false;
		m_BeamRifleCountFrame = 0;
		m_IsBeamRifleShot = false;
		m_IsFinishBeamRifleShot = false;
		m_BeamRifleCoolTime = 0;
		m_IsLockOn = false;
		m_DownFlag = false;
		m_DownNum = 0.0f;
		m_DownCount = 0.0f;
		m_IsDamage = false;
		m_DownNumCoolTime = 0;

		return;
	}

	if (m_DownFlag == true)
	{
 		m_DownCount++;
		if (m_DownCount > MobileSuitData::k_DefaultDownCount)
		{
			m_DownFlag = false;
			m_DownCount = 0;
			m_DownNum = 0;
			m_DownNumCoolTime = 0;
		}
	}
	else
	{
		if (m_DownNum > 0)
		{
			m_DownNumCoolTime++;

			if (m_DownNumCoolTime > MobileSuitData::DownCoolTime)
			{
				m_DownNum = 0;
			}
		}
	}
}
