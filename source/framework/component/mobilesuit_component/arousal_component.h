/**
 * @file aruosal_component.h
 * @brief 覚醒コンポーネント
 */

#pragma once

#include"../component.h"

 /**
 * @brief 覚醒コンポーネント
 */
class ArousalComponent : public Component
{
private:
	//! 最大覚醒量
	float m_MaxArousal;

	//! 現在の覚醒量
	float m_CurrentArousal;

public:
	ArousalComponent(class Actor* owner);
	~ArousalComponent();

	inline void SetMaxArousal(float maxArousal)
	{
		m_MaxArousal = maxArousal;
	}

	inline float GetArousalRatio() const { return (m_CurrentArousal / m_MaxArousal); }
	inline void PlusArousal(float arousal) { m_CurrentArousal += arousal; }
};