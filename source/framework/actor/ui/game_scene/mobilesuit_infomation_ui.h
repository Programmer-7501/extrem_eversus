/**
 * @file mobilesuit_infomation_ui.h
 * @brief ���r���X�[�c�̏��UI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief ���r���X�[�c�̏��UI�N���X
 */
class MobileSuitInfomationUI : public Actor
{
public:
	enum MobileSuitName
	{
		ENEMY001,
		ENEMY002,
		ALLY,
	};
public:

	/**
	* @brief �e�N�X�`���ǂݍ���
	*/
	static void LoadTexture();

	/**
	* @brief �e�N�X�`���j��
	*/
	static void UnloadTexture();

protected:
	//! �e���r���X�[�c
	class MobileSuit* m_ParentActor;

	//! ���C�t�R���|�[�l���g
	class LifeComponent* m_LifeComponent;

	//! �\�����r���{�[�h�R���|�[�l���g
	class BillBoardComponent* m_NameTagBillBoardComponent;

	//! ���C�t�\���r���{�[�h�R���|�[�l���g
	class GaugeBillBoardComponent* m_LifeGaugeBillBoardComponent;

	//! ���r���X�[�c�̖��O
	MobileSuitName m_MobileSuitName;
public:
	MobileSuitInfomationUI();
	~MobileSuitInfomationUI();

	inline void SetParentActor(class MobileSuit* mobileSuit) { m_ParentActor = mobileSuit; }

	inline void SetMobileSuitName(MobileSuitName mobileSuitName) { m_MobileSuitName = mobileSuitName; }
protected:

	/**
	* @brief ���r���X�[�c�̏��UI�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief ���r���X�[�c�̏��UI�Ǝ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief ���r���X�[�c�̏��UI�Ǝ��̍X�V����
	*/
	virtual void UpdateActor() override;
};