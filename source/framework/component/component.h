/**
 * @file component.h
 * @brief �Q�[���I�u�W�F�N�g�̃R���|�[�l���g
 */

#pragma once

 /**
  * @brief �Q�[���I�u�W�F�N�g�̃R���|�[�l���g
  */
class Component
{
protected:
	enum UPDATEORDER
	{
		UPDATEORDER_UNKNOWN,
	};
protected:
	//! ���L�A�N�^�[
	class Actor* m_Owner;

	//! �X�V��
	int m_UpdateOrder;
public:
	Component(class Actor* owner,int updateOrder = UPDATEORDER_UNKNOWN);
	virtual ~Component();

	/**
	* @brief �ǂݍ��ݏ���
	*/
	virtual void Load();

	/**
	* @brief ����������
	*/
	virtual void Init();

	/**
	* @brief �I������
	*/
	virtual void Uninit();

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput();

	/**
	* @brief �X�V����
	*/
	virtual void Update();

	/**
	* @brief ���W�X�V�����֐�
	* @details ���L�A�N�^�[�����W�X�V���s�������ɌĂяo�����֐�
	*/
	virtual void OnUpdateWorldTransform();

	// �Q�b�^�[
	inline int GetUpdateOrder() const { return m_UpdateOrder; }
	
	inline Actor* GetOwner() const { return m_Owner; }
};