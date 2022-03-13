/**
* @file mywindows.h
* @brief �E�B���h�E�����@�\�N���X
*/

#pragma once

#include"../function/singleton.h"

/**
* @namespace WindowData
* @brief �E�B���h�E�̏��
*/
namespace WindowData
{
	//! �E�B���h�E�^�C�g���o�[�̖��O
	static const char* k_ClassName = "AppClass";

	//! �E�B���h�E�^�C�g���o�[�̖��O
	static const char* k_TitleName = "ExtremeVersus";
}

/**
* @brief �E�B���h�E�Y�����N���X
*/
class MyWindows : public Singleton<MyWindows>
{
private:
	//! �E�B���h�E�n���h��
	HWND hwnd = nullptr;

private:
	friend class Singleton<MyWindows>;

	MyWindows() = default;
	~MyWindows() = default;

public:
	/**
	* @brief �E�B���h�E�̐���
	* @return bool �����������������ǂ���
	*/
	bool Create();

	/**
	* @brief �I���֐�
	*/
	void Uninit();

	/**
	* @brief HWND�擾
	*/
	inline HWND GetHWND() const
	{
		return hwnd; 
	}

private:

	/**
	* @brief �E�B���h�E�N���X�̏������A�o�^
	* @return bool �������������������ǂ���
	*/
	bool EntryWindowClass();
};
