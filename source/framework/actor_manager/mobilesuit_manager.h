/**
 * @file mobilesuit_manager.h
 * @brief ���r���X�[�c�̊Ǘ����s��
 */

#pragma once

#include"actor_manager.h"
#include"../actor/mobilesuit/mobilesuit.h"

 /**
 * @brief ���r���X�[�c�̊Ǘ�������N���X
 */
class MobileSuitManager : public ActorManager
{
private:
	//! ���r���X�[�c�A�N�^�[�Y
	std::vector<MobileSuit*> m_MobileSuits;
public:
	MobileSuitManager();
	~MobileSuitManager();

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
	* @brief ���r���X�[�c�̐���
	: @details ���r���X�[�c�̐����ԍ�
	*/
	void SpawnMobileSuit(int spawnNum);


	/**
	* @brief �A�N�^�[�����֐�
	* @return T �쐬�����A�N�^�[
	*/
	template<typename T>
	T* CreateMobileSuitActor()
	{
		// �A�N�^�[�𐶐�
		T* mobileSuit = new T;
		// null�`�F�b�N
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::CreateMobileSuitActor mobileSuit��nullptr");
			return nullptr;
		}
		//�A�N�^�[��ɓo�^
		m_MobileSuits.emplace_back(mobileSuit);

		return mobileSuit;
	}
};