/**
 * @file rigidbody_component.h
 * @brief リジッドボディコンポーネント
 */

#pragma once

#include"../component.h"

 /**
 * @brief リジッドボディコンポーネント
 */
class RigidBodyComponent : public Component
{
private:
	//! 接地したかどうか
	bool m_IsGround;
	//! 重力
	float m_Gravity;
	//! 重力を加えるかどうか
	bool m_IsUseGravity;
	//! 地上の摩擦
	float m_GroundFriction;
	//! 空中の摩擦
	float m_AirFriction;
	//! 速度
	Conv_XM::Vector3f m_Direction;

	//! オブジェクトに接地しているかどうか
	bool m_IsObjectGround;

	//! モビルスーツかどうか
	bool m_IsMobileSuit;
public:
	RigidBodyComponent(class Actor* owner);
	~RigidBodyComponent();

	// セッター、ゲッター
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
	* @brief 入力処理
	*/
	void Update()override;
};