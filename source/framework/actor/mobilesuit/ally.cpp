
#include"../../../system/main.h"
#include "ally.h"
#include"../../component/cameracomponent/lockon_camera_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../component/mesh_component/model_component.h"
#include"../../component/mobilesuit_component/boost_component.h"
#include"../../component/mesh_component/animation_model_component.h"
#include"../../component/mobilesuit_component/life_component.h"
#include"../../../system/input_manager.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../actor_manager/bullet_manager.h"
#include"../../../directx/resource/texture_manager.h"
#include"../../component/mesh_component/animation_model_component.h"
#include"../../../directx/resource/animation_model_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/collision_component/aabb_collision_component.h"
#include"../../../particle/particle_manager.h"
#include"../../../gamedata/damage_data.h"

#include"../../component/mobilesuit_state_component/mobilesuit_state_component.h"

#include"../../component/mobilesuit_state_component/mobilesuit_state_component.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_idle.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_jump_standby.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_walk.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_boost_jump.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_boost_dash.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_beamrifle_shot.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_cannon_shot.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_landing_recovery.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_air_idle.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_step.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_bend_backward.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_turn_around_beamrifle_shot.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_down.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_return.h"
#include"../../component/mobilesuit_state_component/mobilesuit_ai_state_component.h"
#include"../../component/mobilesuit_state_component/character_ai_state/character_ai_state_move.h"
#include"../../component/mobilesuit_state_component/character_ai_state/character_ai_state_start.h"
#include"../../component/mobilesuit_state_component/character_ai_state/character_ai_state_attack.h"

#include"../gamemanager/cost_manager.h"

namespace AllyData
{
	static const char* k_EnemyAnimationModelFileName = "asset/model/robot/guhu_anime.bin";

	//! デフォルトコスト
	static const int k_DefaultCost = 2000;

	//! デフォルトのロックオン距離
	static constexpr float k_DefaultLockOnDistanceSq = 1000.0f;

	//! 振り向き撃ちになる角度(degree)
	static constexpr float k_TurnAroundDegree = 110;
}

AnimationModel* Ally::m_AnimationModel = nullptr;

void Ally::LoadModel()
{
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	m_AnimationModel = animationModelManager.Load(AllyData::k_EnemyAnimationModelFileName);
}

void Ally::UnloadModel()
{
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	animationModelManager.Unload(AllyData::k_EnemyAnimationModelFileName);
}

Ally::Ally()
	:m_LockOnCameraComponent(nullptr)
	, m_StateComponent(nullptr)
	, m_AnimationModelComponent(nullptr)
	, m_BoostDashOffsetRotation(DirectX::XMQuaternionIdentity())
	, m_OffsetRotation(DirectX::XMQuaternionIdentity())
	, m_QuaternionCount(0.0f)
	, m_BurnerEmitter001{}
	, m_BurnerEmitter002{}
	, m_StorePosition(0.0f, 0.0f, 0.0f)
{
	m_ActorType = Actor::ACTORTYPE_ALLIES;
}

Ally::~Ally()
{
}

void Ally::BeamRifleShot()
{
	if (m_BeamRifleBulletManager->GetCurrentBullet() <= 0 || m_IsBeamRifleShot == true)
	{
		return;
	}

	//　フラグを立てる
	m_IsBeamRifleShot = true;
	m_IsFinishBeamRifleShot = false;

	// カウント初期化
	m_BeamRifleCountFrame = 0;

	m_BeamRifleCoolTime = 36;

	// 狙う方向を決める
	if (m_TargetNum == 0)
	{
		m_BeamRifleDirection = m_EnemyMobileSuit001->GetPosition() - m_Position;
	}
	else
	{
		m_BeamRifleDirection = m_EnemyMobileSuit002->GetPosition() - m_Position;
	}
	m_BeamRifleDirection.y = 0.0f;
	m_BeamRifleDirection = DirectX::XMVector3Normalize(m_BeamRifleDirection);

	// 狙う方向と自身の前ベクトルの内積を取得
	Conv_XM::Vector3f myForward = GetForward();
	myForward.y = 0.0f;
	myForward = DirectX::XMVector3Normalize(myForward);

	float dot = Conv_XM::Vector3f::Dot(m_BeamRifleDirection, myForward);
	float sita = acosf(dot);

	//// 振り向きうちの角度を超えてたら
	//if (sita > DirectX::XMConvertToRadians(EnemyData::k_TurnAroundDegree))
	//{
	//	m_StateComponent->ChangeMobileSuitState("MobileSuitStateTurnAroundBeamRifleShot");
	//}
}

void Ally::ResetBeamRifleCoolTime()
{
	m_IsBeamRifleShot = false;
	m_BeamRifleCountFrame = 0;
}

