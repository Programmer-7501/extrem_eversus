
#include"../system/main.h"
#include"renderer.h"
#include"render_data/sampler_data.h"
#include"../system/mywindows.h"
#include"rendering_pipline/shadowinput.h"
#include"rendering_pipline/deferred_rendering.h"
#include"rendering_pipline/output_rendering_result.h"
#include"render_data/srv_data.h"
#include"../system/myimgui.h"

/**
* @brief DirectX11の情報
*/
namespace D3DData
{
	//! フィーチャーレベル
	static D3D_FEATURE_LEVEL k_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	//! シャドウマップ画面倍率
	static int k_ShadowRatio = 4;
}

namespace RendererData
{
	//! レンダーターゲットのクリアカラー
	static const float k_RenderTargetClearColor[4] = { 0.4f,0.4f,0.4f,0.4f };
}

bool Renderer::Init()
{
	// デバイス、スワップチェーン作成
	if (CreateDeviceSwapChain() == false)
	{
		return false;
	}

	// ビューポート登録
	RegisterViewport();

	// ラスタライザー作成
	if (CreateRasterizer() == false)
	{
		return false;
	}

	// レンダーターゲット作成
	if (CreateRenderTarget() == false)
	{
		return false;
	}

	// デプスステンシル作成
	if (CreateDepthSetncil() == false)
	{
		return false;
	}

	// デプスステンシルステート作成
	if (CreateDepthStencilState() == false)
	{
		return false;
	}

	// ブレンドステート作成
	if (CreatBlendState() == false)
	{
		return false;
	}

	// サンプラーステート作成
	if (CreateSamplerState() == false)
	{
		return false;
	}

	// コンスタントバッファ作成
	if (CreateConstantBuffer() == false)
	{
		return false;
	}

	return true;
}

void Renderer::Uninit()
{
	// レンダリングパイプラインの破棄
	UnloadRenderingPipline();
}

void Renderer::DrawBegin()
{
	if (m_RenderTargetView[RENDERTARGET_BACKBUFFER] == nullptr || m_DepthStencilView[DEPTHSTENCIL_BACKBUFFER] == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::DrawBegin nullptr");
		return;
	}

	// レンダーターゲット、デプスステンシルを登録
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView[RENDERTARGET_BACKBUFFER].GetAddressOf(), m_DepthStencilView[DEPTHSTENCIL_BACKBUFFER].Get());
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView[RENDERTARGET_BACKBUFFER].Get(), RendererData::k_RenderTargetClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView[DEPTHSTENCIL_BACKBUFFER].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::DrawEnd()
{
	m_SwapChain->Present(1, 0);
}

void Renderer::ShadowInputStart()
{
	if (m_DepthStencilView[DEPTHSTENCIL_SHADOWMAP] == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::ShadowInputStart m_DepthStencilViewがnullptr");
		return;
	}
	m_DeviceContext->OMSetRenderTargets(0, NULL, m_DepthStencilView[DEPTHSTENCIL_SHADOWMAP].Get());
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView[DEPTHSTENCIL_SHADOWMAP].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ビューポート設定変更
	D3D11_VIEWPORT viewport = {};
	viewport.Width = (FLOAT)SystemData::k_ScreenWidth * D3DData::k_ShadowRatio;
	viewport.Height = (FLOAT)SystemData::k_ScreenHeight * D3DData::k_ShadowRatio;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	SetViewPort(viewport);
}

void Renderer::ShadowInputEnd()
{
	// ビューポート設定戻す
	D3D11_VIEWPORT viewport = {};
	viewport.Width = (FLOAT)SystemData::k_ScreenWidth;
	viewport.Height = (FLOAT)SystemData::k_ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	SetViewPort(viewport);

	// レンダーターゲットから影入力用デプスステンシルを外す
	m_DeviceContext->OMSetRenderTargets(0, NULL, NULL);
	// ピクセルシェーダーに影入力用シェーダーリソースビューを登録
	m_DeviceContext->PSSetShaderResources(SRVData::k_ShadowMapShaderResourceViewIndex, 1, m_DepthStencil_ShaderResourceView[DEPTHSTENCIL_SHADOWMAP].GetAddressOf());

	ImGui::Begin(MyImGuiData::k_DebugWindowName);
	ImGui::BeginChild(MyImGuiData::k_RenderingResultChildWindowName, MyImGuiData::k_RenderingResultChildWindowSize, true);
	ImGui::Text("ShadowDepth");
	ImGui::Image((void*)m_DepthStencil_ShaderResourceView[DEPTHSTENCIL_SHADOWMAP].Get(), MyImGuiData::k_RenderingResultTextureSize);
	ImGui::EndChild();
	ImGui::End();
}

