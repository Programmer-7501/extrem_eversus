
#include"../../../system/main.h"
#include "cannon.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../../particle/particle_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"

namespace CannonData
{
	//! デフォルトの速度
	static constexpr float k_DefaultSpeed = 1.0f;

	//! 弾の色
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.8f, 0.2f, 1.0f);
	//! 1フレーム中に発生させるパーティクル数
	static constexpr int k_DefaultSpawnNum = 10;
	//! ライフ
	static constexpr float k_DefaultLife = 250.0f;
	//! パーティクルサイズ
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(5.0f, 5.0f);
	//! ポジションのばらつき
	static const Conv_XM::Vector3f k_PositionVariate = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
	//! 使用エミッタータイプ
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_RECT;
}

Cannon::Cannon()
	:m_UseParticleName(ParticleManagerData::k_BeamParticleName)
{
	m_ActorType = Actor::ACTORTYPE_CANNON;
}

Cannon::~Cannon()
{
}

void Cannon::SetActive(bool flag)
{
	// 入力
	m_Active = flag;
	// カウント初期化
	m_FrameCount = 0;

	// 非アクティブ処理
	if (m_Active == false)
	{
		return;
	}

	// ポジション入力
	m_Emitter.EmitterPosition = m_Position;
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter.MyIndex, m_Emitter);
	particleManager.SpawnParticle(m_UseParticleName, m_Emitter.MyIndex);
}

void Cannon::SetDirection(const Conv_XM::Vector3f & direction)
{
	m_Emitter.ParticleVelocity = direction;
	m_Direction = direction;
	m_Direction = DirectX::XMVector3Normalize(m_Direction);

	Conv_XM::Vector4f q = Conv_XM::Vector4f::MakeQuaternionFromVec3(m_Direction);
	SetQuaternion(q);
	ComputeWorldTransform();
}

void Cannon::LoadActor()
{
	// OBBコリジョンコンポーネント作成
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_OBBCollisionComponentがnullptr");
		return;
	}

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

void Cannon::InitActor()
{
	m_Emitter.Color = CannonData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = CannonData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = CannonData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = CannonData::k_DefaultLife;
	m_Emitter.Size = CannonData::k_ParticleSize;
	m_Emitter.Speed = CannonData::k_DefaultSpeed;
	m_Emitter.PositionVariance = CannonData::k_PositionVariate;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// エミッターを登録
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void Cannon::ProcessInputActor()
{
	//　パーティクルを発生させる
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.SpawnParticle(m_UseParticleName, m_Emitter.MyIndex);
}

void Cannon::UpdateActor()
{
	// フレームカウントが終了フレームを超えてたら
	if (m_FrameCount > m_FinishFrame)
	{
		// 非アクティブ状態にする
		SetActive(false);
	}

	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Cannon::UpdateActor nullptr");
		return;
	}
	Collision::OBBCollision obb = m_OBBCollisionComponent->GetOBB();

	Conv_XM::Vector3f offsetPosition = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldForward, m_Quaternion);

	// OBBの中心を求める
	Conv_XM::Vector3f center = m_Position + ((offsetPosition * static_cast<float>(m_FrameCount)) / 2.0f);
	obb.Position = center;
	m_OBBCollisionComponent->SetOffsetTransform(Conv_XM::Vector3f(0.0f,0.0f, static_cast<float>(m_FrameCount) / 2.0f), Conv_XM::Vector3f(3.0f,3.0f, static_cast<float>(m_FrameCount)), DirectX::XMQuaternionIdentity());
	// 座標更新をする
	m_OBBCollisionComponent->OnUpdateWorldTransform();

	// カウントアップ
	m_FrameCount++;
}

void Cannon::CollisionDetected(class Actor* coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);
}
