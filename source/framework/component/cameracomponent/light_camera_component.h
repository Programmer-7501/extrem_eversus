/**
 * @file light_camera_component.h
 * @brief ライトカメラコンポーネント
 */

#pragma once

#include"quaternion_camera_component.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief カメラコンポーネント
 */
class LightCameraComponent : public QuaternionCameraComponent
{
private:
	//! 追従するアクター
	Actor* m_ChaseActor;
public:
	LightCameraComponent(class Actor* owner);
	virtual ~LightCameraComponent();

	void Update()override;

	/**
	* @brief ビュープロジェクション行列登録
	*/
	void SetCameraData() override;

	inline void SetChaseTarget(class Actor* actor) 
	{
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("LightCameraComponent::SetChaseTarget actor がnullptr");
			return;
		}

		m_ChaseActor = actor; 
	}
};