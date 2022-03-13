/**
 * @file gauge_billboard_component.h
 * @brief �Q�[�W�r���{�[�h�R���|�[�l���g
 */

#pragma once

#include"changeable_billboard_component.h"

 /**
 * @brief �Q�[�W�X�v���C�g�R���|�[�l���g
 */
class GaugeBillBoardComponent : public ChangeableBillBoardComponent
{
public:
	enum Direction
	{
		LEFT,
		RIGHT,
	};

private:
	//! ��������
	Direction m_Direction;

	//! ����
	float m_Ratio;

	//! �ő�̒���
	float m_MaxLength;

	//! ����
	float m_Length;

	//! �J�n�̃|�W�V����
	Conv_XM::Vector3f m_StartPosition;
public:
	GaugeBillBoardComponent(class Actor* owner);
	virtual ~GaugeBillBoardComponent()override;

	/**
	* @brief �X�V����
	*/
	virtual void Update()override;

	inline void SetDirection(Direction dir) { m_Direction = dir; }
	inline void SetRatio(float ratio) 
	{
		m_Ratio = ratio;
		m_IsUpdate = true;
	}
	inline void SetMaxLength(float maxLength) { m_MaxLength = maxLength; }
	inline void SetStartPosition(const Conv_XM::Vector3f& start) { m_StartPosition = start; }

private:

	/**
	* @brief ���_�\���̂̍X�V
	*/
	virtual void UpdateVetexData() override;
};