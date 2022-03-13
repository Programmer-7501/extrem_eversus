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
class DirectionalLightComponent : public Component
{
protected:
	//! �f�B���N�V���i�����C�g
	CBufferData::DirectionalLight m_DirectionalLight;
public:
	DirectionalLightComponent(class Actor* owner);
	~DirectionalLightComponent() override;

	void SetLight(const CBufferData::DirectionalLight& directionalLight);

	/**
	* @brief �p�x����
	* @param[in] float(x) �p�xX
	* @param[in] float(y) �p�xY
	* @param[in] float(z) �p�xZ
	* @details degree�œ���
	*/
	void SetEuler(float x, float y, float z);
};