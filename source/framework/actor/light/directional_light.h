/**
 * @file directional_light.h
 * @brief ���ڌ�
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief ���ڌ��N���X
 */
class DirectionalLight : public Actor
{
private:
	//! ���C�g�J�����R���|�[�l���g
	class LightCameraComponent* m_LightCameraComponent;

	//! �f�B���N�V���i�����C�g�R���|�[�l���g
	class DirectionalLightComponent* m_DirectionalLightComponent;

public:
	DirectionalLight();
	~DirectionalLight() override;

	void SetChaseTarget(class Actor* actor);

	/**
	* @brief �p�x����
	* @param[in] float(x) �p�xX
	* @param[in] float(y) �p�xY
	* @param[in] float(z) �p�xZ
	* @details degree�œ���
	*/
	void SetEuler(float x, float y, float z);

	void SetLightParameter(const CBufferData::DirectionalLight& directionalLight);
private:

	/**
	* @brief �A�N�^�[�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �A�N�^�[�Ǝ��̏���������
	*/
	void InitActor() override;
};