void Renderer::GBufferInputStart()
{
	if (m_RenderTargetView[RENDERTARGET_ALBEDCOLOR] == nullptr ||
		m_RenderTargetView[RENDERTARGET_NORMAL] == nullptr ||
		m_RenderTargetView[RENDERTARGET_SPECULAR] == nullptr || 
		m_DepthStencilView[DEPTHSTENCIL_MAIN] == nullptr)
	{
		Logger::GetInstance().SetLog("GBuffer::InputBegin nullptr");
		return;
	}

	// レンダーターゲット、デプスステンシルセット
	ID3D11RenderTargetView* rendertargets[] =
	{
		m_RenderTargetView[RENDERTARGET_ALBEDCOLOR].Get(),
		m_RenderTargetView[RENDERTARGET_NORMAL].Get(),
		m_RenderTargetView[RENDERTARGET_SPECULAR].Get(),
	};
	UINT rendertargetnum = ARRAYSIZE(rendertargets);
	m_DeviceContext->OMSetRenderTargets(rendertargetnum, rendertargets, m_DepthStencilView[DEPTHSTENCIL_MAIN].Get());

	// リセット
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView[RENDERTARGET_ALBEDCOLOR].Get(), RendererData::k_RenderTargetClearColor);
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView[RENDERTARGET_NORMAL].Get(), RendererData::k_RenderTargetClearColor);
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView[RENDERTARGET_SPECULAR].Get(), RendererData::k_RenderTargetClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView[DEPTHSTENCIL_MAIN].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::GBufferInputEnd()
{
}

void Renderer::ExcuteDeferredRendering()
{
	if (m_RenderTargetView[RENDERTARGET_MAIN] == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::ExcuteDeferredRendering nullptr");
		return;
	}
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView[RENDERTARGET_MAIN].GetAddressOf(), NULL);

	m_DeferredRendering->Draw();
}

void Renderer::ForwardRenderingStart()
{
	if (m_RenderTargetView[RENDERTARGET_MAIN] == nullptr || m_DepthStencilView[DEPTHSTENCIL_MAIN] == nullptr)
	{
		Logger::GetInstance().SetLog("ForwardRendering::Begin nullptr");
		return;
	}

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView[RENDERTARGET_MAIN].GetAddressOf(), m_DepthStencilView[DEPTHSTENCIL_MAIN].Get());
}

void Renderer::ForwardRenderingEnd()
{
}

void Renderer::ExcuteOutPutRenderingResult()
{
	if (m_RenderTargetView[RENDERTARGET_BACKBUFFER] == nullptr || m_DepthStencilView[DEPTHSTENCIL_BACKBUFFER] == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::DrawBegin nullptr");
		return;
	}

	// レンダーターゲット、デプスステンシルを登録
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView[RENDERTARGET_BACKBUFFER].GetAddressOf(), m_DepthStencilView[DEPTHSTENCIL_BACKBUFFER].Get());
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView[RENDERTARGET_BACKBUFFER].Get(), RendererData::k_RenderTargetClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView[DEPTHSTENCIL_BACKBUFFER].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_OutPutRenderingResult->Draw();
}

void Renderer::SetWorldViewProjection2D()
{
	// ワールド行列セット
	Conv_XM::Matrix4x4f world;
	world = DirectX::XMMatrixIdentity();
	SetWorldMatrix(world);

	// ビュープロジェクションセット
	Conv_XM::Matrix4x4f view, projection;
	view = DirectX::XMMatrixIdentity();
	projection = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, SystemData::k_ScreenWidth, SystemData::k_ScreenHeight, 0.0f, 0.0f, 1.0f);
	SetViewProjectionMatrix(view, projection);
}

