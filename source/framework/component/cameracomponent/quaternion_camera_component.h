/**
 * @file quaternion_camera_component.h
 * @brief クォータニオンカメラコンポーネント
 */

#pragma once

#include"camera_component.h"

 /**
 * @brief クォータニオンカメラコンポーネント
 */
class QuaternionCameraComponent : public CameraComponent
{
protected:
	//! カメラの姿勢クォータニオン
	Conv_XM::Vector4f m_Quaternion;

	//! 前ベクトル
	Conv_XM::Vector3f m_Forward;
public:
	QuaternionCameraComponent(class Actor* owner);
	~QuaternionCameraComponent();

	/**
	* @brief 角度入力
	* @param[in] float(x) 角度X
	* @param[in] float(y) 角度Y
	* @param[in] float(z) 角度Z
	* @details degreeで入力
	*/
	void SetEuler(float x, float y, float z);


	inline void SetQuaternion(Conv_XM::Vector4f quaternion) { m_Quaternion = quaternion; }

	/**
	* @brief ビュープロジェクション行列登録
	*/
	virtual void SetCameraData() override;
};