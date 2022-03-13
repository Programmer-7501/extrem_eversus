
#include"../../../../system/main.h"
#include "obb_debug_view_component.h"
#include"../../../../directx/renderer.h"
#include"../../../actor/actor.h"
#include"../../../../directx/resource/shader_manager.h"
#include"../../../scene/scene.h"
#include"../../../scene/scene_manager.h"
#include"../../../scene_function/scene_render_manager.h"

bool OBBDebugViewComponent::m_IsUseFlag = true;

void OBBDebugViewComponent::SetIsUseFlag(bool flag)
{
	m_IsUseFlag = flag;
}

OBBDebugViewComponent::OBBDebugViewComponent(Actor * owner)
	: MeshComponent(owner)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_VertexBuffer(nullptr)
	, m_IndexNum(0)
	, m_Position(Conv_XM::Vector3f(0.0f,0.0f,0.0f))
	, m_Scale(Conv_XM::Vector3f(1.0f, 1.0f, 1.0f))
	, m_Quaternion(DirectX::XMQuaternionIdentity())
{
	// フォワードレンダリングに変更
	m_RenderingWay = RENDERING_FORWARD;
}

void OBBDebugViewComponent::Draw() const
{
	// nullチェック
	if (m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_VertexBuffer == nullptr)
	{
		Logger::GetInstance().SetLog("OBBDebugViewComponent::Draw nullptr");
		return;
	}
	if (m_Owner->GetActive() == false || m_IsUseFlag == false || m_Visible == false)
	{
		return;
	}

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxT, mtxR, mtxS, mtxW;
	mtxT = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	mtxR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);
	mtxS = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;
	Renderer::GetInstance().SetWorldMatrix(mtxW);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_COLOR);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//インデックスバッファ設定
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 描画
	m_DeviceContext->DrawIndexed(m_IndexNum, 0, 0);
}

bool OBBDebugViewComponent::LoadData()
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

bool OBBDebugViewComponent::CreateMesh()
{
	Renderer& renderer = Renderer::GetInstance();
	ID3D11Device* device = renderer.GetDevice();

	//頂点情報
	VERTEX_COLOR vertex[8];

	vertex[0].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
	vertex[0].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[1].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
	vertex[1].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[2].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
	vertex[2].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[3].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
	vertex[3].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[4].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
	vertex[4].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[5].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
	vertex[5].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[6].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
	vertex[6].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[7].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
	vertex[7].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_COLOR) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	device->CreateBuffer(&bd, &sd, m_VertexBuffer.GetAddressOf());


	WORD index[]{
		0,	1,
		1,	3,
		3,	2,
		2,	0,
		0,	4,
		4,	5,
		5,	1,
		3,	7,
		7,	5,
		2,	6,
		6,	4,
		6,	7
	};

	m_IndexNum = ARRAYSIZE(index);

	// インデックスバッファとして作成するための情報設定
	D3D11_BUFFER_DESC index_buffer_desc;
	index_buffer_desc.ByteWidth = sizeof(WORD) * m_IndexNum;
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	// インデックスバッファの初期値指定
	D3D11_SUBRESOURCE_DATA index_init_data;
	// バッファの初期値設定
	index_init_data.pSysMem = index;
	// インデックスバッファでは使用しない
	index_init_data.SysMemPitch = 0;
	index_init_data.SysMemSlicePitch = 0;

	// インデックスバッファ作成
	device->CreateBuffer(&index_buffer_desc, &index_init_data, m_IndexBuffer.GetAddressOf());

	return true;
}

bool OBBDebugViewComponent::GetDeviceContext()
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

bool OBBDebugViewComponent::GetShader()
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