void Renderer::SetWorldMatrix(const Conv_XM::Matrix4x4f & worldMatrix)
{
	Conv_XM::Matrix4x4f Transpose = DirectX::XMMatrixTranspose(worldMatrix);
	if (m_WorldMatrixConstantBuffer)
	{
		m_DeviceContext->UpdateSubresource(m_WorldMatrixConstantBuffer.Get(), 0, NULL, &Transpose, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("ワールド行列バッファnullptr");
	}
}

void Renderer::SetDrawData(const CBufferData::DrawData & cameraData)
{
	m_DrawData.View = DirectX::XMMatrixTranspose(cameraData.View);
	m_DrawData.Projection = DirectX::XMMatrixTranspose(cameraData.Projection);
	m_DrawData.CameraViewProjectionInverse = DirectX::XMMatrixTranspose(cameraData.CameraViewProjectionInverse);
	m_DrawData.CameraPosition = cameraData.CameraPosition;
	m_DrawData.RandomNumber = Math::GenerateZeroToOneRand();

	if (m_DrawDataConstantBuffer)
	{
		m_DeviceContext->UpdateSubresource(m_DrawDataConstantBuffer.Get(), 0, NULL, &m_DrawData, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("描画データバッファnullptr");
	}
}

void Renderer::SetViewProjectionMatrix(const Conv_XM::Matrix4x4f & viewMatrix, const Conv_XM::Matrix4x4f & projectionMatrix)
{
	if (m_DrawDataConstantBuffer)
	{
		m_DrawData.View = DirectX::XMMatrixTranspose(viewMatrix);
		m_DrawData.Projection = DirectX::XMMatrixTranspose(projectionMatrix);
		m_DeviceContext->UpdateSubresource(m_DrawDataConstantBuffer.Get(), 0, NULL, &m_DrawData, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("描画データバッファnullptr");
	}
}

void Renderer::Set3DCameraViewProjectionInverse(const Conv_XM::Matrix4x4f & viewProjectionInverse)
{
	if (m_DrawDataConstantBuffer)
	{
		m_DrawData.CameraViewProjectionInverse = DirectX::XMMatrixTranspose(viewProjectionInverse);
		m_DeviceContext->UpdateSubresource(m_DrawDataConstantBuffer.Get(), 0, NULL, &m_DrawData, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("描画データバッファnullptr");
	}
}

void Renderer::SetCameraPositon(const Conv_XM::Vector4f & cameraPosition)
{
	if (m_DrawDataConstantBuffer)
	{
		m_DrawData.CameraPosition = cameraPosition;
		m_DeviceContext->UpdateSubresource(m_DrawDataConstantBuffer.Get(), 0, NULL, &m_DrawData, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("描画データバッファnullptr");
	}
}

void Renderer::SetRandomNumber(float randomNum)
{
	if (m_DrawDataConstantBuffer)
	{
		m_DrawData.RandomNumber = randomNum;
		m_DeviceContext->UpdateSubresource(m_DrawDataConstantBuffer.Get(), 0, NULL, &m_DrawData, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("描画データバッファnullptr");
	}
}

void Renderer::SetDirectionalLight(const CBufferData::DirectionalLight & directionalLight)
{
	if (m_DirectionalLightConstantBuffer)
	{
		m_DeviceContext->UpdateSubresource(m_DirectionalLightConstantBuffer.Get(), 0, NULL, &directionalLight, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("ディレクショナルライトバッファnullptr");
	}
}

void Renderer::SetPointLight(const CBufferData::PointLight & pointLight)
{
	if (m_PointLightConstantBuffer)
	{
		m_DeviceContext->UpdateSubresource(m_PointLightConstantBuffer.Get(), 0, NULL, &pointLight, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("ポイントライトバッファnullptr");
	}
}

void Renderer::SetMaterial(const CBufferData::Material & material)
{
	if (m_MaterialConstantBuffer)
	{
		m_DeviceContext->UpdateSubresource(m_MaterialConstantBuffer.Get(), 0, NULL, &material, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("マテリアルバッファnullptr");
	}
}

void Renderer::SetLightViewProjection(const Conv_XM::Matrix4x4f & lightViewProjection)
{
	if (m_LightViewProjectionConstantBuffer)
	{
		// テクスチャバイアス.
		DirectX::XMMATRIX SHADOW_BIAS = DirectX::XMMATRIX(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f);

		CBufferData::ShadowData shadowData;
		shadowData.ShadowBiasMatrix = DirectX::XMMatrixTranspose(SHADOW_BIAS);
		shadowData.LightViewProjection = DirectX::XMMatrixTranspose(lightViewProjection);

		m_DeviceContext->UpdateSubresource(m_LightViewProjectionConstantBuffer.Get(), 0, NULL, &shadowData, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("ライトビュープロジェクションnullptr");
	}
}

void Renderer::SetBones(const CBufferData::BONES & bones)
{
	if (m_BonesConstantBuffer)
	{
		m_DeviceContext->UpdateSubresource(m_BonesConstantBuffer.Get(), 0, NULL, &bones, 0, 0);
	}
	else
	{
		Logger::GetInstance().SetLog("ボーンバッファnullptr");
	}
}

void Renderer::SetBlendState(BlendStateName blendStateName)
{
	if (m_BlendState[blendStateName] == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::SetBlendState m_BlendStateがnullptr");
		return;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->OMSetBlendState(m_BlendState[blendStateName].Get(), blendFactor, 0xffffffff);
}

void Renderer::SetDepthStencilState(DepthStencilStateName depthStencilStateName)
{
	if (m_DepthState[depthStencilStateName])
	{
		m_DeviceContext->OMSetDepthStencilState(m_DepthState[depthStencilStateName].Get(), NULL);
	}
	else
	{
		Logger::GetInstance().SetLog("Renderer::SetDepthStencilState m_DepthState[depthStencilStateName]がnullptr");
		return;
	}
}

void Renderer::SetViewPort(D3D11_VIEWPORT viewPort)
{
	m_DeviceContext->RSSetViewports(1, &viewPort);
}

bool Renderer::CreateDeviceSwapChain()
{
	HRESULT hr;

	//HWND取得
	HWND hwnd = MyWindows::GetInstance().GetHWND();
	if (hwnd == nullptr)
	{
		Logger::GetInstance().SetLog("renderer.h : hwnd nullptr");
		return false;
	}
	
	// スワップチェーン情報入力
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SystemData::k_ScreenWidth;
	swapChainDesc.BufferDesc.Height = SystemData::k_ScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;// MSAAにするなら16程度
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// デバイス、スワップチェーン作成
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		m_SwapChain.GetAddressOf(),
		m_Device.GetAddressOf(),
		&D3DData::k_FeatureLevel,
		m_DeviceContext.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}
	
	return true;
}

void Renderer::RegisterViewport()
{
	// ビューポート設定
	D3D11_VIEWPORT viewport = {};
	viewport.Width = (FLOAT)SystemData::k_ScreenWidth;
	viewport.Height = (FLOAT)SystemData::k_ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &viewport);
}

bool Renderer::CreateRasterizer()
{
	HRESULT hr;

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	hr = m_Device->CreateRasterizerState(&rasterizerDesc, m_RasterizerState.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	m_DeviceContext->RSSetState(m_RasterizerState.Get());

	return true;
}

bool Renderer::CreateRenderTarget()
{
	//---------------------------------------------------------------------------------------------
	//      バックバッファレンダーターゲットの作成
	//---------------------------------------------------------------------------------------------
	HRESULT hr;

	// バックバッファ使用時はスワップチェインと紐づけ
	if (m_SwapChain == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget swapChainがnullptr");
		return false;
	}

	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_RenderTarget_LayerBuffer[RENDERTARGET_BACKBUFFER].GetAddressOf()));

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget GetBufferが失敗");
		return false;
	}

	// バックバッファのレンダーターゲットを作成
	hr = m_Device->CreateRenderTargetView(m_RenderTarget_LayerBuffer[RENDERTARGET_BACKBUFFER].Get(), nullptr, m_RenderTargetView[RENDERTARGET_BACKBUFFER].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateRenderTargetViewが失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      アルベドカラーレンダーターゲットの作成
	//---------------------------------------------------------------------------------------------
	// テクスチャの設定
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = SystemData::k_ScreenWidth;
	texDesc.Height = SystemData::k_ScreenHeight;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	// テクスチャ作成
	hr = m_Device->CreateTexture2D(&texDesc, nullptr, m_RenderTarget_LayerBuffer[RENDERTARGET_ALBEDCOLOR].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateTexture2Dが失敗");
		return false;
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// レンダーターゲットビューを作成
	hr = m_Device->CreateRenderTargetView(m_RenderTarget_LayerBuffer[RENDERTARGET_ALBEDCOLOR].Get(), &rtvDesc, m_RenderTargetView[RENDERTARGET_ALBEDCOLOR].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateRenderTargetViewが失敗");
		return false;
	}

	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// シェーダリソースビューを作成
	hr = m_Device->CreateShaderResourceView(m_RenderTarget_LayerBuffer[RENDERTARGET_ALBEDCOLOR].Get(), &srvDesc, m_RenderTarget_ShaderResourceView[RENDERTARGET_ALBEDCOLOR].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateShaderResourceViewが失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      ノーマルレンダーターゲットの作成
	//---------------------------------------------------------------------------------------------
	// テクスチャの設定
	texDesc = {};
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = SystemData::k_ScreenWidth;
	texDesc.Height = SystemData::k_ScreenHeight;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	// テクスチャの作成
	hr = m_Device->CreateTexture2D(&texDesc, nullptr, m_RenderTarget_LayerBuffer[RENDERTARGET_NORMAL].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateTexture2Dが失敗");
		return false;
	}

	// レンダーターゲットビューの設定
	rtvDesc = {};
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// レンダーターゲットビューを作成
	hr = m_Device->CreateRenderTargetView(m_RenderTarget_LayerBuffer[RENDERTARGET_NORMAL].Get(), &rtvDesc, m_RenderTargetView[RENDERTARGET_NORMAL].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateRenderTargetViewが失敗");
		return false;
	}

	// シェーダリソースビューの設定
	srvDesc = {};
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// シェーダリソースビューを作成
	hr = m_Device->CreateShaderResourceView(m_RenderTarget_LayerBuffer[RENDERTARGET_NORMAL].Get(), &srvDesc, m_RenderTarget_ShaderResourceView[RENDERTARGET_NORMAL].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateShaderResourceViewが失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      スぺキュラレンダーターゲットの作成
	//---------------------------------------------------------------------------------------------
		// テクスチャの設定
	texDesc = {};
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = SystemData::k_ScreenWidth;
	texDesc.Height = SystemData::k_ScreenHeight;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	// テクスチャの作成
	hr = m_Device->CreateTexture2D(&texDesc, nullptr, m_RenderTarget_LayerBuffer[RENDERTARGET_SPECULAR].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateTexture2Dが失敗");
		return false;
	}

	// レンダーターゲットビューの設定
	rtvDesc = {};
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// レンダーターゲットビューを作成
	hr = m_Device->CreateRenderTargetView(m_RenderTarget_LayerBuffer[RENDERTARGET_SPECULAR].Get(), &rtvDesc, m_RenderTargetView[RENDERTARGET_SPECULAR].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateRenderTargetViewが失敗");
		return false;
	}

	// シェーダリソースビューの設定
	srvDesc = {};
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// シェーダリソースビューを作成
	hr = m_Device->CreateShaderResourceView(m_RenderTarget_LayerBuffer[RENDERTARGET_SPECULAR].Get(), &srvDesc, m_RenderTarget_ShaderResourceView[RENDERTARGET_SPECULAR].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderTarget CreateShaderResourceViewが失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      メインレンダーターゲットの作成
	//---------------------------------------------------------------------------------------------
		// テクスチャの設定
	texDesc = {};
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = SystemData::k_ScreenWidth;
	texDesc.Height = SystemData::k_ScreenHeight;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	// テクスチャの作成
	hr = m_Device->CreateTexture2D(&texDesc, nullptr, m_RenderTarget_LayerBuffer[RENDERTARGET_MAIN].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("RenderTarget::MainRTCreate CreateTexture2Dが失敗");
		return false;
	}

	// レンダーターゲットビューの設定
	rtvDesc = {};
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// レンダーターゲットビューを作成
	hr = m_Device->CreateRenderTargetView(m_RenderTarget_LayerBuffer[RENDERTARGET_MAIN].Get(), &rtvDesc, m_RenderTargetView[RENDERTARGET_MAIN].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("RenderTarget::MainRTCreate CreateRenderTargetViewが失敗");
		return false;
	}

	// シェーダリソースビューの設定
	srvDesc = {};
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// シェーダリソースビューを作成
	hr = m_Device->CreateShaderResourceView(m_RenderTarget_LayerBuffer[RENDERTARGET_MAIN].Get(), &srvDesc, m_RenderTarget_ShaderResourceView[RENDERTARGET_MAIN].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("RenderTarget::MainRTCreate CreateShaderResourceViewが失敗");
		return false;
	}

	return true;
}

bool Renderer::CreateDepthSetncil()
{
	HRESULT hr;
	//---------------------------------------------------------------------------------------------
	//      バックバッファデプスステンシルの作成
	//---------------------------------------------------------------------------------------------

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	hr = m_SwapChain->GetDesc(&swapChainDesc);
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil GetDescが失敗");
		return false;
	}

	// テクスチャの設定
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	// テクスチャの作成
	hr = m_Device->CreateTexture2D(&textureDesc, NULL, m_DepthStencil_LayerBuffer[DEPTHSTENCIL_BACKBUFFER].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateTexture2Dが失敗");
		return false;
	}

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	hr = m_Device->CreateDepthStencilView(m_DepthStencil_LayerBuffer[DEPTHSTENCIL_BACKBUFFER].Get(), &depthStencilViewDesc, m_DepthStencilView[DEPTHSTENCIL_BACKBUFFER].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateDepthStencilViewが失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      シャドウマップデプスステンシルの作成
	//---------------------------------------------------------------------------------------------
		// デプスステンシルバッファ作成
	textureDesc = {};
	textureDesc.Width = SystemData::k_ScreenWidth * D3DData::k_ShadowRatio;
	textureDesc.Height = SystemData::k_ScreenHeight * D3DData::k_ShadowRatio;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	hr = m_Device->CreateTexture2D(&textureDesc, NULL, m_DepthStencil_LayerBuffer[DEPTHSTENCIL_SHADOWMAP].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateTexture2Dが失敗");
		return false;
	}

	// デプスステンシルビュー作成
	depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(m_DepthStencil_LayerBuffer[DEPTHSTENCIL_SHADOWMAP].Get(), &depthStencilViewDesc, m_DepthStencilView[DEPTHSTENCIL_SHADOWMAP].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateDepthStencilViewが失敗");
		return false;
	}


	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC depthsrvDesc = {};
	ZeroMemory(&depthsrvDesc, sizeof(depthsrvDesc));
	depthsrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depthsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthsrvDesc.Texture2D.MipLevels = 1;

	// シェーダリソースビューを作成
	hr = m_Device->CreateShaderResourceView(m_DepthStencil_LayerBuffer[DEPTHSTENCIL_SHADOWMAP].Get(), &depthsrvDesc, m_DepthStencil_ShaderResourceView[DEPTHSTENCIL_SHADOWMAP].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateShaderResourceViewが失敗");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      メインデプスステンシルの作成
	//---------------------------------------------------------------------------------------------
		// デプスステンシルバッファ作成
	textureDesc = {};
	textureDesc.Width = SystemData::k_ScreenWidth;
	textureDesc.Height = SystemData::k_ScreenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	hr = m_Device->CreateTexture2D(&textureDesc, NULL, m_DepthStencil_LayerBuffer[DEPTHSTENCIL_MAIN].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateTexture2Dが失敗");
		return false;
	}

	// デプスステンシルビュー作成
	depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(m_DepthStencil_LayerBuffer[DEPTHSTENCIL_MAIN].Get(), &depthStencilViewDesc, m_DepthStencilView[DEPTHSTENCIL_MAIN].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateDepthStencilViewが失敗");
		return false;
	}

	// シェーダリソースビューの設定
	depthsrvDesc = {};
	ZeroMemory(&depthsrvDesc, sizeof(depthsrvDesc));
	depthsrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depthsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthsrvDesc.Texture2D.MipLevels = 1;

	// シェーダリソースビューを作成
	hr = m_Device->CreateShaderResourceView(m_DepthStencil_LayerBuffer[DEPTHSTENCIL_MAIN].Get(), &depthsrvDesc, m_DepthStencil_ShaderResourceView[DEPTHSTENCIL_MAIN].GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("Renderer::CreateDepthSetncil CreateShaderResourceViewが失敗");
		return false;
	}

	return true;
}

bool Renderer::CreateDepthStencilState()
{
	HRESULT hr;

	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	//深度値を参照
	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthState[DEPTHSTENCILSTATE_STANDARD].GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	// 深度値を書き込まない
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthState[DEPTHSTENCILSTATE_NOWRITE].GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	// 深度値を無効にする
	depthStencilDesc.DepthEnable = FALSE;
	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthState[DEPTHSTENCILSTATE_NOWRITE_REFERENCE].GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	//初期設定、深度値有効
	m_DeviceContext->OMSetDepthStencilState(m_DepthState[DEPTHSTENCILSTATE_STANDARD].Get(), NULL);

	return true;
}

bool Renderer::CreatBlendState()
{
	HRESULT hr;


	//---------------------------------------------------------------------------------------------
	//      デフォルトのブレンドステート
	//---------------------------------------------------------------------------------------------

	// ブレンドの設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;


	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Dest=基本色(下レイヤー), Src=合成色(上レイヤー)
	// 通常(アルファブレンド)
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	// Src * SrcA + Dest * (1 - SrcA)
	// ブレンドステートを作成
	hr = m_Device->CreateBlendState(&blendDesc, m_BlendState[BLENDSTATE_DEFAULT].GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//ブレンドステート登録
	m_DeviceContext->OMSetBlendState(m_BlendState[BLENDSTATE_DEFAULT].Get(), blendFactor, 0xffffffff);


	//---------------------------------------------------------------------------------------------
	//      透過あり加算のブレンドステート
	//---------------------------------------------------------------------------------------------

	// 加算(透過あり)
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	// Src * SrcA + Dest * 1
	hr = m_Device->CreateBlendState(&blendDesc, m_BlendState[BLENDSTATE_INVISIBLEADD].GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Renderer::CreateSamplerState()
{
	HRESULT hr;

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MaxAnisotropy = 16;

	// サンプラーステート作成
	hr = m_Device->CreateSamplerState(&samplerDesc, m_SamplerState[SamplerData::DEFAULTSAMPLER_INDEX].GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// ピクセルシェーダーにサンプラーステート登録
	m_DeviceContext->PSSetSamplers(SamplerData::DEFAULTSAMPLER_INDEX, 1, m_SamplerState[SamplerData::DEFAULTSAMPLER_INDEX].GetAddressOf());

	// シャドウマップ用サンプラーステート設定
	D3D11_SAMPLER_DESC shadowsamplerDesc{};
	shadowsamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowsamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowsamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowsamplerDesc.BorderColor[0] = 1.0f;
	shadowsamplerDesc.BorderColor[1] = 1.0f;
	shadowsamplerDesc.BorderColor[2] = 1.0f;
	shadowsamplerDesc.BorderColor[3] = 1.0f;
	shadowsamplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER;
	shadowsamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowsamplerDesc.MaxAnisotropy = 1;
	shadowsamplerDesc.MipLODBias = 0;
	shadowsamplerDesc.MinLOD = -FLT_MAX;
	shadowsamplerDesc.MaxLOD = +FLT_MAX;

	hr = m_Device->CreateSamplerState(&shadowsamplerDesc, m_SamplerState[SamplerData::SHADOWMAPSAMPLER_INDEX].GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// ピクセルシェーダーにサンプラーステート登録
	m_DeviceContext->PSSetSamplers(SamplerData::SHADOWMAPSAMPLER_INDEX, 1, m_SamplerState[SamplerData::SHADOWMAPSAMPLER_INDEX].GetAddressOf());

	return true;
}

bool Renderer::CreateConstantBuffer()
{
	HRESULT hr;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(Conv_XM::Matrix4x4f);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	// ワールド行列バッファ作成
	hr = m_Device->CreateBuffer(&bufferDesc, NULL, m_WorldMatrixConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	// 頂点シェーダーにワールド行列バッファを登録
	m_DeviceContext->VSSetConstantBuffers(CBufferData::WORLDMATRIX_INDEX, 1, m_WorldMatrixConstantBuffer.GetAddressOf());

	// 描画データバッファ作成
	bufferDesc.ByteWidth = sizeof(CBufferData::DrawData);
	hr = m_Device->CreateBuffer(&bufferDesc, NULL, m_DrawDataConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	// ビュー行列バッファを登録
	m_DeviceContext->CSSetConstantBuffers(CBufferData::DRAWDATA_INDEX, 1, m_DrawDataConstantBuffer.GetAddressOf());
	m_DeviceContext->VSSetConstantBuffers(CBufferData::DRAWDATA_INDEX, 1, m_DrawDataConstantBuffer.GetAddressOf());
	m_DeviceContext->PSSetConstantBuffers(CBufferData::DRAWDATA_INDEX, 1, m_DrawDataConstantBuffer.GetAddressOf());

	// ディレクショナルライトバッファを作成
	bufferDesc.ByteWidth = sizeof(CBufferData::DirectionalLight);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, m_DirectionalLightConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	// ピクセルシェーダーにディレクショナルライトバッファを登録
	m_DeviceContext->PSSetConstantBuffers(CBufferData::DIRECTIONALLIGHT_INDEX, 1, m_DirectionalLightConstantBuffer.GetAddressOf());

	// ポイントライトバッファを作成
	bufferDesc.ByteWidth = sizeof(CBufferData::PointLight);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, m_PointLightConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	// ピクセルシェーダーにポイントライトバッファを登録
	m_DeviceContext->PSSetConstantBuffers(CBufferData::POINTLIGHT_INDEX, 1, m_PointLightConstantBuffer.GetAddressOf());

	// マテリアルバッファを作成
	bufferDesc.ByteWidth = sizeof(CBufferData::Material);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, m_MaterialConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	// ピクセルシェーダーにマテリアルバッファを登録
	m_DeviceContext->PSSetConstantBuffers(CBufferData::MATERIAL_INDEX, 1, m_MaterialConstantBuffer.GetAddressOf());

	// ライトビュープロジェクションバッファを作成
	bufferDesc.ByteWidth = sizeof(CBufferData::ShadowData);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, m_LightViewProjectionConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	// ピクセルシェーダーにライトビュープロジェクションバッファを登録
	m_DeviceContext->PSSetConstantBuffers(CBufferData::LIGHTVIEWPROJECTION_INDEX, 1, m_LightViewProjectionConstantBuffer.GetAddressOf());
	
	// ボーンバッファを作成
	bufferDesc.ByteWidth = sizeof(CBufferData::BONES);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, m_BonesConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	// 頂点シェーダーにボーンバッファを登録
	m_DeviceContext->VSSetConstantBuffers(CBufferData::BONE_INDEX, 1, m_BonesConstantBuffer.GetAddressOf());

	

	//--------------------初期化-------------------------//

	// 描画データ
	m_DrawData = {};
	SetDrawData(m_DrawData);

	// ディレクショナルライト
	CBufferData::DirectionalLight directionalLight = {};
	directionalLight.Ambient = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
	directionalLight.Direction = Conv_XM::Vector3f(0.5f, -0.8f, 0.0f);
	directionalLight.Direction = DirectX::XMVector3Normalize(directionalLight.Direction);
	directionalLight.Color = Conv_XM::Vector3f(1.0f, 1.0f, 1.0f);
	SetDirectionalLight(directionalLight);

	// ポイントライト
	CBufferData::PointLight pointLight = {};
	SetPointLight(pointLight);

	// マテリアル
	CBufferData::Material material = {};
	material.Ambient = Conv_XM::Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
	material.Diffuse = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	material.Metaric = 0.5f;
	material.Smoothness = 0.5f;
	SetMaterial(material);

	// ボーン
	CBufferData::BONES bone;

	for (int i = 0; i < MAX_BONE_NUM; i++)
	{
		bone.bones[i] = DirectX::XMMatrixIdentity();
	}

	SetBones(bone);

	return true;
}

bool Renderer::CreateRenderingPipline()
{
	// ディファードレンダリングクラス作成
	m_DeferredRendering = new DeferredRendering(
		m_RenderTarget_ShaderResourceView[RENDERTARGET_ALBEDCOLOR].Get(),
		m_RenderTarget_ShaderResourceView[RENDERTARGET_NORMAL].Get(),
		m_RenderTarget_ShaderResourceView[RENDERTARGET_SPECULAR].Get(),
		m_DepthStencil_ShaderResourceView[DEPTHSTENCIL_MAIN].Get(),
		RendererData::k_RenderTargetClearColor[0],
		RendererData::k_RenderTargetClearColor[1],
		RendererData::k_RenderTargetClearColor[2],
		RendererData::k_RenderTargetClearColor[3]
	);
	if (m_DeferredRendering == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderingPipline m_DeferredRenderingがnullptr");
		return false;
	}
	if (m_DeferredRendering->Init() == false)
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderingPipline m_DeferredRendering->Initが失敗");
		return false;
	}


	// レンダーター結果表示
	m_OutPutRenderingResult = new OutPutRenderingResult(
		m_RenderTarget_ShaderResourceView[RENDERTARGET_MAIN].Get(),
		RendererData::k_RenderTargetClearColor[0],
		RendererData::k_RenderTargetClearColor[1],
		RendererData::k_RenderTargetClearColor[2],
		RendererData::k_RenderTargetClearColor[3]
	);
	if (m_OutPutRenderingResult == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderingPipline m_OutPutRenderingResultがnullptr");
		return false;
	}
	if (m_OutPutRenderingResult->Init() == false)
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderingPipline m_OutPutRenderingResult->Initが失敗");
		return false;
	}


	if (m_DeferredRendering == nullptr)
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderingPipline m_DeferredRenderingがnullptr");
		return false;
	}
	if (m_DeferredRendering->Init() == false)
	{
		Logger::GetInstance().SetLog("Renderer::CreateRenderingPipline m_DeferredRendering->Initが失敗");
		return false;
	}

	return false;
}

void Renderer::UnloadRenderingPipline()
{
	if (m_DeferredRendering)
	{
		delete m_DeferredRendering;
		m_DeferredRendering = nullptr;
	}
	if (m_OutPutRenderingResult)
	{
		delete m_OutPutRenderingResult;
		m_OutPutRenderingResult = nullptr;
	}
}
