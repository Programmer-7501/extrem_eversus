
#include"../../../system/main.h"
#include "mobilesuit_explosion_effect.h"
#include"../../../particle/particle_manager.h"

namespace MobileSuitExplosionEffectData
{
	// �I���t���[��
	static constexpr int k_DefaultFinishFrame = 2;

	//! �F
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.5f, 0.0f, 1.0f);
	//! 1�t���[�����ɔ���������p�[�e�B�N����
	static constexpr int k_DefaultSpawnNum = 20;
	//! ���C�t
	static constexpr float k_DefaultLife = 35.0f;
	//! �p�[�e�B�N���T�C�Y
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(3.0f, 3.0f);
	//! �g�p�G�~�b�^�[�^�C�v
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_SPHERE;
}

MobileSuitExplosionEffect::MobileSuitExplosionEffect()
{
}

MobileSuitExplosionEffect::~MobileSuitExplosionEffect()
{
}

void MobileSuitExplosionEffect::SetActive(bool flag)
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

void MobileSuitExplosionEffect::InitActor()
{
	// �G�t�F�N�g���[�h�ݒ�
	m_EffectMode = EFFECTMODE_ONCE;

	// �t�B�j�b�V���t���[���J�E���g��ݒ�
	SetFinishFrame(MobileSuitExplosionEffectData::k_DefaultFinishFrame);

	// �G�~�b�^�[�ݒ�
	m_Emitter.Color = MobileSuitExplosionEffectData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = MobileSuitExplosionEffectData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = MobileSuitExplosionEffectData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = MobileSuitExplosionEffectData::k_DefaultLife;
	m_Emitter.Size = MobileSuitExplosionEffectData::k_ParticleSize;
	m_Emitter.Speed = 0.2f;
	m_Emitter.Gravity = 0.01f;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �G�~�b�^�[��o�^
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void MobileSuitExplosionEffect::ProcessInputActor()
{
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter.MyIndex);
}

void MobileSuitExplosionEffect::UpdateActor()
{
	// �G�t�F�N�g�X�V
	Effect::UpdateActor();
}
