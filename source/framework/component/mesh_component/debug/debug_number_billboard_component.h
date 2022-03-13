/**
 * @file billboard_component.h
 * @brief �r���{�[�h�R���|�[�l���g
 */

#pragma once

#include"../changeable_billboard_component.h"

class DebugNumberBillBoardComponent : public ChangeableBillBoardComponent
{
public:
	/**
	* @brief �����e�N�X�`���̓ǂݍ���
	*/
	static void LoadNumberTexture();

	/**
	* @brief �����e�N�X�`���̔j��
	*/
	static void UnloadNumberTexture();
private:
	int m_DrawNumber;

public:
	DebugNumberBillBoardComponent(class Actor* owner);
	virtual ~DebugNumberBillBoardComponent();

	void SetNumber(int num);
};