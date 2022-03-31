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

	//! �t���[���J�E���g
	int m_FrameCount;

	//! �I���t���[��
	int m_FinishFrame;

	//! �_���Ă���G
	class MobileSuit* m_TargetMobileSuit;

	//! ����
	Conv_XM::Vector3f m_Direction;

	//! �ǔ��t���O
	bool m_TargetFlag;

	//! ��]���
	float m_RotationLimit;

	//! �G�t�F�N�g�}�l�[�W���[
	class EffectManager* m_LandingEffectManager;

public:
	Bullet();
	~Bullet();


	/**
	* @brief ���x�̃Z�b�g
	* @param[in] const Conv_XM::Vector3f&(direction) ����
	*/
	virtual void SetDirection(const Conv_XM::Vector3f& direction);

	/**
	* @brief �I���t���[���̃Z�b�g
	* @param[in] int(finishFrame) �I���t���[��
	*/
	inline void SetFinishFrame(int finishFrame) { m_FinishFrame = finishFrame; }

	/**
	* @brief �A�N�e�B�u����
	*/
	virtual void SetActive(bool flag)override;

	/**
	* @brief �^�[�Q�b�g���r���X�[�c�̐ݒ�
	* @param[in] class MobileSuit*(mobileSuit) �^�[�Q�b�g�̃��r���X�[�c
	*/
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


	/**
	* @brief ���e�G�t�F�N�g�ݒ�
	*/
	inline void SetLandingEffectManager(class EffectManager* landingEffectManager) { m_LandingEffectManager = landingEffectManager; }
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