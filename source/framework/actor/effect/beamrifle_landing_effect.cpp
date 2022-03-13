#include"../../../system/main.h"
#include "beamrifle_landing_effect.h"
#include"../../../particle/particle_manager.h"

namespace BeanRifleLandingEffectData
{
	// 終了フレーム
	static constexpr int k_DefaultFinishFrame = 4;

	//! 色
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.8f, 1.0f);
	//! 1フレーム中に発生させるパーティクル数
	static constexpr int k_DefaultSpawnNum = 3;
	//! ライフ
	static constexpr float k_DefaultLife = 15.0f;
	//! パーティクルサイズ
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(0.7f, 0.7f);
	//! スピード
	static constexpr float k_DefaultSpeed = 0.2f;
	//! 使用エミッタータイプ
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_SPHERE;
}

BeanRifleLandingEffect::BeanRifleLandingEffect()
{
}

BeanRifleLandingEffect::~BeanRifleLandingEffect()
{
}

void BeanRifleLandingEffect::SetActive(bool flag)
{
	// 入力
	m_Active = flag;

	if (m_Active == true)
	{
		ParticleManager& particleManager = ParticleManager::GetInstance();
		m_Emitter.EmitterPosition = m_Position;
		particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter.MyIndex, m_Emitter);

		m_FrameCount = 0;
	}
}

void BeanRifleLandingEffect::InitActor()
{
	// エフェクトモード設定
	m_EffectMode = EFFECTMODE_ONCE;

	// フィニッシュフレームカウントを設定
	SetFinishFrame(BeanRifleLandingEffectData::k_DefaultFinishFrame);

	// エミッター設定
	m_Emitter.Color = BeanRifleLandingEffectData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = BeanRifleLandingEffectData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = BeanRifleLandingEffectData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = BeanRifleLandingEffectData::k_DefaultLife;
	m_Emitter.Size = BeanRifleLandingEffectData::k_ParticleSize;
	m_Emitter.Speed = BeanRifleLandingEffectData::k_DefaultSpeed;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// エミッターを登録
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void BeanRifleLandingEffect::ProcessInputActor()
{
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter.MyIndex);
}

void BeanRifleLandingEffect::UpdateActor()
{
	// エフェクト更新
	Effect::UpdateActor();
}
