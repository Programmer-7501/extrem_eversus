/**
* @file fps_manager.h
* @brief fps制御
* @see https://teratail.com/questions/158503
*/

#pragma once

#include"../function/singleton.h"

/**
* @brief FPS管理クラス
*/
class FPSManager final : public Singleton<FPSManager>
{
private:
	LARGE_INTEGER m_TimeStart = { 0 };
	LARGE_INTEGER m_TimeEnd = { 0 };
	LARGE_INTEGER m_TimeFreq = { 0 };
	double m_MeasuredFPS = 0.0;
	int m_FPSViewCount = 0;
private:
	friend class Singleton<FPSManager>;

	FPSManager() = default;
	~FPSManager() = default;

public:
	/**
	* @brief 初期化
	*/
	void Init();

	/**
	* @brief 終了処理
	*/
	void Uninit();

	/**
	* @brief FPSの計算
	* @details メインループの最初に行う
	*/
	void Begin();

	/**
	* @brief ImGuiへの入力
	*/
	void InputImGui();

	/**
	* @brief 待機
	* @details メインループの最後に行う
	*/
	void End();
};
