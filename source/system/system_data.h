/**
* @file systemdata.h
* @brief �@�\�f�[�^�W��N���X
*/

#pragma once

/**
* @namespace SystemData
* @brief ��{���
*/
namespace SystemData
{
	//! FPS
	static constexpr int k_FPS = 60;

	//! �E�B���h�E�̕�
	static constexpr int k_ScreenWidth = 960;

	//! �E�B���h�E�̍���
	static constexpr int k_ScreenHeight = 540;

	//! �A�X�y�N�g���[�g
	static constexpr float k_AspectRate = static_cast<float>(k_ScreenWidth) / static_cast<float>(k_ScreenHeight);
}