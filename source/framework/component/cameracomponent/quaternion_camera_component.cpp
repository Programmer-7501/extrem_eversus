
#include"../../../system/main.h"
#include "quaternion_camera_component.h"
#include"../../../directx/renderer.h"

QuaternionCameraComponent::QuaternionCameraComponent(Actor * owner)
	: CameraComponent(owner)
	, m_Quaternion(DirectX::XMQuaternionIdentity())
	, m_Forward(Conv_XM::Vector3f::WorldForward)
{
}

QuaternionCameraComponent::~QuaternionCameraComponent()
{
}

void QuaternionCameraComponent::SetEuler(float x, float y, float z)
{
	float rad_X, rad_Y, rad_Z;
	rad_X = DirectX::XMConvertToRadians(x);
	rad_Y = DirectX::XMConvertToRadians(y);
	rad_Z = DirectX::XMConvertToRadians(z);
	m_Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rad_X, rad_Y, rad_Z);

	m_Forward = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldForward, m_Quaternion);
	m_Forward = DirectX::XMVector3Normalize(m_Forward);
	m_Up = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldUp, m_Quaternion);
	m_Up = DirectX::XMVector3Normalize(m_Up);
}

void QuaternionCameraComponent::SetCameraData()
{
	// レンダラー取得
	Renderer& renderer = Renderer::GetInstance();

	// 長さが0なら
	if (Conv_XM::Vector3f::LengthSq(m_Forward) < Math::k_Epsilon)
	{
		return;
	}

	// カメラポジションセット
	m_CameraData.CameraPosition = m_Position;
	//ビューマトリクス入力
	m_CameraData.View = DirectX::XMMatrixLookToLH(m_Position, m_Forward, m_Up);
	//プロジェクションマトリクス設定
	m_CameraData.Projection = DirectX::XMMatrixPerspectiveFovLH(CameraData::k_Fov, SystemData::k_AspectRate, CameraData::k_NearZ, CameraData::k_FarZ);
	// ビュープロジェクション行列
	m_CameraData.CameraViewProjectionInverse = DirectX::XMMatrixInverse(nullptr, (DirectX::XMMATRIX)m_CameraData.View * (DirectX::XMMATRIX)m_CameraData.Projection);
	// コンスタントバッファにセット
	renderer.SetDrawData(m_CameraData);
}
