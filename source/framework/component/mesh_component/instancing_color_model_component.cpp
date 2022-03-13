
#include"../../../system/main.h"
#include "instancing_color_model_component.h"
#include"../../../directx/resource/model.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

namespace InstancingColorModelComponentData
{
	static constexpr int k_DefaultInstancingNum = 1024;
}

InstancingColorModelComponent::InstancingColorModelComponent(Actor * owner)
	:MeshComponent(owner)
	, m_Model(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_MaxInstanceNum(InstancingColorModelComponentData::k_DefaultInstancingNum)
	, m_InstanceMatrixBuffer(nullptr)
{
}

void InstancingColorModelComponent::Update()
{
	if (m_IsUpdate == true)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		Renderer& renderer = Renderer::GetInstance();
		renderer.GetDeviceContext()->Map(m_InstanceMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

		InstanceColorData* instanceData = (InstanceColorData*)(map.pData);

		for (int i = 0; i < m_InstanceColorData.size(); i++)
		{
			instanceData[i] = m_InstanceColorData[i];
		}

		renderer.GetDeviceContext()->Unmap(m_InstanceMatrixBuffer.Get(), 0);

		m_IsUpdate = false;
	}
}

void InstancingColorModelComponent::Draw() const
{
	// nullチェック
	if (m_Model == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InstanceMatrixBuffer == nullptr || m_MaxInstanceNum <= 0)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Draw nullptr");
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
	UINT stride[2] = { sizeof(VERTEX_NORMAL_TEXCOOD), sizeof(InstanceColorData) };
	UINT offset[2] = { 0, 0 };
	m_Model->InstancingColorDraw(m_InstanceMatrixBuffer.Get(), static_cast<int>(m_InstanceColorData.size()), stride, offset);
}

bool InstancingColorModelComponent::CreateInstancingData()
{
	Renderer& renderer = Renderer::GetInstance();
	ID3D11Device* device = renderer.GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::CreateInstancingData deviceがnullptr");
		return false;
	}

	HRESULT hr;

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;//頂点書き換え用
	bd.ByteWidth = sizeof(InstanceColorData) * m_MaxInstanceNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//頂点書き換え用
	bd.MiscFlags = 0;

	hr = device->CreateBuffer(&bd, nullptr, m_InstanceMatrixBuffer.GetAddressOf());

	if (FAILED(hr) == true)
	{
		Logger::GetInstance().SetLog("InstancingModelComponent::CreateInstancingData CreateBufferが失敗");
		return false;
	}

	return true;
}

bool InstancingColorModelComponent::LoadData()
{
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	if (CreateInstancingData() == false)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Init CreateInstancingDataが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::Init GetShaderが失敗");
		return false;
	}

	return true;
}

bool InstancingColorModelComponent::GetDeviceContext()
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

bool InstancingColorModelComponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_INSTANCING_COLOR);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::GetShader m_VertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_INSTANCING_COLOR);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::GetInterface m_PixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_INSTANCING_COLOR);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("InstancingColorModelComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
