/**
 * @file beam_saber.h
 * @brief �r�[���T�[�x��
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

namespace BeamSaberData
{

	//! �G�~�b�^�[��
	static constexpr int k_MainEmitterNum = 40;
}
 /**
 * @brief �r�[���T�[�x���N���X
 */
class BeamSaber : public Actor
{
public:
	//! �r�[���T�[�x�������f��
	static class Model* m_BeamSaber_Handle_Model;

	static void LoadModel();
	static void UnloadModel();

public:
	enum BeamSaberState
	{
		STATE_NONE,
		STATE_NKAKUTOU001,
		STATE_NKAKUTOU002,
		STATE_NKAKUTOU003,
	};

protected:
	//! OBB�R���W�����R���|�[�l���g
	class OBBCollisionComponent* m_OBBCollisionComponent;

	//! �r�[���T�[�x�������f��
	class ModelComponent* m_ModelComponent;

	//! �g�p�p�[�e�B�N���l�[��
	std::string m_UseParticleName;

	//! �p�[�e�B�N���G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_Emitter[BeamSaberData::k_MainEmitterNum * 2];

	//! �p�[�e�B�N���G�~�b�^�[�C���f�b�N�X
	int m_ParticleEmitterIndex;

	//! �G�t�F�N�g�}�l�[�W���[
	class EffectManager* m_LandingEffectManager;

	//! �J���[
	Conv_XM::Vector4f m_Color;

	//! ���L���r���X�[�c
	class MobileSuit* m_OwnerMobileSuit;

	//! ���
	BeamSaberState m_BeamSaberState;

	//! �r�[���T�[�x���A�j���[�V�����J�E���g
	int m_BeamSaberAnimationCount;

	//! �r�[���T�[�x���G�t�F�N�g����
	float m_BeamSaberMainEffectTime;
	float m_BeamSaberSubEffectTime;
public:
	BeamSaber();
	~BeamSaber();

	/**
	* @brief �g�p�p�[�e�B�N���̖��O�̃Z�b�g
	* @param[in] const std::string&(particleName) �g�p�p�[�e�B�N���̖��O
	*/
	void SetUseParticleName(const std::string& particleName) { m_UseParticleName = particleName; }

	/**
	* @brief �A�N�e�B�u����
	*/
	virtual void SetActive(bool flag)override;

	inline void SetLandingEffectManager(class EffectManager* landingEffectManager) { m_LandingEffectManager = landingEffectManager; }

	inline void SetColor(const Conv_XM::Vector4f color) { m_Color = color; }

	inline void SetOwnerMobileSuit(class MobileSuit* ownerMobileSuit) { m_OwnerMobileSuit = ownerMobileSuit; }

	//! ���Z�b�g
	void Reset();

	//! N�i��1
	void StartNKakutou001();

	//! N�i��2
	void StartNKakutou002();

	//! N�i��3
	void StartNKakutou003();

	void SetParticleTime(float mainTime, float effectTime);

protected:

	/**
	* @brief �r�[���T�[�x���Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �r�[���T�[�x���Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �r�[���T�[�x���Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;
};