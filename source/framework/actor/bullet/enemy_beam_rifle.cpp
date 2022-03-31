
#include"../../../system/main.h"
#include "enemy_beam_rifle.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../../particle/particle_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../actor_manager/effect_manager.h"
#include"bullet_data.h"

EnemyBeamRifle::EnemyBeamRifle()
	: m_UseParticleName(ParticleManagerData::k_BeamParticleName)
{
	m_ActorType = Actor::ACTORTYPE_ENEMY_BULLET;
}

EnemyBeamRifle::~EnemyBeamRifle()
{
}

void EnemyBeamRifle::LoadActor()
{
	// バレットの読み込み処理
	EnemyBullet::LoadActor();
}

void EnemyBeamRifle::InitActor()
{
	// バレットの初期化処理
	EnemyBullet::InitActor();

	m_Emitter.Color = BeamRifleData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = BeamRifleData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = BeamRifleData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = BeamRifleData::k_DefaultLife;
	m_Emitter.Size = BeamRifleData::k_ParticleSize;
	m_Emitter.Speed = 0.001f;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// エミッターを登録
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void EnemyBeamRifle::ProcessInputActor()
{
	// バレットの入力処理
	EnemyBullet::ProcessInputActor();

	// パーティクルを生成させる
	m_Emitter.EmitterPosition = m_Position;
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter.MyIndex, m_Emitter);
	particleManager.SpawnParticle(m_UseParticleName, m_Emitter.MyIndex);
}

void EnemyBeamRifle::UpdateActor()
{
	// バレットの更新処理
	EnemyBullet::UpdateActor();
}

void EnemyBeamRifle::CollisionDetected(Actor * coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);

	// 非アクティブ状態にする
	SetActive(false);

	// 着弾エフェクトを発生させる
	if (m_LandingEffectManager == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBeamRifle::CollisionDetected m_LandingEffectManagerがnullptr");
		return;
	}
	m_LandingEffectManager->UseEffect(m_Position);
}
