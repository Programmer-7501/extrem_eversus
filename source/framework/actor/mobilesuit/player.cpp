
#include"../../../system/main.h"
#include "player.h"
#include"../../component/cameracomponent/lockon_camera_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../component/mesh_component/model_component.h"
#include"../../component/mobilesuit_component/boost_component.h"
#include"../../component/mobilesuit_component/life_component.h"
#include"../../../gamedata/damage_data.h"
#include"../../component/mesh_component/animation_model_component.h"
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
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_nkakutou_first.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_nkakutou_second.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_nkakutou_third.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_turn_around_beamrifle_shot.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_down.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_return.h"
#include"../../component/mobilesuit_state_component/mobilesuit_state/mobilesuit_state_bazooka.h"
#include"../../component/mesh_component/lockon_cursol_billboard_component.h"
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
#include"../gamemanager/cost_manager.h"
#include"../../../system/audio_manager.h"

namespace PlayerData
{
	//! モデルファイルネーム
	static const char* k_PlayerAnimationModelFileName = "asset/model/robot/guhu_anime.bin";

	//! ビームライフルを撃つまでの時間
	static constexpr int k_BeamRifleShotTime = 12;

	//! ビームライフルを撃つ全体フレーム
	static constexpr int k_BeamRifleShotAmoutTime = 48;

	//! ビームライフルクールタイム
	static constexpr int k_BeamRifleCoolTime = 36;

	//! 振り向き撃ちになる角度(degree)
	static constexpr float k_TurnAroundDegree = 110;

	//! ロックオンカーソルの最大値
	static constexpr float k_LockOnCursolMaxSize = 5.0f;
	//! ロックオンカーソルの最小値
	static constexpr float k_LockOnCursolMinSize = 2.0f;

	//! 赤ロックオンカーソルテクスチャファイルネーム
	static const char* k_RedLockOnCursolTextureFileName = "asset/texture/redlock.png";
	//! 緑ロックオンカーソルテクスチャファイルネーム
	static const char* k_GreenLockOnCursolTextureFileName = "asset/texture/greenlock.png";
	//! 黄ロックオンカーソルテクスチャファイルネーム
	static const char* k_YellowLockOnCursolTextureFileName = "asset/texture/yellowlock.png";

	//! デフォルトのロックオン距離
	static constexpr float k_DefaultLockOnDistanceSq = 1000.0f;

	//! コリジョンオフセットスケール
	static const Conv_XM::Vector3f k_OBBCollisionOffsetScale = Conv_XM::Vector3f(0.5f, 2.4f, 0.5f);
	//! コリジョンオフセットポジション
	static const Conv_XM::Vector3f k_OBBCollisionOffsetPosition = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);

	//! ショットSE
	static const char* ShotSE = "asset/sound/SE/ShotSE.wav";
	
	//! 命中SE
	static const char* HitSE = "asset/sound/SE/HitSE.wav";

	//! 倒れるSE
	static const char* DownSE = "asset/sound/SE/DownSE.wav";
}

void Player::LoadSound()
{
	// オーディオ
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Load(PlayerData::ShotSE);
	audioManager.Load(PlayerData::HitSE);
	audioManager.Load(PlayerData::DownSE);
}

void Player::UnloadSound()
{
	// オーディオ
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Unload(PlayerData::ShotSE);
	audioManager.Unload(PlayerData::HitSE);
	audioManager.Unload(PlayerData::DownSE);
}

void Player::LoadModel()
{
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	animationModelManager.Load(PlayerData::k_PlayerAnimationModelFileName);
}

void Player::UnloadModel()
{
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	animationModelManager.Unload(PlayerData::k_PlayerAnimationModelFileName);
}

Player::Player()
	:m_LockOnCameraComponent(nullptr)
	, m_LockonCursolBillboardComponent(nullptr)
	, m_AnimationModelComponent(nullptr)
	, m_BoostDashOffsetRotation(DirectX::XMQuaternionIdentity())
	, m_OffsetRotation(DirectX::XMQuaternionIdentity())
	, m_StorePosition(0.0f,0.0f,0.0f)
{
	m_ActorType = Actor::ACTORTYPE_PLAYER;
}

Player::~Player()
{
}


