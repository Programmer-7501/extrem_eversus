/**
 * @file bazooka.h
 * @brief �o�Y�[�J
 */

#pragma once

#include"bullet.h"

 /**
 * @brief �o�Y�[�J�N���X
 */
class Bazooka : public Bullet
{
public:
	/**
	* @brief ���f���ǂݍ��ݏ���
	*/
	static void LoadModel();

	/**
	* @brief ���f���j������
	*/
	static void UnloadModel();
private:
	//! ���f���R���|�[�l���g
	class ModelComponent* m_ModelComponent;

public:
	Bazooka();
	~Bazooka();

protected:

	/**
	* @brief �o�Y�[�J�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �o�Y�[�J�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �o�Y�[�J�Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief �o�Y�[�J�Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

	/**
	* @brief �����蔻�菈��
	* @details �Փˎ��ɌĂяo�����֐�
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;
};