
#include"../../../system/main.h"
#include "grid_component.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

namespace GridData
{
	static constexpr float k_Size = 10.0f;
	static constexpr int k_Count = 10;
	static const Conv_XM::Vector4f k_Color(0.0f, 1.0f, 0.2f, 1.0f);
}

GridComponent::GridComponent(class Actor* owner)
	: MeshComponent(owner)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_VertexCount(0)
	, m_VertexBuffer(nullptr)
{
	// フォワードレンダリングに変更
	m_RenderingWay = RENDERING_FORWARD;
}

void GridComponent::Draw() const
{
	// nullチェック
	if (m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_VertexBuffer == nullptr)
	{
		Logger::GetInstance().SetLog("GridComponent::Draw nullptr");
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
	Conv_XM::Matrix4x4f mtx = DirectX::XMMatrixIdentity();
	Renderer::GetInstance().SetWorldMatrix(mtx);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_COLOR);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 描画
	m_DeviceContext->Draw(m_VertexCount, 0);
}

bool GridComponent::LoadData()
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

	// メッシュの制作
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init CreateMeshが失敗");
		return false;
	}

	return true;
}

bool GridComponent::CreateMesh()
{
	// デバイスの取得
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("GridComponent::CreateMesh deviceがnullptr");
		return false;
	}

	m_VertexCount = (GridData::k_Count + 1) * 4;

	float length = GridData::k_Size * GridData::k_Count;

	float start = -length * 0.5f;

	float end = length * 0.5f;

	VERTEX_COLOR* vertex = nullptr;
	vertex = DBG_NEW VERTEX_COLOR[m_VertexCount];

	for (int i = 0; i < GridData::k_Count + 1; i++)
	{
		if (i >= m_VertexCount)
		{
			break;
		}

		int n = (GridData::k_Count + 1) * 2;
		vertex[i * 2].Position = Conv_XM::Vector3f(start + GridData::k_Size * i, 0.0f, start);
		vertex[i * 2].Color = GridData::k_Color;

		vertex[i * 2 + 1].Position = Conv_XM::Vector3f(start + GridData::k_Size * i, 0.0f, end);
		vertex[i * 2 + 1].Color = GridData::k_Color;

		vertex[n + i * 2].Position = Conv_XM::Vector3f(start, 0.0f, start + GridData::k_Size * i);
		vertex[n + i * 2].Color = GridData::k_Color;

		vertex[n + i * 2 + 1].Position = Conv_XM::Vector3f(end, 0.0f, start + GridData::k_Size * i);
		vertex[n + i * 2 + 1].Color = GridData::k_Color;
	}

	//頂点バッファ生成
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_COLOR) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData{};
	vertexInitData.pSysMem = vertex;

	HRESULT hr;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		if (vertex)
		{
			delete[] vertex;
			vertex = nullptr;
		}
		Logger::GetInstance().SetLog("GridComponent::CreateMesh CreateBufferが失敗");
		return false;
	}

	if (vertex)
	{
		delete[] vertex;
		vertex = nullptr;
	}

	return true;
}

bool GridComponent::GetDeviceContext()
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

bool GridComponent::GetShader()
{
	// 頂点シェーダーの取得
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_GRID);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_GRID);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_GRID);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
