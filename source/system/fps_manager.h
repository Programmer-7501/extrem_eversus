/**
* @file fps_manager.h
* @brief fps����
* @see https://teratail.com/questions/158503
*/

#pragma once

#include"../function/singleton.h"

/**
* @brief FPS�Ǘ��N���X
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
	* @brief ������
	*/
	void Init();

	/**
	* @brief �I������
	*/
	void Uninit();

	/**
	* @brief FPS�̌v�Z
	* @details ���C�����[�v�̍ŏ��ɍs��
	*/
	void Begin();

	/**
	* @brief ImGui�ւ̓���
	*/
	void InputImGui();

	/**
	* @brief �ҋ@
	* @details ���C�����[�v�̍Ō�ɍs��
	*/
	void End();
};
