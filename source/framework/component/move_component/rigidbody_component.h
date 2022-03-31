/**
 * @file rigidbody_component.h
 * @brief ���W�b�h�{�f�B�R���|�[�l���g
 */

#pragma once

#include"../component.h"

 /**
 * @brief ���W�b�h�{�f�B�R���|�[�l���g
 */
class RigidBodyComponent : public Component
{
private:
	//! �ڒn�������ǂ���
	bool m_IsGround;
	//! �d��
	float m_Gravity;
	//! �d�͂������邩�ǂ���
	bool m_IsUseGravity;
	//! �n��̖��C
	float m_GroundFriction;
	//! �󒆂̖��C
	float m_AirFriction;
	//! ���x
	Conv_XM::Vector3f m_Direction;

	//! �I�u�W�F�N�g�ɐڒn���Ă��邩�ǂ���
	bool m_IsObjectGround;

	//! ���r���X�[�c���ǂ���
	bool m_IsMobileSuit;
public:
	RigidBodyComponent(class Actor* owner);
	~RigidBodyComponent();

	// �Z�b�^�[�A�Q�b�^�[
	inline bool GetIsGround() const { return m_IsGround; }
	inline void SetIsGround(bool flag) { m_IsGround = flag; }

	inline void SetGravity(float gravity) { m_Gravity = gravity; }

	inline void SetIsUseGravity(bool flag) { m_IsUseGravity = flag; }

	inline Conv_XM::Vector3f GetVelocity() const { return m_Direction; }
	inline void SetDirection(const Conv_XM::Vector3f& velocity) { m_Direction = velocity; }
	inline void SetVelocityX(float velocity_x) { m_Direction.x = velocity_x; }
	inline void SetVelocityY(float velocity_y) { m_Direction.y = velocity_y; }
	inline void SetVelocityZ(float velocity_z) { m_Direction.z = velocity_z; }

	inline void PlusVelocity(const Conv_XM::Vector3f& velocity) { m_Direction += velocity; }

	inline void SetGroundFriction(float friction) { m_GroundFriction = friction; }
	inline void SetAirFriction(float friction) { m_AirFriction = friction; }

	inline bool GetIsObjectGround() const { return m_IsObjectGround; }
	inline void SetIsObjectGround(bool flag) { m_IsObjectGround = flag; }

	inline void SetIsMobileSuit(bool flag) { m_IsMobileSuit = flag; }
private:

	/**
	* @brief ���͏���
	*/
	void Update()override;
};