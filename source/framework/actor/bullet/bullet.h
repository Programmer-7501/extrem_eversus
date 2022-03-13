/**
 * @file bullet.h
 * @brief �o���b�g
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief �o���b�g�N���X
 */
class Bullet : public Actor
{
protected:
	//! ���W�b�h�{�f�B�R���|�[�l���g
	class RigidBodyComponent* m_RigidBodyComponent;

	//! OBB�R���W�����R���|�[�l���g
	class OBBCollisionComponent* m_OBBCollisionComponent;

	//! ���x
	float m_Speed;

	//! �g�p�p�[�e�B�N���l�[��
	std::string m_UseParticleName;

	//! �p�[�e�B�N���G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_Emitter;

	//! �p�[�e�B�N���G�~�b�^�[�C���f�b�N�X
	int m_ParticleEmitterIndex;

	//! �t���[���J�E���g
	int m_FrameCount;

	//! �I���t���[��
	int m_FinishFrame;

	//! �G�t�F�N�g�}�l�[�W���[
	class EffectManager* m_LandingEffectManager;

	//! �_���Ă���G
	class MobileSuit* m_TargetMobileSuit;

	//! ���x
	Conv_XM::Vector3f m_Velocity;

	//! �ǔ��t���O
	bool m_TargetFlag;

	float m_RotationLimit;
public:
	Bullet();
	~Bullet();


	/**
	* @brief ���x�̃Z�b�g
	* @param[in] const Conv_XM::Vector3f&(velocity) ���x
	*/
	virtual void SetVelocity(const Conv_XM::Vector3f& velocity);

	/**
	* @brief �I���t���[���̃Z�b�g
	* @param[in] int(finishFrame) �I���t���[��
	*/
	inline void SetFinishFrame(int finishFrame) { m_FinishFrame = finishFrame; }

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

	inline void SetTargetMobileSuit(class MobileSuit* mobileSuit) 
	{
		if (mobileSuit == nullptr)
		{

		}
		else
		{
			m_TargetFlag = true;
			m_TargetMobileSuit = mobileSuit;
		}
	}
protected:

	/**
	* @brief �o���b�g�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �o���b�g�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �o���b�g�Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief v�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

	/**
	* @brief �����蔻�菈��
	* @details �Փˎ��ɌĂяo�����֐�
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;

	/**
	* @brief �^�[�Q�b�g��ǐՂ���
	*/
	void TargetChase();
};