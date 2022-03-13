
#include"main.h"
#include"fade.h"
#include"../directx/renderer.h"
#include"../directx/resource/shader_manager.h"
#include"../directx/render_data/srv_data.h"

Fade::Fade()
	:m_Active(false)
	, m_CurrentFrame(0)
	, m_FadeTime(0)
	, m_Arufa(0.0f)
	, m_FadeState(FADE_NONE)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_ShaderResourceView(nullptr)
	, m_Color(0.0f,0.0f,0.0f)
{

}

Fade::~Fade()
{
	
}

void Fade::Init()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_DeviceContextがnullptr");
		return;
	}

	// 頂点シェーダーの取得
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_UNLITCOLORTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_UNLITCOLORTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return;
	}

	// インプットレイアウトの取得
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_UNLITCOLORTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputLayoutがnullptr");
		return;
	}


	// デバイスの取得
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// nullチェック
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init deviceがnullptr");
		return;
	}

	HRESULT hr;

	//頂点バッファ生成
	//頂点書き換え用
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;//頂点書き換え用
	bd.ByteWidth = sizeof(VERTEX_COLOR_TEXCOOD) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//頂点書き換え用
	bd.MiscFlags = 0;

	hr = device->CreateBuffer(&bd, NULL, m_VertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ChangeableSpriteComponent::Init CreateBufferが失敗");
		return;
	}

	// 四角形のインデックスを定義
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	//// バッファを作成
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	// インデックス情報の追加
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;                 // デフォルトアクセス
	bufferDesc.ByteWidth = sizeof(WORD) * 6;                // サイズはインデックスの数 6
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;         // インデックスバッファを使用
	bufferDesc.CPUAccessFlags = 0;                          // CPUのバッファへのアクセス拒否

		//// リソースの設定
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = index;

	// インデックスバッファを作成
	hr = device->CreateBuffer(&bufferDesc, &initData, m_IndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ChangeableSpriteComponent::Init CreateBufferが失敗");
		return;
	}

}

void Fade::Update()
{
	if (!m_Active)
	{
		return;
	}

	switch (m_FadeState)
	{
	case Fade::FADE_IN:
		m_Arufa = (float)m_CurrentFrame / (float)m_FadeTime;
		break;
	case Fade::FADE_OUT:
		m_Arufa = 1.0f - (float)m_CurrentFrame / (float)m_FadeTime;
		break;
	default:
		break;
	}

	if (m_CurrentFrame >= m_FadeTime)
	{
		m_Active = false;
	}

	m_CurrentFrame++;
}

void Fade::Draw()
{
	Conv_XM::Vector4f color = Conv_XM::Vector4f(m_Color.x, m_Color.y, m_Color.z, m_Arufa);
	//頂点書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	m_DeviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_COLOR_TEXCOOD* vertex = (VERTEX_COLOR_TEXCOOD*)msr.pData;
	vertex[0] = { {-0.5f, -0.5f,0.0f}, {color},{0.0f,0.0f} };
	vertex[1] = { { 0.5f + SystemData::k_ScreenWidth, -0.5f,0.0f}, {color},{1.0f,0.0f} };
	vertex[2] = { {-0.5f, 0.5f + SystemData::k_ScreenHeight,0.0f},  {color},{0.0f,1.0f} };
	vertex[3] = { { 0.5f + SystemData::k_ScreenWidth, 0.5f + SystemData::k_ScreenHeight,0.0f},  {color},{1.0f,1.0f} };
	m_DeviceContext->Unmap(m_VertexBuffer.Get(), 0);

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Renderer::GetInstance().SetWorldMatrix(mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_COLOR_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// インデックスバッファを設定
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//テクスチャ設定
	m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &m_ShaderResourceView);

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画
	m_DeviceContext->DrawIndexed(6, 0, 0);
}

void Fade::SetFade(FadeMode state, int fadetime)
{
	if (m_Active == true)
	{
		return;
	}

	m_FadeState = state;
	m_Active = true;
	m_FadeTime = fadetime;
	m_CurrentFrame = 0;

	switch (m_FadeState)
	{
	case Fade::FADE_IN:
		m_Arufa = 0.0f;
		break;
	case Fade::FADE_OUT:
		m_Arufa = 1.0f;
		break;
	default:
		break;
	}
}