void Player::BeamRifleShot()
{
	if (m_StateComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::BeamRifleShot m_StateComponentがnullptr");
		return;
	}

	if (m_BeamRifleBulletManager->GetCurrentBullet() <= 0 || m_IsBeamRifleShot == true)
	{
		return;
	}

	//　フラグを立てる
	m_IsBeamRifleShot = true;
	m_IsFinishBeamRifleShot = false;

	// カウント初期化
	m_BeamRifleCountFrame = 0;

	m_BeamRifleCoolTime = PlayerData::k_BeamRifleCoolTime;

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

	// 振り向きうちの角度を超えてたら
	if (sita > DirectX::XMConvertToRadians(PlayerData::k_TurnAroundDegree))
	{
		m_StateComponent->ChangeMobileSuitState("MobileSuitStateTurnAroundBeamRifleShot");
	}
}

void Player::ResetBeamRifleCoolTime()
{
	m_IsBeamRifleShot = false;
	m_BeamRifleCountFrame = 0;
}

void Player::CollisionDetected(Actor * coleObj)
{
	if (m_LifeComponent == nullptr || m_StateComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::CollisionDetected nullptr");
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

	if (coleObj->GetActorType() == Actor::ACTORTYPE_ENEMY)
	{
		Conv_XM::Vector3f pos = m_Position;
		pos.x = m_StorePosition.x;
		pos.z = m_StorePosition.z;
		SetPosition(pos);
		ComputeWorldTransform();
	}

	if (coleObj->GetActorType() == Actor::ACTORTYPE_ENEMY_BULLET)
	{
		m_LifeComponent->PlusLife(-DamageData::k_BeamRifleDamage);
		m_DownNum += DamageData::k_BeamRifleDown;
		m_StateComponent->ChangeMobileSuitState("MobileSuitStateBendBackWard");
		m_DownNumCoolTime = 0;

		AudioManager& audioManager = AudioManager::GetInstance();
		audioManager.Play(PlayerData::HitSE,false);
	}


	if (m_DownNum > GameData::k_DefaultDownNum)
	{
		m_StateComponent->ChangeMobileSuitState("MobileSuitStateDown");
		m_DownFlag = true;

		AudioManager& audioManager = AudioManager::GetInstance();
		audioManager.Play(PlayerData::DownSE, false);
	}
}

void Player::SetActive(bool flag)
{
	// アクティブフラグを変更
	m_Active = flag;

	// 非アクティブになった場合
	if (m_Active == false)
	{
		// nullチェック
		if (m_CostManager == nullptr)
		{
			Logger::GetInstance().SetLog("Player::SetActive m_CostManagerがnullptr");
			return;
		}

		// コスト変更
		m_CostManager->DecreaseMyCost(m_MyCost);
	}
	else// アクティブになったら
	{
		// 球の初期化
		m_BeamRifleBulletManager->Reset();
	}
}

void Player::SetIsMoveFlag(bool flag)
{
	// nullチェック
	if (m_StateComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::SetActive m_StateComponentがnullptr");
		return;
	}

	m_StateComponent->SetIsMoveFlag(flag);
}

void Player::LoadActor()
{
	// モビルスーツの読み込み処理
	MobileSuit::LoadActor();

	// モデルの読み込み
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	AnimationModel* animation = animationModelManager.GetModel(PlayerData::k_PlayerAnimationModelFileName);;
	if (animation == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor animationがnullptr");
		return;
	}

	// アニメーションモデルコンポーネント作成
	m_AnimationModelComponent = CreateComponent<AnimationModelComponent>();
	if (m_AnimationModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuit::InitActor m_AnimationModelComponentがnullptr");
		return;
	}
	m_AnimationModelComponent->SetAnimationModel(animation);
	m_AnimationModelComponent->SetAnimation("walk");

	// カメラコンポーネント作成
	m_LockOnCameraComponent = CreateComponent<LockOnCameraComponent>();
	if (m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor m_LockOnCameraComponentがnullptr");
		return;
	}

	// ロックオンカーソルコンポーネントの作成
	m_LockonCursolBillboardComponent = CreateComponent<LockonCursolBillboardComponent>();
	if (m_LockonCursolBillboardComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor m_LockonCursolBillboardComponentがnullptr");
		return;
	}

	// ステートコンポーネントの作成
	m_StateComponent = CreateComponent<MobileSuitStateComponent>();
	if (m_StateComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor m_StateComponentがnullptr");
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
	m_StateComponent->CreateMobileSuitState<MobileSuitStateJumpStandby>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateNKakutouFirst>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateNKakutouSecond>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateNKakutouThird>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateDown>();
	m_StateComponent->CreateMobileSuitState<MobileSuitStateReturn>();

	m_StateComponent->CreateMobileSuitState<MobileSuitStateBazooka>();
}

void Player::InitActor()
{
	// モビルスーツの初期化
	MobileSuit::InitActor();

	// オフセットローテーションの入力
	m_OffsetRotation = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldRight, DirectX::XMConvertToRadians(-90.0f));
	m_AnimationModelComponent->SetOffsetQuaternion(m_OffsetRotation);
	m_AnimationModelComponent->SetOffsetScale(Conv_XM::Vector3f(0.25f, 0.25f, 0.25f));

		// オフセットローテーションを入力
	m_BoostDashOffsetRotation = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldRight, DirectX::XMConvertToRadians(-40.0f));

	// ロックオンカメラの入力
	m_LockOnCameraComponent->SetPosition(m_Position);

	// ターゲット設定
	m_LockOnCameraComponent->SetTarget(m_EnemyMobileSuit001);

	// メインカメラのセット
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor currentSceneがnullptr");
		return;
	}
	SceneRenderManager* sceneRenderManager = currentScene->GetSceneRenderManager();
	if (sceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor sceneRenderManagerがnullptr");
		return;
	}
	sceneRenderManager->SetMainCameraComponent(m_LockOnCameraComponent);

	m_AABBCollisionComponent->SetOffsetTransform(
		Conv_XM::Vector3f(0.0f, -1.0f, 0.0f),
		Conv_XM::Vector3f(0.5f, 2.2f, 0.5f)
	);

	// シーンコリジョンマネージャーに設定する
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor sceneCollisionManagerがnullptr");
		return;
	}
	sceneCollisionManager->SetMobileSuitData(SceneCollisionManager::PLAYER, this, m_RigidBodyComponent, m_OBBCollisionComponent, m_AABBCollisionComponent);
	m_RigidBodyComponent->SetIsGround(false);

	if (m_StateComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor m_StateComponentがnullptr");
		return;
	}

	// コンポーネントをセット
	m_StateComponent->SetComponent(m_RigidBodyComponent, m_LockOnCameraComponent, m_BoostComponent, m_AnimationModelComponent);

	// 初期状態を登録
	m_StateComponent->ChangeMobileSuitState("MobileSuitStateIdle");

	if (m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor nullptr");
		return;
	}


	// 初期の回転を敵に向かせる
	Conv_XM::Vector3f epos = m_EnemyMobileSuit001->GetPosition();
	Conv_XM::Vector3f pos = m_Position;
	Conv_XM::Vector3f dif = epos - m_Position;
	dif.y = 0;
	dif = DirectX::XMVector3Normalize(dif);

	Conv_XM::Vector3f axis;
	Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, dif, &axis);

	float dot = Conv_XM::Vector3f::Dot(dif, Conv_XM::Vector3f::WorldForward);
	float sita = acosf(dot);

	Conv_XM::Vector4f q = DirectX::XMQuaternionRotationAxis(axis, sita);
	SetQuaternion(q);

	ID3D11ShaderResourceView* redSRV = TextureManager::GetInstance().Load(PlayerData::k_RedLockOnCursolTextureFileName);
	ID3D11ShaderResourceView* greenSRV = TextureManager::GetInstance().Load(PlayerData::k_GreenLockOnCursolTextureFileName);
	ID3D11ShaderResourceView* yellowSRV = TextureManager::GetInstance().Load(PlayerData::k_YellowLockOnCursolTextureFileName);
	if (m_LockonCursolBillboardComponent == nullptr || redSRV == nullptr || greenSRV == nullptr || yellowSRV == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor nullptr");
		return;
	}
	// ロックのテクスチャセット
	m_LockonCursolBillboardComponent->SetSRV(redSRV, LockonCursolBillboardComponent::LOCKONSTATE_RED);
	m_LockonCursolBillboardComponent->SetSRV(greenSRV, LockonCursolBillboardComponent::LOCKONSTATE_GREEN);
	m_LockonCursolBillboardComponent->SetSRV(yellowSRV, LockonCursolBillboardComponent::LOCKONSTATE_YELLOW);
	m_LockonCursolBillboardComponent->SetPosition(m_EnemyMobileSuit001->GetPosition());
	m_LockonCursolBillboardComponent->SetLockOnState(LockonCursolBillboardComponent::LOCKONSTATE_GREEN);

	// バーナーパーティクル設定
	CBufferData::ParticleEmitter emitter = {};
	emitter.EmitterPosition = m_Position;
	emitter.EmitterPosition -= GetForward();
	emitter.ParticleVelocity = Conv_XM::Vector4f(0.0f, -0.0f, 0.0f, 0.0f);
	emitter.MaxSpawnParticlesThisFrame = 20;
	emitter.ParticleLifeSpan = 4;
	emitter.PositionVariance = Conv_XM::Vector4f(0.01f, 0.02f, 0.01f, 0.0f);
	emitter.Color = Conv_XM::Vector4f(0.0f, 0.3f, 0.8f, 1.0f);
	emitter.IsUse = 3;
	emitter.Size = Conv_XM::Vector2f(0.4f, 0.4f);
	emitter.EmitterType = CBufferData::ET_RECT;
	emitter.SizeOverLifeTime = 1;
	m_BurnerEmitter001 = emitter;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	m_BurnerEmitter001.MyIndex = particleManager.RegisterParticleEmitter(m_BurnerEmitter001);

	//! OBBコリジョン調整
	m_OBBCollisionComponent->SetOffsetScale(PlayerData::k_OBBCollisionOffsetScale);
	m_OBBCollisionComponent->SetOffsetPosition(PlayerData::k_OBBCollisionOffsetPosition);
}

