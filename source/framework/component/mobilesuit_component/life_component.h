/**
 * @file life_component.h
 * @brief ライフコンポーネント
 */

#pragma once

#include"../component.h"

 /**
 * @brief ライフコンポーネント
 */
class LifeComponent : public Component
{
private:
	//! 最大ライフ
	float m_MaxLife;
	
	//! 現在のライフ
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