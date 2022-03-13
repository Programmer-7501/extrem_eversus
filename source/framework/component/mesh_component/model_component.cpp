
#include"../../../system/main.h"
#include "model_component.h"
#include"../../../directx/resource/model.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

ModelComponent::ModelComponent(Actor * owner)
	: MeshComponent(owner)
	, m_Model(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_OffsetScale(1.0f,1.0f,1.0f)
	, m_OffsetQuaternion(DirectX::XMQuaternionIdentity())
{
	// 影描画をする
	m_IsShadowInput = true;
}

void ModelComponent::Draw() const
{
	// nullチェック
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false || m_Visible == false)
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

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_OffsetQuaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	Renderer& renderer = Renderer::GetInstance();
	renderer.SetWorldMatrix(mtxWorld);
	renderer.SetMaterial(m_Material);

	// モデル描画
	m_Model->Draw();
}

void ModelComponent::ShadowDraw() const
{
	// nullチェック
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::Draw nullptr");
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
	m_DeviceContext->PSSetShader(NULL, NULL, 0);

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_OffsetQuaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	Renderer::GetInstance().SetWorldMatrix(mtxWorld);

	// モデル描画
	m_Model->Draw();
}

bool ModelComponent::LoadData()
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

bool ModelComponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool ModelComponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_GBUFFERINPUT);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetShader m_VertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_GBUFFERINPUT);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_PixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_GBUFFERINPUT);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
