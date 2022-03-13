
#include"../../system/main.h"
#include "shadowinput.h"
#include"../renderer.h"

ShadowInput::ShadowInput(ID3D11DepthStencilView * shadowInputDSV)
	:m_ShadowInputDSV(shadowInputDSV)
{
}

ShadowInput::~ShadowInput()
{
}

bool ShadowInput::Init()
{
	Renderer& renderer = Renderer::GetInstance();

	m_DeviceContext = renderer.GetDeviceContext();

	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::Init m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

void ShadowInput::Begin()
{
	if (m_ShadowInputDSV == nullptr)
	{
		Logger::GetInstance().SetLog("GBuffer::InputBegin m_ShadowInputDSVがnullptr");
		return;
	}

	// レンダーターゲットセット
	m_DeviceContext->OMSetRenderTargets(0, NULL, m_ShadowInputDSV);

	// リセット
	m_DeviceContext->ClearDepthStencilView(m_ShadowInputDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowInput::End()
{
	// レンダーターゲットリセット
	m_DeviceContext->OMSetRenderTargets(0, NULL, NULL);
}
