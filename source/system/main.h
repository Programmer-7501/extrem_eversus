/**
 * @file main.h
 * @brief 主要な使用ライブラリのインクルード
 * @brief 主要な定義
 */

#pragma once

#define _CRT_SECURE_NO_WARNINGS

// new時のメモリリーク検知マクロ
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
 // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
 // allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// 便利ライブラリ
#include<Windows.h>
#include <algorithm>
#include <wrl/client.h>
#include<vector>
#include<string>
#include<unordered_map>

// システムデータ
#include"system_data.h"

// ゲームデータ
#include"../gamedata/game_data.h"

// 入力用ライブラリ
#include<Xinput.h>

#include<DirectXTex/DirectXTex.h>

// 使用される数学ライブラリ
#include"../math/calculation/convenient_directxmath.h"

// 入力クラス
#include"input_manager.h"

// ログ出力クラス
#include"../function/logger.h"

// imgui
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

// DirectX11
#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>

#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")

#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxgi.lib")

#pragma comment (lib, "Xinput.lib")

// DirectXTexのライブラリをリンク

#if _DEBUG
#pragma comment(lib, "DirectXTex/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/x64/Release/DirectXTex.lib")
#endif