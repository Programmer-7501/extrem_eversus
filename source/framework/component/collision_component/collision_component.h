/**
 * @file collision_component.h
 * @brief �R���W�����R���|�[�l���g
 */

#pragma once

#include"../component.h"

 /**
 * @brief �R���W�����R���|�[�l���g
 */
class CollisionComponent : public Component
{
protected:
	//! �g�p���邩�ǂ���
	bool m_IsUseFlag;
public:
	CollisionComponent(class Actor* owner);
	~CollisionComponent();

	inline void SetIsUseFlag(bool flag) { m_IsUseFlag = flag; }
	inline bool GetIsUseFlag() const { return m_IsUseFlag; }
};