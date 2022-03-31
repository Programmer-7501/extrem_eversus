
#include"../../../system/main.h"
#include "bazooka_landing_effect.h"
#include"../../../particle/particle_manager.h"

namespace BazookaLandingEffectData
{
	// �I���t���[��
	static constexpr int k_DefaultFinishFrame = 4;

	//! �F
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.8f, 1.0f);
	//! 1�t���[�����ɔ���������p�[�e�B�N����
	static constexpr int k_DefaultSpawnNum = 3;
	//! ���C�t
	static constexpr float k_DefaultLife = 15.0f;
	//! �p�[�e�B�N���T�C�Y
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(0.7f, 0.7f);
	//! �X�s�[�h
	static constexpr float k_DefaultSpeed = 0.2f;
	//! �g�p�G�~�b�^�[�^�C�v
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_SPHERE;
}

BazookaLandingEffect::BazookaLandingEffect()
{
}

BazookaLandingEffect::~BazookaLandingEffect()
{
}

void BazookaLandingEffect::SetActive(bool flag)
{
	// ����
	m_Active = flag;

	if (m_Active == true)
	{
		ParticleManager& particleManager = ParticleManager::GetInstance();
		m_Emitter.EmitterPosition = m_Position;
		particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter.MyIndex, m_Emitter);

		m_FrameCount = 0;
	}
}

void BazookaLandingEffect::InitActor()
{
	// �G�t�F�N�g���[�h�ݒ�
	m_EffectMode = EFFECTMODE_ONCE;

	// �t�B�j�b�V���t���[���J�E���g��ݒ�
	SetFinishFrame(BazookaLandingEffectData::k_DefaultFinishFrame);

	// �G�~�b�^�[�ݒ�
	m_Emitter.Color = BazookaLandingEffectData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = BazookaLandingEffectData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = BazookaLandingEffectData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = BazookaLandingEffectData::k_DefaultLife;
	m_Emitter.Size = BazookaLandingEffectData::k_ParticleSize;
	m_Emitter.Speed = BazookaLandingEffectData::k_DefaultSpeed;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �G�~�b�^�[��o�^
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void BazookaLandingEffect::ProcessInputActor()
{
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter.MyIndex);
}

void BazookaLandingEffect::UpdateActor()
{
	// �G�t�F�N�g�X�V
	Effect::UpdateActor();
}
