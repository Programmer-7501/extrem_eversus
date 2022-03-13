/**
 * @file debug_camera.h
 * @brief �f�o�b�O�J����
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �f�o�b�O�J����
 */
class DebugCamera : public Actor
{
private:
	//! �J�����R���|�[�l���g
	class CameraComponent* m_CameraComponent;
public:
	DebugCamera();
	~DebugCamera() override;

private:
	/**
	* @brief �A�N�^�[�Ǝ��̏���������
	*/
	void InitActor() override;

	/**
	* @brief �A�N�^�[�Ǝ��̓��͏���
	*/
	void ProcessInputActor() override;
};
