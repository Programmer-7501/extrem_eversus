/**
 * @file light_camera_component.h
 * @brief ���C�g�J�����R���|�[�l���g
 */

#pragma once

#include"quaternion_camera_component.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief �J�����R���|�[�l���g
 */
class LightCameraComponent : public QuaternionCameraComponent
{
private:
	//! �Ǐ]����A�N�^�[
	Actor* m_ChaseActor;
public:
	LightCameraComponent(class Actor* owner);
	virtual ~LightCameraComponent();

	void Update()override;

	/**
	* @brief �r���[�v���W�F�N�V�����s��o�^
	*/
	void SetCameraData() override;

	inline void SetChaseTarget(class Actor* actor) 
	{
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("LightCameraComponent::SetChaseTarget actor ��nullptr");
			return;
		}

		m_ChaseActor = actor; 
	}
};