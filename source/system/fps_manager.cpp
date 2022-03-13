
#include"main.h"
#include "fps_manager.h"
#include"myimgui.h"

/**
* @namespace FPSManagerData
* @brief FPS�̊Ǘ����
*/
namespace FPSManagerData
{
	//! FPS���v������t���[��
	static constexpr int k_FPSMesureFrame = 60;
}

void FPSManager::Init()
{
	// ���x���擾
	QueryPerformanceFrequency(&m_TimeFreq);

	// �^�C�}�[�̍ŏ����x��1msec�ɂ���
	timeBeginPeriod(1);
}

void FPSManager::Uninit()
{
	// �^�C�}�[�̍ŏ����x��߂�
	timeEndPeriod(1);
}

void FPSManager::Begin()
{
	//�v���J�n
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
	//�v���I��
	QueryPerformanceCounter(&m_TimeEnd);

	LONGLONG delta = m_TimeEnd.QuadPart - m_TimeStart.QuadPart;

	//�����̏��v����
	double deltaTime = static_cast<double>(delta) / static_cast<double>(m_TimeFreq.QuadPart);

	//�ҋ@����
	double restTime = (1.0 / static_cast<double>(SystemData::k_FPS)) - deltaTime;

	if (restTime > 0.0f)
	{
		DWORD sleepTime = static_cast<DWORD>(restTime * 1000.0);
		Sleep(sleepTime);
	}

	// FPS�v��
	if (m_FPSViewCount >= FPSManagerData::k_FPSMesureFrame)
	{
		LARGE_INTEGER loopEnd;
		//1���[�v������̎��Ԃ��Z�o
		QueryPerformanceCounter(&loopEnd);
		delta = loopEnd.QuadPart - m_TimeStart.QuadPart;
		deltaTime = (double)delta / (double)m_TimeFreq.QuadPart;

		m_MeasuredFPS = 1.0 / deltaTime;

		// �J�E���g���Z�b�g
		m_FPSViewCount = 0;
	}

	// �J�E���g�A�b�v
	m_FPSViewCount++;
}