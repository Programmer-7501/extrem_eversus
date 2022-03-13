
#include"../../system/main.h"
#include "forward_rendering.h"
#include"../renderer.h"
#include"../resource/shader_manager.h"

ForwardRendering::ForwardRendering(
	ID3D11RenderTargetView * mainRTV,
	ID3D11DepthStencilView* mainDSV,
	float clearColor_R,
	float clearColor_B,
	float clearColor_G,
	float clearColor_A
)
	:m_MainRTV(mainRTV)
	,m_MainDSV(mainDSV)
	,m_Color{ clearColor_R ,clearColor_B ,clearColor_G ,clearColor_A }
{

}

ForwardRendering::~ForwardRendering()
{
}

bool ForwardRendering::Init()
{
	Renderer& renderer = Renderer::GetInstance();

	m_DeviceContext = renderer.GetDeviceContext();

	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("ForwardRendering::Init m_DeviceContext‚ªnullptr");
		return false;
	}

	return true;
}

void ForwardRendering::Begin() const
{
	if (m_MainRTV == nullptr || m_MainDSV == nullptr)
	{
		Logger::GetInstance().SetLog("ForwardRendering::Begin nullptr");
		return;
	}

	m_DeviceContext->OMSetRenderTargets(1, &m_MainRTV, m_MainDSV);
}

void ForwardRendering::End() const
{
	m_DeviceContext->OMSetRenderTargets(0, NULL, NULL);
}
