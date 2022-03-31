
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
	// �o���b�g�̓ǂݍ��ݏ���
	EnemyBullet::LoadActor();
}

void EnemyBeamRifle::InitActor()
{
	// �o���b�g�̏���������
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
	// �G�~�b�^�[��o�^
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void EnemyBeamRifle::ProcessInputActor()
{
	// �o���b�g�̓��͏���
	EnemyBullet::ProcessInputActor();

	// �p�[�e�B�N���𐶐�������
	m_Emitter.EmitterPosition = m_Position;
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter.MyIndex, m_Emitter);
	particleManager.SpawnParticle(m_UseParticleName, m_Emitter.MyIndex);
}

void EnemyBeamRifle::UpdateActor()
{
	// �o���b�g�̍X�V����
	EnemyBullet::UpdateActor();
}

void EnemyBeamRifle::CollisionDetected(Actor * coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);

	// ��A�N�e�B�u��Ԃɂ���
	SetActive(false);

	// ���e�G�t�F�N�g�𔭐�������
	if (m_LandingEffectManager == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBeamRifle::CollisionDetected m_LandingEffectManager��nullptr");
		return;
	}
	m_LandingEffectManager->UseEffect(m_Position);
}
