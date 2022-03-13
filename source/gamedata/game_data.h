#pragma once

/**
* @namespace GameData
* @brief ゲーム情報
*/
namespace GameData
{
	//! ステージの左端
	constexpr float k_StageLeftLimit = -86.0f;

	//! ステージの右端
	constexpr float k_StageRightLimit = 86.0f;

	//! ステージの前端
	constexpr float k_StageFrontLimit = 90.0f;

	//! ステージの後ろ端
	constexpr float k_StageBackLimit = -90.0f;

	//! デフォルトのダウン値
	constexpr float k_DefaultDownNum = 5.0f;
}