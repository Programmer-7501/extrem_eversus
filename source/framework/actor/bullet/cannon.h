/**
 * @file bullet.h
 * @brief �o���b�g
 */

#pragma once

#include"bullet.h"

 /**
 * @brief �L���m���N���X
 */
class Cannon : public Bullet
{
private:
	//! �e�̑��x
	Conv_XM::Vector3f m_Velocity;
public:
	Cannon();
	~Cannon();

	/**
	* @brief �A�N�e�B�u����
	*/
	virtual void SetActive(bool flag)override;

	/**
	* @brief ���x�̃Z�b�g
	* @param[in] const Conv_XM::Vector3f&(velocity) ���x
	*/
	virtual void SetVelocity(const Conv_XM::Vector3f& velocity) override;

protected:

	/**
	* @brief �L���m���Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �L���m���Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �L���m���Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief �L���m���Ǝ��̍X�V����
	*/
	virtual void UpdateActor()override;

	/**
	* @brief �����蔻�菈��
	* @details �Փˎ��ɌĂяo�����֐�
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;
};