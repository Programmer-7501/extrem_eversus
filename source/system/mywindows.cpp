

#include"main.h"

#include "mywindows.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/**
* @brief �R�[���o�b�N�֐�
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
		if (MessageBox(hwnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
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
	// �o�^����O��
	UnregisterClass(WindowData::k_ClassName, GetModuleHandle(NULL));
}

bool MyWindows::EntryWindowClass()
{
	// �E�B���h�E�N���X��������
	WNDCLASSEX windowClass = {
	sizeof(WNDCLASSEX),				// �\���̂̃T�C�Y
	CS_HREDRAW | CS_VREDRAW,		// �N���X�̃X�^�C��
	WndProc,						// �E�B���h�E�v���V�[�W��
	0,								// �⏕������
	0,								// �⏕������
	GetModuleHandle(NULL),			// ���̃v���O�����̃C���X�^���X�n���h��
	NULL,							// �A�C�R���摜
	LoadCursor(NULL, IDC_ARROW),	// �J�[�\���摜
	(HBRUSH)(COLOR_WINDOW + 1),		// �w�i�u���V(�w�i�F)
	NULL,							// ���j���[��
	WindowData::k_ClassName,			// �N���X��									
	NULL							// �������A�C�R��
	};

	// �\���̂̓o�^
	if (RegisterClassEx(&windowClass) == 0)
	{
		return false;
	}

	return true;
}

bool MyWindows::Create()
{
	// �E�B���h�E�N���X�̏������A�o�^
	if (EntryWindowClass() == false)
	{
		return false;
	}

	// �E�B���h�E�X�^�C��
	const DWORD WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	// �E�B���h�E�T�C�Y�̎Z�o	
	RECT windowRect = { 0, 0, SystemData::k_ScreenWidth, SystemData::k_ScreenHeight };
	AdjustWindowRect(&windowRect, WINDOW_STYLE, FALSE);

	// �E�B���h�E�̋�`���畝�ƍ��������߂�
	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// �f�X�N�g�b�v�̃T�C�Y���擾

	// �f�X�N�g�b�v�̋�`���擾
	RECT desktopRect;
	// �������̓E�B���h�E�n���h���i�p�\�R����ʂ��E�B���h�E�j�A�������̓p�\�R����ʂ̃E�B���h�E���擾����֐�,�������ɋl�߂�
	GetWindowRect(GetDesktopWindow(), &desktopRect);

	// �f�X�N�g�b�v�̕��ƍ������Z�o
	int desktop_width = desktopRect.right - desktopRect.left;
	int desktop_height = desktopRect.bottom - desktopRect.top;

	// int c = max(a,b)�͑傫���ق���c�ɓ���
	int windowX = max((desktop_width - windowWidth) / 2, 0);
	int windowY = max((desktop_height - windowHeight) / 2, 0);

	// �E�B���h�E�̐���
	hwnd = CreateWindow(			// winAPI�̊֐� �E�B���h�E�̃n���h�����������Ă���AHWND�ł����Ƃ�
		WindowData::k_ClassName,				// �E�B���h�E�@�N���X
		WindowData::k_TitleName,				// �E�B���h�E�@�e�L�X�g �E�B���h�E�̏�̕�����
		WINDOW_STYLE,						// �E�B���h�E�@�X�^�C��  ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX�����邱�Ƃɂ��E�B���h�T�C�Y�ύX�s��
		windowX,							// �T�C�Y�ƈʒu�@�E�B���h�E�̍���x�ʒu
		windowY,							// �T�C�Y�ƈʒu	�E�B���h�E�̍��゙�ʒu
		windowWidth,						// �T�C�Y�ƈʒu�@��
		windowHeight,						// �T�C�Y�ƈʒu�@����
		NULL,								// �e�E�B���h�E�n���h��
		NULL,								// ���j���[�n���h��
		GetModuleHandle(NULL),				// �C���X�^���X�n���h�� 
		NULL								// �ǉ��̃A�v���P�[�V�����f�[�^
	);

	// ���s������false��Ԃ�
	if (hwnd == nullptr)
	{
		return false;
	}

	// �w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��

	ShowWindow(hwnd, SW_SHOW);// �E�B���h�E�o��

	// ���s������false��Ԃ�
	if (UpdateWindow(hwnd) == FALSE)// �E�B���h�E�̍X�V
	{
		return false;
	}

	return true;
}
