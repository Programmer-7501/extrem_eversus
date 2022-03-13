/**
* @file mywindows.h
* @brief ウィンドウ生成機能クラス
*/

#pragma once

#include"../function/singleton.h"

/**
* @namespace WindowData
* @brief ウィンドウの情報
*/
namespace WindowData
{
	//! ウィンドウタイトルバーの名前
	static const char* k_ClassName = "AppClass";

	//! ウィンドウタイトルバーの名前
	static const char* k_TitleName = "ExtremeVersus";
}

/**
* @brief ウィンドウズ生成クラス
*/
class MyWindows : public Singleton<MyWindows>
{
private:
	//! ウィンドウハンドル
	HWND hwnd = nullptr;

private:
	friend class Singleton<MyWindows>;

	MyWindows() = default;
	~MyWindows() = default;

public:
	/**
	* @brief ウィンドウの生成
	* @return bool 生成が成功したかどうか
	*/
	bool Create();

	/**
	* @brief 終了関数
	*/
	void Uninit();

	/**
	* @brief HWND取得
	*/
	inline HWND GetHWND() const
	{
		return hwnd; 
	}

private:

	/**
	* @brief ウィンドウクラスの初期化、登録
	* @return bool 初期化が成功したかどうか
	*/
	bool EntryWindowClass();
};
