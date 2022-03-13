/**
 * @file collision_component.h
 * @brief コリジョンコンポーネント
 */

#pragma once

#include"../component.h"

 /**
 * @brief コリジョンコンポーネント
 */
class CollisionComponent : public Component
{
protected:
	//! 使用するかどうか
	bool m_IsUseFlag;
public:
	CollisionComponent(class Actor* owner);
	~CollisionComponent();

	inline void SetIsUseFlag(bool flag) { m_IsUseFlag = flag; }
	inline bool GetIsUseFlag() const { return m_IsUseFlag; }
};