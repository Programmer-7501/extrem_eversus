
#include"../../../system/main.h"
#include "beam_rifle.h"
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


BeamRifle::BeamRifle()
	: m_UseParticleName(ParticleManagerData::k_BeamParticleName)
	, m_Emitter{}
{
}

BeamRifle::~BeamRifle()
{
}

void BeamRifle::LoadActor()
{
	// バレットの読み込み処理
	Bullet::LoadActor();
}

void BeamRifle::InitActor()
{
	// バレットの初期化処理
	Bullet::InitActor();

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

void BeamRifle::ProcessInputActor()
{
	// バレットの入力処理
	Bullet::ProcessInputActor();

	// パーティクルを生成させる
	m_Emitter.EmitterPosition = m_Position;
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter.MyIndex, m_Emitter);
	particleManager.SpawnParticle(m_UseParticleName, m_Emitter.MyIndex);
}

void BeamRifle::UpdateActor()
{
	// バレットの更新処理
	Bullet::UpdateActor();
}

void BeamRifle::CollisionDetected(Actor * coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);

	// 非アクティブ状態にする
	SetActive(false);

	// 着弾エフェクトを発生させる
	if (m_LandingEffectManager == nullptr)
	{
		Logger::GetInstance().SetLog("BeamRifle::CollisionDetected m_LandingEffectManagerがnullptr");
		return;
	}
	m_LandingEffectManager->UseEffect(m_Position);
}
