/**
 * @file enemy.h
 * @brief �G�l�~�[�N���X
 */

#pragma once

#include"mobilesuit.h"
#include"../../../directx/render_data/cbuffer_data.h"

#include"../../../ai/astar.h"

 /**
 * @brief  �G�l�~�[�N���X
 */
class Enemy : public MobileSuit
{
public:
	enum EnemyNumber
	{
		ENEMY001,
		ENEMY002,
		ENEMYMAX,
	};
public:
	static class AnimationModel* m_AnimationModel;

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

	// �o�H�T���p�O���t
	WeightedGraph m_WeightedGraph;

	//! �G�l�~�[�ԍ�
	EnemyNumber m_EnemyNumber;
public:
	Enemy();
	~Enemy();


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

	inline void SetWeightedGraph(WeightedGraph weightedGraph) { m_WeightedGraph = weightedGraph; }

	inline void SetEnemyNumber(EnemyNumber enemyNumber) { m_EnemyNumber = enemyNumber; }
private:

	/**
	* @brief �G�l�~�[�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �G�l�~�[�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �G�l�~�[�Ǝ��̓��͏���
	*/
	void ProcessInputActor() override;

};