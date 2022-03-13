/**
 * @file debug_camera.h
 * @brief デバッグカメラ
 */

#pragma once

#include"../actor.h"

 /**
 * @brief デバッグカメラ
 */
class DebugCamera : public Actor
{
private:
	//! カメラコンポーネント
	class CameraComponent* m_CameraComponent;
public:
	DebugCamera();
	~DebugCamera() override;

private:
	/**
	* @brief アクター独自の初期化処理
	*/
	void InitActor() override;

	/**
	* @brief アクター独自の入力処理
	*/
	void ProcessInputActor() override;
};
