
#include"main.h"
#include "fps_manager.h"
#include"myimgui.h"

/**
* @namespace FPSManagerData
* @brief FPSの管理情報
*/
namespace FPSManagerData
{
	//! FPSを計測するフレーム
	static constexpr int k_FPSMesureFrame = 60;
}

void FPSManager::Init()
{
	// 精度を取得
	QueryPerformanceFrequency(&m_TimeFreq);

	// タイマーの最小精度を1msecにする
	timeBeginPeriod(1);
}

void FPSManager::Uninit()
{
	// タイマーの最小精度を戻す
	timeEndPeriod(1);
}

void FPSManager::Begin()
{
	//計測開始
	QueryPerformanceCounter(&m_TimeStart);
}

void FPSManager::InputImGui()
{
	ImGui::Begin(MyImGuiData::k_DebugWindowName);
	ImGui::Text("FPS : %f", m_MeasuredFPS);
	ImGui::End();
}

void FPSManager::End()
{
	//計測終了
	QueryPerformanceCounter(&m_TimeEnd);

	LONGLONG delta = m_TimeEnd.QuadPart - m_TimeStart.QuadPart;

	//処理の所要時間
	double deltaTime = static_cast<double>(delta) / static_cast<double>(m_TimeFreq.QuadPart);

	//待機時間
	double restTime = (1.0 / static_cast<double>(SystemData::k_FPS)) - deltaTime;

	if (restTime > 0.0f)
	{
		DWORD sleepTime = static_cast<DWORD>(restTime * 1000.0);
		Sleep(sleepTime);
	}

	// FPS計測
	if (m_FPSViewCount >= FPSManagerData::k_FPSMesureFrame)
	{
		LARGE_INTEGER loopEnd;
		//1ループ当たりの時間を算出
		QueryPerformanceCounter(&loopEnd);
		delta = loopEnd.QuadPart - m_TimeStart.QuadPart;
		deltaTime = (double)delta / (double)m_TimeFreq.QuadPart;

		m_MeasuredFPS = 1.0 / deltaTime;

		// カウントリセット
		m_FPSViewCount = 0;
	}

	// カウントアップ
	m_FPSViewCount++;
}