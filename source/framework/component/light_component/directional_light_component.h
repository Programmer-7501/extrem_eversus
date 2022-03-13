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
class DirectionalLightComponent : public Component
{
protected:
	//! ディレクショナルライト
	CBufferData::DirectionalLight m_DirectionalLight;
public:
	DirectionalLightComponent(class Actor* owner);
	~DirectionalLightComponent() override;

	void SetLight(const CBufferData::DirectionalLight& directionalLight);

	/**
	* @brief 角度入力
	* @param[in] float(x) 角度X
	* @param[in] float(y) 角度Y
	* @param[in] float(z) 角度Z
	* @details degreeで入力
	*/
	void SetEuler(float x, float y, float z);
};