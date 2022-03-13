/**
 * @file quaternion_camera_component.h
 * @brief �N�H�[�^�j�I���J�����R���|�[�l���g
 */

#pragma once

#include"camera_component.h"

 /**
 * @brief �N�H�[�^�j�I���J�����R���|�[�l���g
 */
class QuaternionCameraComponent : public CameraComponent
{
protected:
	//! �J�����̎p���N�H�[�^�j�I��
	Conv_XM::Vector4f m_Quaternion;

	//! �O�x�N�g��
	Conv_XM::Vector3f m_Forward;
public:
	QuaternionCameraComponent(class Actor* owner);
	~QuaternionCameraComponent();

	/**
	* @brief �p�x����
	* @param[in] float(x) �p�xX
	* @param[in] float(y) �p�xY
	* @param[in] float(z) �p�xZ
	* @details degree�œ���
	*/
	void SetEuler(float x, float y, float z);


	inline void SetQuaternion(Conv_XM::Vector4f quaternion) { m_Quaternion = quaternion; }

	/**
	* @brief �r���[�v���W�F�N�V�����s��o�^
	*/
	virtual void SetCameraData() override;
};