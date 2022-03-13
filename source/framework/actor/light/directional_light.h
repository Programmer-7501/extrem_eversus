/**
 * @file directional_light.h
 * @brief 直接光
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief 直接光クラス
 */
class DirectionalLight : public Actor
{
private:
	//! ライトカメラコンポーネント
	class LightCameraComponent* m_LightCameraComponent;

	//! ディレクショナルライトコンポーネント
	class DirectionalLightComponent* m_DirectionalLightComponent;

public:
	DirectionalLight();
	~DirectionalLight() override;

	void SetChaseTarget(class Actor* actor);

	/**
	* @brief 角度入力
	* @param[in] float(x) 角度X
	* @param[in] float(y) 角度Y
	* @param[in] float(z) 角度Z
	* @details degreeで入力
	*/
	void SetEuler(float x, float y, float z);

	void SetLightParameter(const CBufferData::DirectionalLight& directionalLight);
private:

	/**
	* @brief アクター独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief アクター独自の初期化処理
	*/
	void InitActor() override;
};
