/**
 * @file directional_light_component.h
 * @brief �f�B���N�V���i�����C�g�R���|�[�l���g
 */

#pragma once

#include"../../../directx/render_data/cbuffer_data.h"
#include"../component.h"

 /**
 * @brief �f�B���N�V���i�����C�g�R���|�[�l���g
 */
class PointLightComponent : public Component
{
protected:
	//! �f�B���N�V���i�����C�g
	CBufferData::PointLight m_PointLight;
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent() override;

	void SetLight(const CBufferData::PointLight& pointLight);
	void SetPos(float x, float y, float z);
};