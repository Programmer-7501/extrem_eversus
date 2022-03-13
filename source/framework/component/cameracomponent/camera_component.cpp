
#include"../../../system/main.h"
#include "camera_component.h"
#include"../../../directx/renderer.h"

CameraComponent::CameraComponent(Actor * owner)
	: Component(owner)
	, m_CameraData{}
	, m_Position(CameraData::k_DefaultPosition)
	, m_Up(CameraData::k_DefaultUp)
	, m_Target(CameraData::k_DefaultTarget)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::SetCameraData()
{
	// レンダラー取得
	Renderer& renderer = Renderer::GetInstance();

	// カメラポジションセット
	m_CameraData.CameraPosition = m_Position;
	//ビューマトリクス入力
	m_CameraData.View = DirectX::XMMatrixLookAtLH(m_Position, m_Target, m_Up);
	//プロジェクションマトリクス設定
	m_CameraData.Projection = DirectX::XMMatrixPerspectiveFovLH(CameraData::k_Fov, SystemData::k_AspectRate, CameraData::k_NearZ, CameraData::k_FarZ);
	// ビュープロジェクション行列
	m_CameraData.CameraViewProjectionInverse = DirectX::XMMatrixInverse(nullptr, (DirectX::XMMATRIX)m_CameraData.View * (DirectX::XMMATRIX)m_CameraData.Projection);
	// コンスタントバッファにセット
	renderer.SetDrawData(m_CameraData);
}
