/**
 * @file boost_component.h
 * @brief ブーストコンポーネント
 */

#pragma once

#include"../component.h"

 /**
 * @brief ブーストコンポーネント
 */
class BoostComponent : public Component
{
private:
	//! 最大ブースト量
	float m_MaxBoost;

	//! 現在のブースト量
	float m_CurrentBoost;

	//! ブーストを使用したかどうか
	bool m_IsUseBoost;
public:
	BoostComponent(class Actor* owner);
	~BoostComponent();

	inline void SetMaxBoost(float maxBoost)
	{
		m_MaxBoost = maxBoost;
		m_CurrentBoost = maxBoost;
	}
	inline void UseBoost(float boost)
	{
		m_CurrentBoost -= boost; 
		m_IsUseBoost = true;
	}
	inline void ResetBoost() { m_CurrentBoost = m_MaxBoost; }

	inline float GetBoostRatio() const { return (m_CurrentBoost / m_MaxBoost); }

	bool GetIsOverHeat() const;

	inline bool GetIsUseBoostFlag() const { return m_IsUseBoost; }
	inline void SetIsUseBoostFlag(bool flag){ m_IsUseBoost = flag; }
};