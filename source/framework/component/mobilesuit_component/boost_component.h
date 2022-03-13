/**
 * @file boost_component.h
 * @brief �u�[�X�g�R���|�[�l���g
 */

#pragma once

#include"../component.h"

 /**
 * @brief �u�[�X�g�R���|�[�l���g
 */
class BoostComponent : public Component
{
private:
	//! �ő�u�[�X�g��
	float m_MaxBoost;

	//! ���݂̃u�[�X�g��
	float m_CurrentBoost;

	//! �u�[�X�g���g�p�������ǂ���
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