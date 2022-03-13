/**
 * @file camera_component.h
 * @brief �J�����R���|�[�l���g
 */

#pragma once

#include"../component.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @namespace CameraData
 * @brief �J�����f�[�^
 */
namespace CameraData
{
	//! �t�H�u�f�[�^
	static constexpr float k_Fov = DirectX::XMConvertToRadians(60.0f);
	//! ��A�[Z
	static constexpr float k_NearZ = 0.1f;
	//! �t�@�[Z
	static constexpr float k_FarZ = 1000.0f;
	//! �f�t�H���g�J�����|�W�V����
	static const Conv_XM::Vector3f k_DefaultPosition = Conv_XM::Vector3f(0.0f, 5.0f, -5.0f);
	//! �f�t�H���g��x�N�g��
	static const Conv_XM::Vector3f k_DefaultUp = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
	//! �f�t�H���g�^�[�Q�b�g
	static const Conv_XM::Vector3f k_DefaultTarget = Conv_XM::Vector3f(0.0f, 0.0f, 0.0f);
}

 /**
 * @brief �J�����R���|�[�l���g
 */
class CameraComponent : public Component
{
protected:
	CBufferData::DrawData m_CameraData;

	//! �|�W�V����
	Conv_XM::Vector3f m_Position;
	//! ��x�N�g��
	Conv_XM::Vector3f m_Up;
	//! �^�[�Q�b�g
	Conv_XM::Vector3f m_Target;
public:
	CameraComponent(class Actor* owner);
	virtual ~CameraComponent();

	/**
	* @brief �r���[�v���W�F�N�V�����s��o�^
	*/
	virtual void SetCameraData();

	// �Q�b�^�[
	inline const Conv_XM::Matrix4x4f& GetViewMatrix() const { return m_CameraData.View; }

	inline void SetPosition(const Conv_XM::Vector3f& position) { m_Position = position; }
	inline void SetUp(const Conv_XM::Vector3f& up) { m_Up = up; }
	inline void SetTarget(const Conv_XM::Vector3f& target) { m_Target = target; }

	inline Conv_XM::Vector3f GetPosition() const { return m_Position; }
	inline Conv_XM::Vector3f GetUp() const { return m_Up; }
};