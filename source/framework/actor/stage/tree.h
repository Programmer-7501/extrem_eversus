/**
 * @file tree.h
 * @brief �؂̃I�u�W�F�N�g
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �؂̃I�u�W�F�N�g�z�u�N���X
 */
class Tree : public Actor
{
public:

	/**
	* @brief �e�N�X�`���ǂݍ���
	*/
	static void LoadTexture();

	/**
	* @brief �e�N�X�`���̔j��
	*/
	static void UnloadTexture();

private:
	//! �r���{�[�h�R���|�[�l���g
	std::vector<class BillBoardComponent*> m_BillBoardComponents;

public:
	Tree();
	~Tree() override;

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
