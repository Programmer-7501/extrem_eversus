
#include"main.h"
#include "myimgui.h"
#include"mywindows.h"
#include"../directx/renderer.h"

void MyImGui::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//imguiのスタイル
	//ImGui::StyleColorsLight();
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// HWND取得
	HWND hwnd = MyWindows::GetInstance().GetHWND();
	if (hwnd == nullptr)
	{
		Logger::GetInstance().SetLog("MyImGui::Init hwndがnullptr");
	}
	ImGui_ImplWin32_Init(hwnd);

	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance().GetDeviceContext();
	if (device == nullptr || deviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("MyImGui::Init device,deviceContextがnullptr");
	}
	ImGui_ImplDX11_Init(device, deviceContext);

	//iniを生成しないように
	io.IniFilename = NULL;
	//日本語フォントに対応
	//ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());
}

void MyImGui::Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void MyImGui::Update()
{
	Begin();

	ImGui::SetWindowSize(MyImGuiData::k_DebugWindowSize);
}

void MyImGui::Draw() const
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void MyImGui::Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
