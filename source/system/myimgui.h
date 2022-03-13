/**
* @file myimgui.h
* @brief imgui使用
*/

#pragma once

#include"../function/singleton.h"

/**
* @namespace MyImGuiData
* @brief ImGuiの情報
*/
namespace MyImGuiData
{
	//! デバッグウィンドウの名前
	static const char* k_DebugWindowName = "SceneInfo";
	//! デバッグウィンドウのサイズ
	static const ImVec2 k_DebugWindowSize = ImVec2(400.0f, 300.0f);

	//! 計測時間子ウィンドウの名前
	static const char* k_TimeMeasurementChilWindowName = "TimeMeasurement";
	//! 計測時間子ウィンドウのサイズ1
	static const ImVec2 k_TimeMeasurementChilWindowSize = ImVec2(400.0f, 40.0f);

	//! レンダリング結果子ウィンドウの名前
	static const char* k_RenderingResultChildWindowName = "RenderingResult";
	//! レンダリング結果子ウィンドウのサイズ
	static const ImVec2 k_RenderingResultChildWindowSize = ImVec2(400.0f, 200.0f);

	//! レンダリング結果テクスチャサイズ
	static const ImVec2 k_RenderingResultTextureSize = ImVec2(100.0f, 100.0f);
}

/**
* @brief imgui仕様クラス
*/
class MyImGui final : public Singleton<MyImGui>
{
private:
	friend class Singleton<MyImGui>;

	MyImGui() = default;
	~MyImGui() = default;

public:
	/**
	* @brief 初期化処理
	*/
	void Init();

	/**
	* @brief 終了処理
	*/
	void Uninit();
	/**
	* @brief 更新処理
	*/
	void Update();

	/**
	* @brief 描画処理
	*/
	void Draw() const;

private:
	/**
	* @brief 終了関数
	*/
	void Begin();
};
