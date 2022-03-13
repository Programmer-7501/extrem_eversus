/**
 * @file main.h
 * @brief ��v�Ȏg�p���C�u�����̃C���N���[�h
 * @brief ��v�Ȓ�`
 */

#pragma once

#define _CRT_SECURE_NO_WARNINGS

// new���̃��������[�N���m�}�N��
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
 // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
 // allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// �֗����C�u����
#include<Windows.h>
#include <algorithm>
#include <wrl/client.h>
#include<vector>
#include<string>
#include<unordered_map>

// �V�X�e���f�[�^
#include"system_data.h"

// �Q�[���f�[�^
#include"../gamedata/game_data.h"

// ���͗p���C�u����
#include<Xinput.h>

#include<DirectXTex/DirectXTex.h>

// �g�p����鐔�w���C�u����
#include"../math/calculation/convenient_directxmath.h"

// ���̓N���X
#include"input_manager.h"

// ���O�o�̓N���X
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

// DirectXTex�̃��C�u�����������N

#if _DEBUG
#pragma comment(lib, "DirectXTex/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/x64/Release/DirectXTex.lib")
#endif