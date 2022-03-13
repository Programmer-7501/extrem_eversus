/**
 * @file changeable_billboard_component.h
 * @brief �ύX�\�r���{�[�h�R���|�[�l���g
 */

#pragma once

#include"billboard_component.h"

class ChangeableBillBoardComponent : public BillBoardComponent
{
protected:
	//! �F
	Conv_XM::Vector4f m_Color;
	//! �X�V�t���O
	bool m_IsUpdate;

public:
	ChangeableBillBoardComponent(class Actor* owner);
	virtual ~ChangeableBillBoardComponent();


	/**
	* @brief �X�V����
	*/
	virtual void Update()override;

	/**
	* @brief �`�揈��
	*/
	void Draw() const override;
private:

	/**
	* @brief ���_�\���̂̍X�V
	*/
	virtual void UpdateVetexData();

	/**
	* @brief �f�[�^�ǂݍ���
	* @return bool �����������ǂ���
	*/
	bool LoadData() override;

	/**
	* @brief ���b�V���쐬
	* @return bool �����������ǂ���
	*/
	bool CreateMesh() override;

	/**
	* @brief �V�F�[�_�[�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetShader() override;
};