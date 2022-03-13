/**
 * @file aruosal_component.h
 * @brief �o���R���|�[�l���g
 */

#pragma once

#include"../component.h"

 /**
 * @brief �o���R���|�[�l���g
 */
class ArousalComponent : public Component
{
private:
	//! �ő�o����
	float m_MaxArousal;

	//! ���݂̊o����
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