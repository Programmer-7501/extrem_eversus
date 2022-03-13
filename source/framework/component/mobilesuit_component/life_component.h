/**
 * @file life_component.h
 * @brief ���C�t�R���|�[�l���g
 */

#pragma once

#include"../component.h"

 /**
 * @brief ���C�t�R���|�[�l���g
 */
class LifeComponent : public Component
{
private:
	//! �ő僉�C�t
	float m_MaxLife;
	
	//! ���݂̃��C�t
	float m_CurrentLife;
public:
	LifeComponent(class Actor* owner);
	~LifeComponent();

	inline void SetMaxLife(float life) 
	{
		m_MaxLife = life;
		m_CurrentLife = life;
	}
	void PlusLife(float life);

	void SetLife(float life);

	inline float GetLife() const { return m_CurrentLife; }
	inline float GetLifeRatio() const { return (m_CurrentLife / m_MaxLife); }
};