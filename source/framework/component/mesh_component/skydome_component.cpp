
#include"../../../system/main.h"
#include "skydome_component.h"
#include"../../../directx/resource/model.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

SkyDomeomponent::SkyDomeomponent(Actor * owner)
	: MeshComponent(owner)
	, m_Model(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
{
	// フォワードレンダリング
	m_RenderingWay = RENDERING_FORWARD;
}

void SkyDomeomponent::Draw() const
{
	// nullチェック
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("SkyDomeomponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false)
	{
		return;
	}

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();
	Renderer::GetInstance().SetWorldMatrix(mtxWorld);

	// モデル描画
	m_Model->Draw();
}

bool SkyDomeomponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetShaderが失敗");
		return false;
	}

	return true;
}

bool SkyDomeomponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool SkyDomeomponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_STANDARD3D);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetShader m_VertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_STANDARD3D);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_PixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_STANDARD3D);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