void Ally::LoadActor()
{
	// モビルスーツの読み込み処理
	MobileSuit::LoadActor();

	// アニメーションモデルコンポーネント作成
	m_AnimationModelComponent = CreateComponent<AnimationModelComponent>();
	if (m_AnimationModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Enemy::LoadActor m_AnimationModelComponentがnullptr");
		return;
	}
	m_AnimationModelComponent->SetAnimationModel(m_AnimationModel);
	m_AnimationModelComponent->SetAnimation("walk");

	// カメラコンポーネント作成
	m_LockOnCameraComponent = CreateComponent<LockOnCameraComponent>();
	if (m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Enemy::InitActor m_LockOnCameraComponentがnullptr");
		return;
	}

	// ステートコンポーネントの作成
	m_StateComponent = CreateComponent<MobileSuitAIStateComponent>();
	if (m_StateComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Enemy::InitActor m_StateComponentがnullptr");
		return;
	}


	// 状態作成
	m_StateComponent->CreateMobileSuitState<MobileSuitStateIdle>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateWalk>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateBoostJump>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateBoostDash>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateBeamRifleShot>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateCannonShot>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateLandingRecovery>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateAirIdle>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateTurnAroundBeamRifleShot>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateStep>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateBendBackWard>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateDown>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateReturn>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateJumpStandby>();

	MobileSuitAIStateComponent* mobile = dynamic_cast<MobileSuitAIStateComponent*>(m_StateComponent);
	mobile->CreateCharacterState<CharcterAIStateMove>();
	mobile->CreateCharacterState<CharcterAIStateStart>();
	mobile->CreateCharacterState<CharcterAIStateAttack>();
}

void Ally::InitActor()
{
	// モビルスーツの初期化
	MobileSuit::InitActor();

	// オフセットローテーションの入力
	m_OffsetRotation = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldRight, DirectX::XMConvertToRadians(-90.0f));
	m_AnimationModelComponent->SetOffsetQuaternion(m_OffsetRotation);
	m_AnimationModelComponent->SetOffsetScale(Conv_XM::Vector3f(0.25f, 0.25f, 0.25f));

	m_OBBCollisionComponent->SetOffsetTransform(
		Conv_XM::Vector3f(0.0f, -1.0f, 0.0f),
		Conv_XM::Vector3f(0.5f, 2.2f, 0.5f),
		DirectX::XMQuaternionIdentity()
	);

	m_AABBCollisionComponent->SetOffsetTransform(
		Conv_XM::Vector3f(0.0f, -1.0f, 0.0f),
		Conv_XM::Vector3f(0.5f, 2.2f, 0.5f)
	);

	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* currentScene = sceneManager.GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Ally::InitActor currentSceneがnullptr");
		return;
	}
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("Ally::InitActor sceneCollisionManagerがnullptr");
		return;
	}
	//　コリジョンのセット
	sceneCollisionManager->SetMobileSuitData(SceneCollisionManager::ALLY, this, m_RigidBodyComponent, m_OBBCollisionComponent, m_AABBCollisionComponent);


	if (m_StateComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Ally::InitActor m_StateComponentがnullptr");
		return;
	}

	// コンポーネントをセット
	m_StateComponent->SetComponent(m_RigidBodyComponent, m_LockOnCameraComponent, m_BoostComponent, m_AnimationModelComponent);

	// 初期状態を登録
	m_StateComponent->ChangeMobileSuitState("MobileSuitStateIdle");
	MobileSuitAIStateComponent* mobile = dynamic_cast<MobileSuitAIStateComponent*>(m_StateComponent);
	mobile->SetWeightedGraph(m_WeightedGraph);
	mobile->ChangeCharacterAIState("CharcterAIStateStart");
}

