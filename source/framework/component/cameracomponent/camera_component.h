/**
 * @file camera_component.h
 * @brief カメラコンポーネント
 */

#pragma once

#include"../component.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @namespace CameraData
 * @brief カメラデータ
 */
namespace CameraData
{
	//! フォブデータ
	static constexpr float k_Fov = DirectX::XMConvertToRadians(60.0f);
	//! 二アーZ
	static constexpr float k_NearZ = 0.1f;
	//! ファーZ
	static constexpr float k_FarZ = 1000.0f;
	//! デフォルトカメラポジション
	static const Conv_XM::Vector3f k_DefaultPosition = Conv_XM::Vector3f(0.0f, 5.0f, -5.0f);
	//! デフォルト上ベクトル
	static const Conv_XM::Vector3f k_DefaultUp = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
	//! デフォルトターゲット
	static const Conv_XM::Vector3f k_DefaultTarget = Conv_XM::Vector3f(0.0f, 0.0f, 0.0f);
}

 /**
 * @brief カメラコンポーネント
 */
class CameraComponent : public Component
{
protected:
	CBufferData::DrawData m_CameraData;

	//! ポジション
	Conv_XM::Vector3f m_Position;
	//! 上ベクトル
	Conv_XM::Vector3f m_Up;
	//! ターゲット
	Conv_XM::Vector3f m_Target;
public:
	CameraComponent(class Actor* owner);
	virtual ~CameraComponent();

	/**
	* @brief ビュープロジェクション行列登録
	*/
	virtual void SetCameraData();

	// ゲッター
	inline const Conv_XM::Matrix4x4f& GetViewMatrix() const { return m_CameraData.View; }

	inline void SetPosition(const Conv_XM::Vector3f& position) { m_Position = position; }
	inline void SetUp(const Conv_XM::Vector3f& up) { m_Up = up; }
	inline void SetTarget(const Conv_XM::Vector3f& target) { m_Target = target; }

	inline Conv_XM::Vector3f GetPosition() const { return m_Position; }
	inline Conv_XM::Vector3f GetUp() const { return m_Up; }
};