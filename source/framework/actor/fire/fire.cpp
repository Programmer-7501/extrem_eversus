
#include"../../../system/main.h"
#include "fire.h"
#include"../../../particle/particle_manager.h"

namespace FireData
{
	//! �f�t�H���g�̑��x
	static constexpr float k_DefaultSpeed = 0.08f;
	//! �e�̐F
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.0f, 1.0f);
	//! 1�t���[�����ɔ���������p�[�e�B�N����
	static constexpr int k_DefaultSpawnNum = 3;
	//! ���C�t
	static constexpr float k_DefaultLife = 40.0f;
	//! �p�[�e�B�N���T�C�Y
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(2.0f, 2.0f);
	//! �|�W�V�����̂΂��
	static const Conv_XM::Vector3f k_PositionVariate = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
	//! �g�p�G�~�b�^�[�^�C�v
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_RECT;
}

Fire::Fire()
	: m_Emitter{}
{
}

void Fire::LoadActor()
{
}

void Fire::InitActor()
{
	m_Emitter.Color = FireData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = FireData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = FireData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = FireData::k_DefaultLife;
	m_Emitter.Size = FireData::k_ParticleSize;
	m_Emitter.Speed = FireData::k_DefaultSpeed;
	m_Emitter.PositionVariance = FireData::k_PositionVariate;
	m_Emitter.ParticleVelocity = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
	m_Emitter.VelocityVariance = Conv_XM::Vector3f(0.01f, 0.0f, 0.01f);
	m_Emitter.SizeOverLifeTime = 1;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �G�~�b�^�[��o�^
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void Fire::ProcessInputActor()
{
	//�@�p�[�e�B�N���𔭐�������
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter.MyIndex);
}
