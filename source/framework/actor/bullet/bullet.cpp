
#include"../../../system/main.h"
#include "bullet.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../../particle/particle_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../actor_manager/effect_manager.h"

namespace BulletData
{
	//! デフォルトの速度
	static constexpr float k_DefaultSpeed = 0.75f;

	//! デフォルトの終了フレーム
	static constexpr int k_DefaultFinishFrame = 300;

	//! 弾の色
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.8f, 1.0f);
	//! 1フレーム中に発生させるパーティクル数
	static constexpr int k_DefaultSpawnNum = 2;
	//! ライフ
	static constexpr float k_DefaultLife = 5.0f;
	//! パーティクルサイズ
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(1.0f, 1.0f);
	//! 使用エミッタータイプ
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_SPHERE;

	//! 最大回転量
	static constexpr float k_MaxRotate = 0.25f;
}

Bullet::Bullet()
	: m_RigidBodyComponent(nullptr)
	, m_OBBCollisionComponent(nullptr)
	, m_Speed(BulletData::k_DefaultSpeed)
	, m_Emitter{}
	, m_ParticleEmitterIndex(-1)
	, m_FrameCount(0)
	, m_FinishFrame(BulletData::k_DefaultFinishFrame)
	, m_LandingEffectManager(nullptr)
	, m_TargetMobileSuit(nullptr)
	, m_Velocity(0.0f,0.0f,0.0f)
	, m_TargetFlag(false)
	, m_RotationLimit(BulletData::k_MaxRotate)
{
	m_ActorType = Actor::ACTORTYPE_PLAYER_BULLET;
}

Bullet::~Bullet()
{
}

void Bullet::SetVelocity(const Conv_XM::Vector3f & direction)
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_RigidBodyComponentがnullptr");
		return;
	}

	m_Velocity = DirectX::XMVector3Normalize(direction);

	m_RigidBodyComponent->SetVelocity(m_Velocity * m_Speed);
}

void Bullet::SetActive(bool flag)
{
	// 入力
	m_Active = flag;

	if (m_Active == false)
	{
		m_FrameCount = 0;
		m_TargetFlag = false;
		m_TargetMobileSuit = nullptr;
	}
}

void Bullet::LoadActor()
{
	// リジッドボディコンポーネント作成
	m_RigidBodyComponent = CreateComponent<RigidBodyComponent>();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_RigidBodyComponentがnullptr");
		return;
	}

	// OBBコリジョンコンポーネント作成
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_OBBCollisionComponentがnullptr");
		return;
	}
	m_OBBCollisionComponent->SetOffsetScale(Conv_XM::Vector3f(0.5f, 0.5f, 0.5f));

	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* currentScene = sceneManager.GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor currentSceneがnullptr");
		return;
	}
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor sceneCollisionManagerがnullptr");
		return;
	}
	//　コリジョンのセット
	sceneCollisionManager->SetBulletCollision(m_OBBCollisionComponent);

	// OBBデバッグビュー作成
	CreateComponent<OBBDebugViewComponent>();
}

void Bullet::InitActor()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor nullptr");
		return;
	}

	m_RigidBodyComponent->SetGravity(0.0f);
	m_RigidBodyComponent->SetAirFriction(0.0f);
	m_RigidBodyComponent->SetIsGround(false);

	m_Emitter.Color = BulletData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = BulletData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = BulletData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = BulletData::k_DefaultLife;
	m_Emitter.Size = BulletData::k_ParticleSize;
	m_Emitter.Speed = 0.001f;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// エミッターを登録
	m_ParticleEmitterIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void Bullet::ProcessInputActor()
{
	// 追跡フラグが立っているなら追う
	if (m_TargetFlag == true)
	{
		TargetChase();
	}

	// パーティクルを生成させる
	m_Emitter.EmitterPosition = m_Position;
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.UpdateParticleEmitterConstantBuffer(m_ParticleEmitterIndex, m_Emitter);
	particleManager.SpawnParticle(m_UseParticleName, m_ParticleEmitterIndex);
}

void Bullet::UpdateActor()
{
	// フレームカウントが終了フレームを超えてたら
	if (m_FinishFrame < m_FrameCount)
	{
		// 非アクティブ状態にする
		SetActive(false);
	}
	//　カウントアップ
	m_FrameCount++;
}

void Bullet::CollisionDetected(class Actor* coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);

	// 非アクティブ状態にする
	SetActive(false);

	// 着弾エフェクトを発生させる
	if (m_LandingEffectManager == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::CollisionDetected m_LandingEffectManagerがnullptr");
		return;
	}
	m_LandingEffectManager->UseEffect(m_Position);
}

void Bullet::TargetChase()
{
	if (m_TargetFlag == false || m_TargetMobileSuit == nullptr)
	{
		return;
	}

	// ターゲットのポジション取得
	Conv_XM::Vector3f targetPos = m_TargetMobileSuit->GetPosition();

	// 自分からターゲットの単位ベクトル取得
	Conv_XM::Vector3f myPosToTargetVec = targetPos - m_Position;
	myPosToTargetVec = DirectX::XMVector3Normalize(myPosToTargetVec);

	// 速度の単位ベクトル取得
	Conv_XM::Vector3f velDir;
	velDir = DirectX::XMVector3Normalize(m_Velocity);

	//0ベクトルかどうかエラーチェック
	float error = Conv_XM::Vector3f::LengthSq(myPosToTargetVec);
	if (error < Math::k_Epsilon)
	{
		return;
	}

	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(velDir, myPosToTargetVec);

	if (dot < 0.0f)
	{
		m_TargetFlag = false;
	}
	else
	{
		Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(velDir, myPosToTargetVec);
		float angle = xmangle.x;

		Conv_XM::Vector4f NextVecQuaternion;

		// 同じ方角なら
		if (dot > 0.9999f)
		{
			NextVecQuaternion = DirectX::XMQuaternionIdentity();
		}
		else
		{
			if (fabsf(angle) > DirectX::XMConvertToRadians(m_RotationLimit))
			{
				angle = DirectX::XMConvertToRadians(m_RotationLimit);
			}
			//外積を求め回転軸を求める
			Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(velDir, myPosToTargetVec);

			// 次のベクトルとアクターのベクトルが同一方向なら
			if (Conv_XM::Vector3f::LengthSq(axis) < Math::k_Epsilon)
			{
				NextVecQuaternion = DirectX::XMQuaternionIdentity();
			}
			else
			{
				//クォータニオンを求める
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
			}
		}

		m_Velocity = DirectX::XMVector3Rotate(m_Velocity, NextVecQuaternion);

		m_Velocity = DirectX::XMVector3Normalize(m_Velocity);

		// リジッドボディに速度を入力する
		m_RigidBodyComponent->SetVelocity(m_Velocity * m_Speed);
	}
}
