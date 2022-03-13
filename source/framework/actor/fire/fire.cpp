
#include"../../../system/main.h"
#include "fire.h"
#include"../../../particle/particle_manager.h"

namespace FireData
{
	//! デフォルトの速度
	static constexpr float k_DefaultSpeed = 0.08f;
	//! 弾の色
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.0f, 1.0f);
	//! 1フレーム中に発生させるパーティクル数
	static constexpr int k_DefaultSpawnNum = 3;
	//! ライフ
	static constexpr float k_DefaultLife = 40.0f;
	//! パーティクルサイズ
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(2.0f, 2.0f);
	//! ポジションのばらつき
	static const Conv_XM::Vector3f k_PositionVariate = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
	//! 使用エミッタータイプ
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
	// エミッターを登録
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void Fire::ProcessInputActor()
{
	//　パーティクルを発生させる
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter.MyIndex);
}
