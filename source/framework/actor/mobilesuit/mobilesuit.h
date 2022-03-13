/**
 * @file mobilesuit_base.h
 * @brief ���r���X�[�c�̃x�[�X
 */

#pragma once

#include"../actor.h"

 /**
 * @brief ���r���X�[�c�̃x�[�X�N���X
 */
class MobileSuit : public Actor
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
protected:
	//! ���W�b�h�{�f�B�R���|�[�l���g
	class RigidBodyComponent* m_RigidBodyComponent;

	//! �u�[�X�g�R���|�[�l���g
	class BoostComponent* m_BoostComponent;

	//! ���C�t�R���|�[�l���g
	class LifeComponent* m_LifeComponent;

	//! �o���R���|�[�l���g
	class ArousalComponent* m_ArousalComponent;

	//! �����蔻��R���W����
	class OBBCollisionComponent* m_OBBCollisionComponent;

	//! �I�u�W�F�N�g�Ƃ̓����蔻��R���W����
	class AABBCollisionComponent* m_AABBCollisionComponent;

	//! �u�[�X�g�_�b�V�������ۂ�
	bool m_IsBoostDash;

	//! �r�[�����C�t���o���b�g�}�l�[�W���[
	class BulletManager* m_BeamRifleBulletManager;

	//! �L���m���}�l�[�W���[
	class BulletManager* m_CannonManager;

	//! ����
	class MobileSuit* m_AlliesMobileSuit;

	//! 1�ڂ̃G�l�~�[
	class MobileSuit* m_EnemyMobileSuit001;

	//! 2�ڂ̃G�l�~�[
	class MobileSuit* m_EnemyMobileSuit002;

	//! �^�[�Q�b�g�ԍ�
	int m_TargetNum;

	//! �r�[�����C�t���V���b�g�J�E���g�t���[��
	int m_BeamRifleCountFrame;
	//! �r�[�����C�t���������Ă�����
	bool m_IsBeamRifleShot;
	//! �r�[�����C�t�������������ǂ���
	bool m_IsFinishBeamRifleShot;
	//! �_������
	Conv_XM::Vector3f m_BeamRifleDirection;
	//! �r�[�����C�t���N�[���^�C��
	int m_BeamRifleCoolTime;

	//! �����G�t�F�N�g
	class EffectManager* m_MobileSuitEffectManager;

	//! ���b�N�I�����Ă��邩�ǂ���
	bool m_IsLockOn;

	//! �_�E�����Ă��邩�ǂ���
	bool m_DownFlag;
	//! �_�E���l
	float m_DownNum;
	//! �_�E�����A�J�E���g
	float m_DownCount;

	//! �_���[�W���󂯂����ǂ���
	bool m_IsDamage;

	//! �_�E���l�񕜃J�E���g
	int m_DownNumCoolTime;

	//! �r�[���T�[�x��
	class BeamSaber* m_BeamSaber;

	//! �i���p����
	ActorType m_ColedType;

	// �R�X�g�}�l�[�W���[�N���X
	class CostManager* m_CostManager;

	//! �R�X�g
	int m_MyCost;
public:
	MobileSuit();
	~MobileSuit();

	// �p�����[�^�[�����Z�b�g����
	virtual void Reset();

	/**
	* @brief �r�[�����C�t���V���b�g
	*/
	virtual void BeamRifleShot();

	/**
	* @brief �r�[�����C�t���̃N�[���^�C���폜
	*/
	virtual void ResetBeamRifleCoolTime();

	// �Z�b�^�[,�Q�b�^�[
	inline void SetIsBoostDash(bool flag) { m_IsBoostDash = flag; }
	inline bool GetIsBoostDash() const { return m_IsBoostDash; }

	inline class BulletManager* GetBeamRifleBulletManager() const { return m_BeamRifleBulletManager; }
	inline void SetBeamRifleBulletManager(class BulletManager* beamRifleBulletManager) { m_BeamRifleBulletManager = beamRifleBulletManager; }

	inline class BulletManager* GetCannonBulletManager() const { return m_CannonManager; }
	inline void SetCannonBulletManager(class BulletManager* cannonBulletManager) { m_CannonManager = cannonBulletManager; }

	inline void SetAllies(class MobileSuit* allies001) { m_AlliesMobileSuit = allies001; }
	inline class MobileSuit* GetAlliesMobileSuit() const { return m_AlliesMobileSuit; }

	inline void SetEnemy(class MobileSuit* enemy001, class MobileSuit* enemy002)
	{
		m_EnemyMobileSuit001 = enemy001;
		m_EnemyMobileSuit002 = enemy002;
	}

	inline class MobileSuit* GetEnemyMobileSuit001() const { return m_EnemyMobileSuit001; }
	inline class MobileSuit* GetEnemyMobileSuit002() const { return m_EnemyMobileSuit002; }

	inline int GetTargetNumber() const { return m_TargetNum; }
	inline void SetTargetNumber(int targetNum) { m_TargetNum = targetNum; }

	inline bool GetIsBeamRifleShot() const { return m_IsBeamRifleShot; }

	inline class LifeComponent* GetLifeComponent() const { return m_LifeComponent; }

	inline void SetMobileSuitExplosionEffectManager(class EffectManager* mobileSuitExplosionEffectManager) { m_MobileSuitEffectManager = mobileSuitExplosionEffectManager; }

	inline bool GetIsLockOn() const { return m_IsLockOn; }

	inline bool GetDownFlag() const { return m_DownFlag; }

	inline void SetIsDamageFlag(bool flag) { m_IsDamage = flag; }
	inline bool GetIsDamageFlag() { return m_IsDamage; }

	inline void SetBeamSaber(class BeamSaber* beamSaber) { m_BeamSaber = beamSaber; }
	class BeamSaber* GetBeamSaber() { return m_BeamSaber; }

	virtual void SetIsMoveFlag(bool flag);

	inline bool GetIsBeamRifleShot() { return m_IsBeamRifleShot; }
protected:

	/**
	* @brief ���r���X�[�c�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ���r���X�[�c�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief v�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;
};