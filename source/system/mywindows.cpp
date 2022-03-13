

#include"main.h"

#include "mywindows.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/**
* @brief コールバック関数
*/
LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) true;

	switch (msg) {
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) {
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	case WM_ACTIVATEAPP:
	case WM_INPUT:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		break;

	case WM_CLOSE:
		if (MessageBox(hwnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void MyWindows::Uninit()
{
	// 登録から外す
	UnregisterClass(WindowData::k_ClassName, GetModuleHandle(NULL));
}

bool MyWindows::EntryWindowClass()
{
	// ウィンドウクラスを初期化
	WNDCLASSEX windowClass = {
	sizeof(WNDCLASSEX),				// 構造体のサイズ
	CS_HREDRAW | CS_VREDRAW,		// クラスのスタイル
	WndProc,						// ウィンドウプロシージャ
	0,								// 補助メモリ
	0,								// 補助メモリ
	GetModuleHandle(NULL),			// このプログラムのインスタンスハンドル
	NULL,							// アイコン画像
	LoadCursor(NULL, IDC_ARROW),	// カーソル画像
	(HBRUSH)(COLOR_WINDOW + 1),		// 背景ブラシ(背景色)
	NULL,							// メニュー名
	WindowData::k_ClassName,			// クラス名									
	NULL							// 小さいアイコン
	};

	// 構造体の登録
	if (RegisterClassEx(&windowClass) == 0)
	{
		return false;
	}

	return true;
}

bool MyWindows::Create()
{
	// ウィンドウクラスの初期化、登録
	if (EntryWindowClass() == false)
	{
		return false;
	}

	// ウィンドウスタイル
	const DWORD WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	// ウィンドウサイズの算出	
	RECT windowRect = { 0, 0, SystemData::k_ScreenWidth, SystemData::k_ScreenHeight };
	AdjustWindowRect(&windowRect, WINDOW_STYLE, FALSE);

	// ウィンドウの矩形から幅と高さを求める
	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// デスクトップのサイズを取得

	// デスクトップの矩形を取得
	RECT desktopRect;
	// 第一引数はウィンドウハンドル（パソコン画面もウィンドウ）、第一引数はパソコン画面のウィンドウを取得する関数,第二引数に詰める
	GetWindowRect(GetDesktopWindow(), &desktopRect);

	// デスクトップの幅と高さを算出
	int desktop_width = desktopRect.right - desktopRect.left;
	int desktop_height = desktopRect.bottom - desktopRect.top;

	// int c = max(a,b)は大きいほうがcに入る
	int windowX = max((desktop_width - windowWidth) / 2, 0);
	int windowY = max((desktop_height - windowHeight) / 2, 0);

	// ウィンドウの生成
	hwnd = CreateWindow(			// winAPIの関数 ウィンドウのハンドルがかえってくる、HWNDでうけとる
		WindowData::k_ClassName,				// ウィンドウ　クラス
		WindowData::k_TitleName,				// ウィンドウ　テキスト ウィンドウの上の文字列
		WINDOW_STYLE,						// ウィンドウ　スタイル  ^ WS_THICKFRAME ^ WS_MAXIMIZEBOXをつけることによりウィンドサイズ変更不可
		windowX,							// サイズと位置　ウィンドウの左上x位置
		windowY,							// サイズと位置	ウィンドウの左上ｙ位置
		windowWidth,						// サイズと位置　幅
		windowHeight,						// サイズと位置　高さ
		NULL,								// 親ウィンドウハンドル
		NULL,								// メニューハンドル
		GetModuleHandle(NULL),				// インスタンスハンドル 
		NULL								// 追加のアプリケーションデータ
	);

	// 失敗したらfalseを返す
	if (hwnd == nullptr)
	{
		return false;
	}

	// 指定のウィンドウハンドルのウィンドウを指定の方法で表示

	ShowWindow(hwnd, SW_SHOW);// ウィンドウ出力

	// 失敗したらfalseを返す
	if (UpdateWindow(hwnd) == FALSE)// ウィンドウの更新
	{
		return false;
	}

	return true;
}
