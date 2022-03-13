/**
 * @file sideseven_object.h
 * @brief �T�C�h�Z�u���̃I�u�W�F�N�g
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �T�C�h�Z�u���̃I�u�W�F�N�g�z�u�N���X
 */
class SideSevenObject : public Actor
{
public:
	/**
	* @brief ���f���̓ǂݍ���
	*/
	static void LoadModel();

	/**
	* @brief ���f���̔j��
	*/
	static void UnloadModel();
private:
	//! ���I�u�W�F�N�g
	class InstancingModelComponent* m_RoadObjectInstancingModelComponent;

	//! �R���N���[�g�I�u�W�F�N�g
	class InstancingModelComponent* m_ConcreteObjectInstancingModelComponent;

	//! �����I�u�W�F�N�g
	class InstancingModelComponent* m_BuildingObjectInstancingModelComponent;

	//! �����I�u�W�F�N�g�̃R���W�����R���|�[�l���g
	std::vector<class AABBCollisionComponent*> m_BuildingObjectCollisionComponents;
public:
	SideSevenObject();
	~SideSevenObject() override;

private:

	/**
	* @brief �ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ����������
	*/
	void InitActor() override;
};
