
#include"../../../system/main.h"
#include "light_camera_component.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"

/**
* @namespace LightCameraData
* @brief ライトカメラデータ
*/
namespace LightCameraData
{
	//! フォブデータ
	static constexpr float k_Fov = DirectX::XMConvertToRadians(60.0f);
	//! 切り取り幅
	static constexpr float k_Width = 64.0f;
	//! 切り取り高さ
	static constexpr float k_Height = 64.0f;
	//! 二アーZ
	static constexpr float k_NearZ = 0.1f;
	//! ファーZ
	static constexpr float k_FarZ = 100.0f;

	//! 地面からどれだけ離れた位置にいるか
	static constexpr float k_GroundHeight = 20.0f;
}

LightCameraComponent::LightCameraComponent(Actor * owner)
	: QuaternionCameraComponent(owner)
	, m_ChaseActor(nullptr)
{
}

LightCameraComponent::~LightCameraComponent()
{
}

void LightCameraComponent::Update()
{
	if (m_ChaseActor == nullptr)
	{
		return;
	}
	m_Position = m_ChaseActor->GetPosition();
	m_Position.y += LightCameraData::k_GroundHeight;
}

void LightCameraComponent::SetCameraData()
{
	// レンダラー取得
	Renderer& renderer = Renderer::GetInstance();

	Conv_XM::Matrix4x4f view, projection, viewProjection;

	//ビューマトリクス入力
	view = DirectX::XMMatrixLookToLH(m_Position, m_Forward, m_Up);
	//プロジェクションマトリクス設定
	projection = DirectX::XMMatrixOrthographicLH(LightCameraData::k_Width * 2, LightCameraData::k_Height * 2, LightCameraData::k_NearZ, LightCameraData::k_FarZ);
	// ビュープロジェクション行列
	viewProjection = (DirectX::XMMATRIX)view * (DirectX::XMMATRIX)projection;

	// コンスタントバッファにセット
	renderer.SetViewProjectionMatrix(view, projection);
	renderer.SetLightViewProjection(viewProjection);
}