void Player::UninitActor()
{
	// テクスチャ破棄
	TextureManager::GetInstance().Unload(PlayerData::k_RedLockOnCursolTextureFileName);
	TextureManager::GetInstance().Unload(PlayerData::k_GreenLockOnCursolTextureFileName);
	TextureManager::GetInstance().Unload(PlayerData::k_YellowLockOnCursolTextureFileName);
}

void Player::ProcessInputActor()
{
	// nullチェック
	if (m_LockonCursolBillboardComponent == nullptr || m_AnimationModelComponent == nullptr || m_OBBCollisionComponent == nullptr || m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Player::ProcessInputActor nullptr");
		return;
	}

	// カメラ切り替え
	InputManager& inputManager = InputManager::GetInstance();
	if (inputManager.GetIsTrigger(InputManager::IN_DECIDE))
	{
		if (m_TargetNum == 0)
		{
			// ターゲット設定
			m_LockOnCameraComponent->SetTarget(m_EnemyMobileSuit002);
			m_TargetNum = 1;
		}
		else if (m_TargetNum == 1)
		{
			// ターゲット設定
			m_LockOnCameraComponent->SetTarget(m_EnemyMobileSuit001);
			m_TargetNum = 0;
		}
	}
	// エネミーが非アクティブになった場合
	if (m_EnemyMobileSuit001->GetActive() == false)
	{
		m_TargetNum = 1;
		// ターゲット設定
		m_LockOnCameraComponent->SetTarget(m_EnemyMobileSuit002);
	}
	if (m_EnemyMobileSuit002->GetActive() == false)
	{
		m_TargetNum = 0;
		// ターゲット設定
		m_LockOnCameraComponent->SetTarget(m_EnemyMobileSuit001);
	}

	// ロックオンカーソルの切り替え
	Conv_XM::Vector3f dif;
	float distanceSq;

	// ターゲットはどっちか
	if (m_TargetNum == 0)
	{
		// ロックオンビルボードコンポーネントのポジション入力
		m_LockonCursolBillboardComponent->SetPosition(m_EnemyMobileSuit001->GetPosition());

		// エネミーとの距離を測る
		dif = m_EnemyMobileSuit001->GetPosition() - m_Position;

		// 長さの2乗を図る
		distanceSq = Conv_XM::Vector3f::LengthSq(dif);

		// ダウンしているかどうか
		bool isDown = m_EnemyMobileSuit001->GetDownFlag();
		if (isDown == true)
		{
			m_LockonCursolBillboardComponent->SetLockOnState(LockonCursolBillboardComponent::LOCKONSTATE_YELLOW);
		}
		else// ダウンしてないなら
		{
			if (distanceSq > PlayerData::k_DefaultLockOnDistanceSq)
			{
				m_LockonCursolBillboardComponent->SetLockOnState(LockonCursolBillboardComponent::LOCKONSTATE_GREEN);
				m_IsLockOn = false;
			}
			else
			{
				m_LockonCursolBillboardComponent->SetLockOnState(LockonCursolBillboardComponent::LOCKONSTATE_RED);
				m_IsLockOn = true;
			}
		}
		float lockOnCursolSize = Math::Clamp(distanceSq / 200.0f, PlayerData::k_LockOnCursolMinSize, PlayerData::k_LockOnCursolMaxSize);
		m_LockonCursolBillboardComponent->SetSize(Conv_XM::Vector2f(lockOnCursolSize, lockOnCursolSize));
	}
	else
	{
		// ロックオンビルボードコンポーネントのポジション入力
		m_LockonCursolBillboardComponent->SetPosition(m_EnemyMobileSuit002->GetPosition());

		// エネミーとの距離を測る
		dif = m_EnemyMobileSuit002->GetPosition() - m_Position;

		// 長さの2乗を図る
		distanceSq = Conv_XM::Vector3f::LengthSq(dif);

		// ダウンしているかどうか
		bool isDown = m_EnemyMobileSuit002->GetDownFlag();
		if (isDown == true)
		{
			m_LockonCursolBillboardComponent->SetLockOnState(LockonCursolBillboardComponent::LOCKONSTATE_YELLOW);
		}
		else
		{
			if (distanceSq > PlayerData::k_DefaultLockOnDistanceSq)
			{
				m_LockonCursolBillboardComponent->SetLockOnState(LockonCursolBillboardComponent::LOCKONSTATE_GREEN);
				m_IsLockOn = false;
			}
			else
			{
				m_LockonCursolBillboardComponent->SetLockOnState(LockonCursolBillboardComponent::LOCKONSTATE_RED);
				m_IsLockOn = true;
			}
		}
		float lockOnCursolSize = Math::Clamp(distanceSq / 200.0f, PlayerData::k_LockOnCursolMinSize, PlayerData::k_LockOnCursolMaxSize);
		m_LockonCursolBillboardComponent->SetSize(Conv_XM::Vector2f(lockOnCursolSize, lockOnCursolSize));
	}

	// コリジョンを動かす
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
	Conv_XM::Vector4f posture = DirectX::XMQuaternionSlerp( m_OffsetRotation, m_BoostDashOffsetRotation, m_QuaternionCount);
	Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldRight, DirectX::XMConvertToRadians(30.0f));
	Conv_XM::Vector4f q2 = DirectX::XMQuaternionIdentity();
	Conv_XM::Vector4f posture2 = DirectX::XMQuaternionSlerp(q2, q1, m_QuaternionCount);
	m_OBBCollisionComponent->SetOffsetQuaternion(posture2);

	// ビームライフルを打つ
	if (m_IsBeamRifleShot == true)
	{
		// ビームライフルを撃つフレームになったら
		if (m_BeamRifleCountFrame > PlayerData::k_BeamRifleShotTime && m_IsFinishBeamRifleShot == false)
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

			// ロックオンしていたら
			if (m_IsLockOn == true)
			{
				m_BeamRifleBulletManager->UseBullet(target, m_Position, m_BeamRifleDirection);
			}
			else
			{
				m_BeamRifleBulletManager->UseBullet(nullptr, m_Position, m_BeamRifleDirection);
			}

			// 音を鳴らす
			AudioManager& audioManager = AudioManager::GetInstance();
			audioManager.Play(PlayerData::ShotSE, false);
		}

		// 全体フレームとクールタイムを超えたら
		if (m_BeamRifleCountFrame > PlayerData::k_BeamRifleShotAmoutTime + m_BeamRifleCoolTime)
		{
			m_IsBeamRifleShot = false;
			m_BeamRifleCountFrame = 0;
		}

		// カウントアップ
		m_BeamRifleCountFrame++;
	}

	// ポジションを保存する
	m_StorePosition = m_Position;
}
