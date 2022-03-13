/**
 * @file directional_light_component.h
 * @brief ディレクショナルライトコンポーネント
 */

#pragma once

#include"../../../directx/render_data/cbuffer_data.h"
#include"../component.h"

 /**
 * @brief ディレクショナルライトコンポーネント
 */
class PointLightComponent : public Component
{
protected:
	//! ディレクショナルライト
	CBufferData::PointLight m_PointLight;
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent() override;

	void SetLight(const CBufferData::PointLight& pointLight);
	void SetPos(float x, float y, float z);
};