void Ally::ProcessInputActor()
{
	if (m_IsBoostDash == true)
	{
		m_QuaternionCount += 0.1f;
		if (m_QuaternionCount > 1.0f)
		{
			m_QuaternionCount = 1.0f;
		}
	}
	else
	{
		m_QuaternionCount -= 0.1f;
		if (m_QuaternionCount < 0.0f)
		{
			m_QuaternionCount = 0.0f;
		}
	}
	Conv_XM::Vector4f posture = DirectX::XMQuaternionSlerp(m_OffsetRotation, m_BoostDashOffsetRotation, m_QuaternionCount);
	Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldRight, DirectX::XMConvertToRadians(30.0f));
	Conv_XM::Vector4f q2 = DirectX::XMQuaternionIdentity();
	Conv_XM::Vector4f posture2 = DirectX::XMQuaternionSlerp(q2, q1, m_QuaternionCount);
	m_OBBCollisionComponent->SetOffsetQuaternion(posture2);

	if (m_IsBeamRifleShot == true)
	{
		// ビームライフルを撃つフレームになったら
		if (m_BeamRifleCountFrame > 12 && m_IsFinishBeamRifleShot == false)
		{
			MobileSuit* target = nullptr;
			// 狙う方向を決める
			if (m_TargetNum == 0)
			{
				m_BeamRifleDirection = m_EnemyMobileSuit001->GetPosition() - m_Position;
				target = m_EnemyMobileSuit001;
			}
			else
			{
				m_BeamRifleDirection = m_EnemyMobileSuit002->GetPosition() - m_Position;
				target = m_EnemyMobileSuit002;
			}

			m_BeamRifleDirection = DirectX::XMVector3Normalize(m_BeamRifleDirection);

			m_IsFinishBeamRifleShot = true;

			if (m_IsLockOn == true)
			{
				m_BeamRifleBulletManager->UseBullet(target, m_Position, m_BeamRifleDirection);
			}
			else
			{
				m_BeamRifleBulletManager->UseBullet(nullptr, m_Position, m_BeamRifleDirection);
			}
		}

		// 全体フレームとクールタイムを超えたら
		if (m_BeamRifleCountFrame > 48 + m_BeamRifleCoolTime)
		{
			m_IsBeamRifleShot = false;
			m_BeamRifleCountFrame = 0;
		}

		// カウントアップ
		m_BeamRifleCountFrame++;
	}


	// ロックオンカーソルの切り替え
	Conv_XM::Vector3f dif;
	float distanceSq;

	// ターゲットはどっちか
	if (m_TargetNum == 0)
	{
		// エネミーとの距離を測る
		dif = m_EnemyMobileSuit001->GetPosition() - m_Position;

		// 長さの2乗を図る
		distanceSq = Conv_XM::Vector3f::LengthSq(dif);

		// ロックオン距離かどうかを判定
		if (distanceSq > AllyData::k_DefaultLockOnDistanceSq)
		{
			m_IsLockOn = false;
		}
		else
		{
			m_IsLockOn = true;
		}
	}
	else
	{
		// エネミーとの距離を測る
		dif = m_EnemyMobileSuit002->GetPosition() - m_Position;

		// 長さの2乗を図る
		distanceSq = Conv_XM::Vector3f::LengthSq(dif);

		// ロックオン距離かどうかを判定
		if (distanceSq > AllyData::k_DefaultLockOnDistanceSq)
		{
			m_IsLockOn = false;
		}
		else
		{
			m_IsLockOn = true;
		}
	}
}

void Ally::CollisionDetected(Actor * coleObj)
{
	if (m_LifeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Enemy::CollisionDetected nullptr");
		return;
	}

	if (m_DownFlag == true)
	{
		return;
	}

	if (coleObj == nullptr)
	{
		Conv_XM::Vector3f pos = m_Position;
		pos.x = m_StorePosition.x;
		pos.z = m_StorePosition.z;
		SetPosition(pos);
		ComputeWorldTransform();
		return;
	}

	Actor::ActorType coleObjActorType = coleObj->GetActorType();

	if (coleObjActorType == Actor::ACTORTYPE_ENEMY_BULLET)
	{
		m_LifeComponent->PlusLife(-DamageData::k_BeamRifleDamage);
		m_DownNum += DamageData::k_BeamRifleDown;
		m_StateComponent->ChangeMobileSuitState("MobileSuitStateBendBackWard");
		m_DownNumCoolTime = 0;
	}

	if (m_DownNum > GameData::k_DefaultDownNum)
	{
		m_StateComponent->ChangeMobileSuitState("MobileSuitStateDown");
		m_DownFlag = true;
	}

	//switch (coleObjActorType)
	//{
	//case Actor::ACTORTYPE_ENEMY_BULLET:
	//	m_LifeComponent->PlusLife(-DamageData::k_BeamRifleDamage);
	//	m_DownNum += DamageData::k_BeamRifleDown;
	//	break;
	//case Actor::ACTORTYPE_BEAMRIFLE:
	//	break;
	//case Actor::ACTORTYPE_CANNON:
	//	m_LifeComponent->PlusLife(-DamageData::k_CannonDamage);
	//	m_DownNum += DamageData::k_CannonDown;
	//	break;
	//default:
	//	break;
	//}

	if (m_DownNum > GameData::k_DefaultDownNum)
	{
		m_DownFlag = true;
	}
}

void Ally::SetActive(bool flag)
{
	// アクティブフラグを変更
	m_Active = flag;

	// 非アクティブになった場合
	if (m_Active == false)
	{
		// nullチェック
		if (m_CostManager == nullptr)
		{
			Logger::GetInstance().SetLog("Enemy::SetActive m_CostManagerがnullptr");
			return;
		}

		// コストを減らす
		m_CostManager->DecreaseMyCost(m_MyCost);
	}
}

void Ally::SetIsMoveFlag(bool flag)
{
	m_StateComponent->SetIsMoveFlag(flag);
}
