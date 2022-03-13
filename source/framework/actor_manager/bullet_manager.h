/**
 * @file bullet_manager.h
 * @brief �o���b�g�̊Ǘ����s��
 */

#pragma once

#include"actor_manager.h"
#include"../actor/bullet/bullet.h"

 /**
 * @brief �o���b�g�̊Ǘ�������N���X
 */
class BulletManager : public ActorManager
{
	enum ReloadMethod
	{
		//! �����
		RM_ONEBYONE,
		//! �g���؂�����
		RM_ALLUSE,
		//! �����[�h���Ȃ�
		RM_NONE_RELOAD,
		RM_MAX,
	};
private:
	//! �o���b�g�A�N�^�[�Y
	std::vector<Bullet*> m_Bullets;

	//! �e�ő吔
	int m_MaxBullet;

	//! �c�e��
	int m_CurrentBullet;

	//! �����[�h���@
	ReloadMethod m_ReloadMethod;

	//! �����[�h�J�E���g
	int m_ReloadCount;

	//! �����[�h����
	int m_ReloadTime;
public:
	BulletManager();
	~BulletManager();

	/**
	* @brief �e�̎g�p
	* @param[in] class MobileSuit*(target) �_������
	* @param[in] const Conv_XM::Vector3f&(position) �e�̔����|�W�V����
	* @param[in] const Conv_XM::Vector3f&(velocity) �e�̑��x
	* @return class Bullet* �g�p����o���b�g
	*/
	class Bullet* UseBullet(class MobileSuit* target, const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& velocity);

	/**
	* @brief �A�N�^�[�����֐�
	* @param[in] int(maxInstanceNum) �Ǘ�����e��
	* @param[in] int(maxBulletNum) �ő�̒e��
	*/
	template<typename T>
	void CreateBulletActors(int maxInstanceNum, int maxBulletNum)
	{
		m_MaxBullet = maxBulletNum;
		m_CurrentBullet = maxBulletNum;
		m_Bullets.reserve(maxInstanceNum);

		for (int i = 0; i < maxInstanceNum; i++)
		{
			// �A�N�^�[�𐶐�
			T* bullet = new T;
			// null�`�F�b�N
			if (bullet == nullptr)
			{
				Logger::GetInstance().SetLog("BulletManager::CreateBulletActors bullet��nullptr");
				return;
			}
			//�A�N�^�[��ɓo�^
			m_Bullets.emplace_back(bullet);
		}
	}


	/**
	* @brief �����[�h����
	*/
	void Reload();

	/**
	* @brief ������
	*/
	void Reset();


	//�Z�b�^�[�A�Q�b�^�[
	inline void SetReloadMethod(ReloadMethod reloadMethod, int reloadTime = 0)
	{
		m_ReloadMethod = reloadMethod;
		m_ReloadTime = reloadTime;
	}

	inline int GetMaxBullet() const { return m_MaxBullet; }
	inline int GetCurrentBullet() const { return m_CurrentBullet; }

	void SetLandingEffectManager(class EffectManager* effectManager);
private:

	/**
	* @brief �ǂݍ��ݏ���
	*/
	void LoadData() override;

	/**
	* @brief ����������
	*/
	void Init() override;

	/**
	* @brief �I������
	*/
	void Uninit() override;

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput()override;

	/**
	* @brief �X�V����
	*/
	void Update() override;

};