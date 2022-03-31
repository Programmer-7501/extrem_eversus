/**
 * @file player.h
 * @brief �v���C���[�N���X
 */

#pragma once

#include"mobilesuit.h"
#include"../../../directx/render_data/cbuffer_data.h"

namespace PlayerData
{
	//! �r�[�����C�t���̍ő�C���X�^���X��
	static constexpr int k_MaxBeamRifleInstanceNum = 20;

	//! �r�[�����C�t���̍ő吔
	static constexpr int k_MaxBeamRifleNum = 10;

	//! �L���m���̍ő�C���X�^���X��
	static constexpr int k_MaxCannonInstanceNum = 2;

	//! �L���m���̍ő吔
	static constexpr int k_MaxCannonNum = 1;
}

 /**
 * @brief �v���C���[�N���X
 */
class Player : public MobileSuit
{
public:
	/**
	* @brief �T�E���h�ǂݍ���
	*/
	static void LoadSound();

	/**
	* @brief �T�E���h�j��
	*/
	static void UnloadSound();

	/**
	* @brief ���f���ǂݍ���
	*/
	static void LoadModel();

	/**
	* @brief ���f���j��
	*/
	static void UnloadModel();
private:
	//! ���b�N�I���J�����R���|�[�l���g
	class LockOnCameraComponent* m_LockOnCameraComponent;

	//! ���b�N�I���J�[�\���R���|�[�l���g
	class LockonCursolBillboardComponent* m_LockonCursolBillboardComponent;

	//! �A�j���[�V�������f��
	class AnimationModelComponent* m_AnimationModelComponent;

	//! �u�[�X�g�_�b�V�����̃I�t�Z�b�g���[�e�[�V����
	Conv_XM::Vector4f m_BoostDashOffsetRotation;

	//! �ʏ펞�̃I�t�Z�b�g���[�e�[�V����
	Conv_XM::Vector4f m_OffsetRotation;

	//! �N�H�[�^�j�I�������p�J�E���g
	float m_QuaternionCount;

	//! �o�[�i�[�G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_BurnerEmitter001;
	CBufferData::ParticleEmitter m_BurnerEmitter002;

	//! ��t���[���O�̃|�W�V�����ۑ�
	Conv_XM::Vector3f m_StorePosition;

public:
	Player();
	~Player();

	/**
	* @brief �r�[�����C�t���V���b�g
	*/
	virtual void BeamRifleShot() override;

	/**
	* @brief �r�[�����C�t���̃N�[���^�C���폜
	*/
	virtual void ResetBeamRifleCoolTime() override;

	/**
	* @brief �����蔻�菈��
	* @details �Փˎ��ɌĂяo�����֐�
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;

	void SetActive(bool flag)override;

	inline void SetCostManager(class CostManager* costManager) { m_CostManager = costManager; }

	void SetIsMoveFlag(bool flag) override;
private:

	/**
	* @brief �v���C���[�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �v���C���[�Ǝ��̔j������
	*/
	void UninitActor()override;

	/**
	* @brief �v���C���[�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �v���C���[�Ǝ��̓��͏���
	*/
	void ProcessInputActor() override;
};