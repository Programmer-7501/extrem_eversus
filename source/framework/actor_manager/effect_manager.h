/**
 * @file effect_manager.h
 * @brief �G�t�F�N�g�̊Ǘ����s��
 */

#pragma once

#include"actor_manager.h"
#include"../actor/effect/effect.h"

 /**
 * @brief �G�t�F�N�g�̊Ǘ�������N���X
 */
class EffectManager : public ActorManager
{
private:
	//! �o���b�g�A�N�^�[�Y
	std::vector<Effect*> m_Effects;

public:
	EffectManager();
	~EffectManager();

	/**
	* @brief �G�t�F�N�g�̎g�p
	* @param[in] const Conv_XM::Vector3f&(position) �G�t�F�N�g�̔����|�W�V����
	* @return class Bullet* �g�p����G�t�F�N�g
	*/
	class Effect* UseEffect(const Conv_XM::Vector3f& position);

	/**
	* @brief �A�N�^�[�����֐�
	* @param[in] int(maxInstanceNum) �Ǘ�����G�t�F�N�g��
	* @param[in] int(maxEffectNum) �ő�̃G�t�F�N�g��
	*/
	template<typename T>
	void CreateEffectActors(int maxInstanceNum)
	{
		m_Effects.reserve(maxInstanceNum);

		for (int i = 0; i < maxInstanceNum; i++)
		{
			// �A�N�^�[�𐶐�
			T* effect = new T;
			// null�`�F�b�N
			if (effect == nullptr)
			{
				Logger::GetInstance().SetLog("BulletManager::CreateBulletActors effect��nullptr");
				return;
			}
			//�A�N�^�[��ɓo�^
			m_Effects.emplace_back(effect);
		}
	}

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