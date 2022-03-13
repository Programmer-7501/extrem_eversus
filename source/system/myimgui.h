/**
* @file myimgui.h
* @brief imgui�g�p
*/

#pragma once

#include"../function/singleton.h"

/**
* @namespace MyImGuiData
* @brief ImGui�̏��
*/
namespace MyImGuiData
{
	//! �f�o�b�O�E�B���h�E�̖��O
	static const char* k_DebugWindowName = "SceneInfo";
	//! �f�o�b�O�E�B���h�E�̃T�C�Y
	static const ImVec2 k_DebugWindowSize = ImVec2(400.0f, 300.0f);

	//! �v�����Ԏq�E�B���h�E�̖��O
	static const char* k_TimeMeasurementChilWindowName = "TimeMeasurement";
	//! �v�����Ԏq�E�B���h�E�̃T�C�Y1
	static const ImVec2 k_TimeMeasurementChilWindowSize = ImVec2(400.0f, 40.0f);

	//! �����_�����O���ʎq�E�B���h�E�̖��O
	static const char* k_RenderingResultChildWindowName = "RenderingResult";
	//! �����_�����O���ʎq�E�B���h�E�̃T�C�Y
	static const ImVec2 k_RenderingResultChildWindowSize = ImVec2(400.0f, 200.0f);

	//! �����_�����O���ʃe�N�X�`���T�C�Y
	static const ImVec2 k_RenderingResultTextureSize = ImVec2(100.0f, 100.0f);
}

/**
* @brief imgui�d�l�N���X
*/
class MyImGui final : public Singleton<MyImGui>
{
private:
	friend class Singleton<MyImGui>;

	MyImGui() = default;
	~MyImGui() = default;

public:
	/**
	* @brief ����������
	*/
	void Init();

	/**
	* @brief �I������
	*/
	void Uninit();
	/**
	* @brief �X�V����
	*/
	void Update();

	/**
	* @brief �`�揈��
	*/
	void Draw() const;

private:
	/**
	* @brief �I���֐�
	*/
	void Begin();
};
