/**
 * @file game_over_scene_ui.h
 * @brief ゲームオーバーシーンUI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief ゲームオーバーシーンUIクラス
 */
class GameOverSceneUI : public Actor
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

	//! youloseスプライト
	class SpriteComponent* m_YouLoseSprite;

	//! PressEnterのスプライト
	class SpriteComponent* m_PressEnterSprite;

	//! フレームカウント
	int m_FrameCount;
public:
	GameOverSceneUI();
	~GameOverSceneUI();

protected:

	/**
	* @brief ゲームオーバーシーンUI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ゲームオーバーシーンUI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ゲームオーバーシーンUI独自の更新処理
	*/
	virtual void UpdateActor()override;
};