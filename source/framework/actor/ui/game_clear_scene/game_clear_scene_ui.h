/**
 * @file game_clear_scene_ui.h
 * @brief ゲームクリアシーンUI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief ゲームクリアシーンUIクラス
 */
class GameClearSceneUI : public Actor
{
public:

	/**
	* @brief テクスチャ読み込み
	*/
	static void LoadTexture();

	/**
	* @brief テクスチャ破棄
	*/
	static void UnloadTexture();

protected:

	//! 背景のスプライト
	class SpriteComponent* m_BackGroundSprite;

	//! youwinスプライト
	class SpriteComponent* m_YouWinSprite;

	//! PressEnterのスプライト
	class SpriteComponent* m_PressEnterSprite;

	//! フレームカウント
	int m_FrameCount;
public:
	GameClearSceneUI();
	~GameClearSceneUI();

protected:

	/**
	* @brief ゲームクリアシーンUI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ゲームクリアシーンUI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ゲームクリアシーンUI独自の更新処理
	*/
	virtual void UpdateActor()override;
};