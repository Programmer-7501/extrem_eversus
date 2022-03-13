/**
 * @file gauge_sprite_component.h
 * @brief �Q�[�W�X�v���C�g�R���|�[�l���g
 */

#pragma once

#include"changeable_sprite_component.h"

 /**
 * @brief �Q�[�W�X�v���C�g�R���|�[�l���g
 */
class GaugeSpriteComponent : public ChangeableSpriteComponent
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

	//! �J�n�̃|�W�V����
	Conv_XM::Vector2f m_StartPosition;
public:
	GaugeSpriteComponent(class Actor* owner);
	virtual ~GaugeSpriteComponent()override;

	/**
	* @brief �X�V����
	*/
	virtual void Update()override;

	inline void SetDirection(Direction dir) { m_Direction = dir; }
	inline void SetRatio(float ratio) { m_Ratio = ratio; }
	inline void SetMaxLength(float maxLength) { m_MaxLength = maxLength; }
	inline void SetStartPosition(const Conv_XM::Vector2f& start) { m_StartPosition = start; }
};