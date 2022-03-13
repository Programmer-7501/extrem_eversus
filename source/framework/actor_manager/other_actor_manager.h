/**
 * @file other_actor_manager.h
 * @brief ���̑��A�N�^�[�̊Ǘ����s��
 */

#pragma once

#include"actor_manager.h"
#include"../actor/actor.h"

 /**
 * @brief ���̑��A�N�^�[�̊Ǘ�������N���X
 */
class OtherActorManager : public ActorManager
{
protected:
	//! �A�N�^�[�W����
	std::vector<class Actor*> m_Actors;
public:
	OtherActorManager();
	~OtherActorManager();

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

	/**
	* @brief �A�N�^�[�����֐�
	* @return T �쐬�����A�N�^�[
	*/
	template<typename T>
	T* CreateActor()
	{
		// �A�N�^�[�𐶐�
		T* actor = new T;
		// null�`�F�b�N
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::CreateActor actor��nullptr");
			return nullptr;
		}
		//�A�N�^�[��ɓo�^
		m_Actors.emplace_back(actor);

		return actor;
	}
};