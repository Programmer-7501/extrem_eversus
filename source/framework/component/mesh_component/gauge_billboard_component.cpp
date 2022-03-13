
#include"../../../system/main.h"
#include "gauge_billboard_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../actor/actor.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../component/cameracomponent/camera_component.h"

GaugeBillBoardComponent::GaugeBillBoardComponent(Actor * owner)
	:ChangeableBillBoardComponent(owner)
	, m_Direction(LEFT)
	, m_Ratio(1.0f)
	, m_MaxLength(1.0f)
	, m_Length(1.0f)
	, m_StartPosition(0.0f, 0.0f, 0.0f)
{
}

GaugeBillBoardComponent::~GaugeBillBoardComponent()
{
}

void GaugeBillBoardComponent::Update()
{
	m_Length = m_MaxLength * m_Ratio;

	m_Position = m_StartPosition;

	//　継承元の更新
	ChangeableBillBoardComponent::Update();
}

void GaugeBillBoardComponent::UpdateVetexData()
{
	//頂点書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	m_DeviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	if (m_Direction == LEFT)
	{
		VERTEX_COLOR_TEXCOOD* vertex = (VERTEX_COLOR_TEXCOOD*)msr.pData;
		vertex[0] = { {- m_Length,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y} };
		vertex[1] = { { 0.0f,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y} };
		vertex[2] = { {- m_Length, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
		vertex[3] = { { 0.0f, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
	}
	else
	{
		VERTEX_COLOR_TEXCOOD* vertex = (VERTEX_COLOR_TEXCOOD*)msr.pData;
		vertex[0] = { {-0.5f,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y} };
		vertex[1] = { { 0.5f + m_Length,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y} };
		vertex[2] = { {-0.5f, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
		vertex[3] = { { 0.5f + m_Length, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };

	}
	m_DeviceContext->Unmap(m_VertexBuffer.Get(), 0);

	// 更新フラグを戻す
	m_IsUpdate = false;
}
