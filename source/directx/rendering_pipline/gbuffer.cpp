
#include"../../system/main.h"
#include "../rendering_pipline/gbuffer.h"
#include"../renderer.h"

GBuffer::GBuffer(
	ID3D11RenderTargetView * albedColorRTV,
	ID3D11RenderTargetView * normalRTV,
	ID3D11RenderTargetView * specularRTV,
	ID3D11RenderTargetView * depthRTV,
	ID3D11DepthStencilView * mainDS,
	float clearColor_R,
	float clearColor_B,
	float clearColor_G,
	float clearColor_A
)
	: m_AlbedColorRTV(albedColorRTV)
	, m_NormalRTV(normalRTV)
	, m_SpecularRTV(specularRTV)
	, m_depthRTV(depthRTV)
	, m_MainDSV(mainDS)
	, m_DeviceContext(nullptr)
	, m_Color{ clearColor_R,clearColor_B,clearColor_G,clearColor_A }
{
}

GBuffer::~GBuffer()
{
}

bool GBuffer::Init()
{
	Renderer& renderer = Renderer::GetInstance();

	m_DeviceContext = renderer.GetDeviceContext();

	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("GBuffer::Init m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

void GBuffer::InputBegin()
{
	if (m_AlbedColorRTV == nullptr || m_NormalRTV == nullptr || m_SpecularRTV == nullptr || m_MainDSV == nullptr)
	{
		Logger::GetInstance().SetLog("GBuffer::InputBegin nullptr");
		return;
	}

	// レンダーターゲット、デプスステンシルセット
	ID3D11RenderTargetView* rendertargets[] =
	{
		m_AlbedColorRTV,
		m_NormalRTV,
		m_SpecularRTV,
		m_depthRTV,
	};
	int rendertargetnum = ARRAYSIZE(rendertargets);
	m_DeviceContext->OMSetRenderTargets(rendertargetnum, rendertargets, m_MainDSV);

	// リセット
	m_DeviceContext->ClearRenderTargetView(m_AlbedColorRTV, m_Color);
	m_DeviceContext->ClearRenderTargetView(m_NormalRTV, m_Color);
	m_DeviceContext->ClearRenderTargetView(m_SpecularRTV, m_Color);
	m_DeviceContext->ClearRenderTargetView(m_depthRTV, m_Color);
	m_DeviceContext->ClearDepthStencilView(m_MainDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GBuffer::InputEnd()
{
	// レンダーターゲットリセット
	m_DeviceContext->OMSetRenderTargets(0, NULL, NULL);
}