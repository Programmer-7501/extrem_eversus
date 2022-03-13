
#include"../../../system/main.h"
#include "instancing_model_component.h"
#include"../../../directx/resource/model.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"


InstancingModelComponent::InstancingModelComponent(Actor * owner)
	:MeshComponent(owner)
	, m_Model(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_MaxInstanceNum(0)
	, m_InstanceMatrixBuffer(nullptr)
{
}

void InstancingModelComponent::Draw() const
{
	// nullチェック
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InstanceMatrixBuffer == nullptr || m_MaxInstanceNum <= 0)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::Draw nullptr");
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

	Renderer& renderer = Renderer::GetInstance();
	renderer.SetMaterial(m_Material);

	// モデル描画
	m_Model->InstancingDraw(m_InstanceMatrixBuffer.Get(), m_MaxInstanceNum);
}

void InstancingModelComponent::ShadowDraw() const
{
	// nullチェック
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::ShadowDraw nullptr");
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

	// モデル描画
	m_Model->InstancingDraw(m_InstanceMatrixBuffer.Get(), m_MaxInstanceNum);
}

void InstancingModelComponent::SetInstanceMatrixData(const std::vector<Conv_XM::Matrix4x4f>& instanceMatrixData)
{
	if (instanceMatrixData.empty() == true)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::CreateInstancingData instanceMatrixDataが空");
		return;
	}

	m_MaxInstanceNum = static_cast<int>(instanceMatrixData.size());

	Renderer& renderer = Renderer::GetInstance();
	ID3D11Device* device = renderer.GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::CreateInstancingData deviceがnullptr");
		return;
	}

	HRESULT hr;

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Conv_XM::Matrix4x4f) * m_MaxInstanceNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = instanceMatrixData.data();

	hr = device->CreateBuffer(&bd, &sd, m_InstanceMatrixBuffer.GetAddressOf());

	if (FAILED(hr) == true)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::CreateInstancingData CreateBufferが失敗");
		return;
	}
}

bool InstancingModelComponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::Init GetShaderが失敗");
		return false;
	}

	return true;
}

bool InstancingModelComponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool InstancingModelComponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_INSTANCING);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::GetShader m_VertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_GBUFFERINPUT);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::GetInterface m_PixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_INSTANCING);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}