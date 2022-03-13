/**
 * @file lockon_camera_component.h
 * @brief ロックオンカメラコンポーネント
 */

#pragma once

#include"camera_component.h"

 /**
 * @brief ロックオンカメラコンポーネント
 */
class LockOnCameraComponent : public CameraComponent
{
private:
	//! ロックオン対象
	class Actor* m_TargetActor;

	//! カメラの姿勢クォータニオン
	Conv_XM::Vector4f m_Quaternion;

	//! カメラの前ベクトル
	Conv_XM::Vector3f m_Forward;

	//! カメラの右ベクトル
	Conv_XM::Vector3f m_Right;

	//! 注視点を動かさないフラグ
	bool m_IsNotTargetMoveFlag;

	//! ターゲットのポジションを保存する変数
	Conv_XM::Vector3f m_StoreTargetPos;
public:
	LockOnCameraComponent(class Actor* owner);
	~LockOnCameraComponent();

	/**
	* @brief 更新処理
	*/
	virtual void Update();

	inline void SetTarget(class Actor* actor) { m_TargetActor = actor; }

	inline Conv_XM::Vector3f GetForward() const { return m_Forward; }

	inline Conv_XM::Vector3f GetRight() const { return m_Right; }

	void SetNotMoveFlag(bool flag);
};