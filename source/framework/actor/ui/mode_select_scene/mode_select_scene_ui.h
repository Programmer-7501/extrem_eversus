/**
 * @file mode_select_scene_ui.h
 * @brief モード選択UI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief モード選択UIクラス
 */
class ModeSelectSceneUI : public Actor
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

	/**
	* @brief サウンド読み込み
	*/
	static void LoadSound();

	/**
	* @brief サウンド破棄
	*/
	static void UnloadSound();

public:
	enum SelectScene
	{
		SS_NONE,
		SS_GAMESCENE,
		SS_SHADERDEMOSCENE,
		SS_AIDEMOSCENE,
		SS_GRAPHMAKESCENE,
		SS_MAX,
	};
protected:

	//! モード選択
	SelectScene m_ModeSelect;

	//! シーンに送るモード
	SelectScene m_SendMode;

	//! 背景のスプライト
	class SpriteComponent* m_BackGroundSprite;

	//! ゲームシーンへのスプライト
	class ChangeableSpriteComponent* m_ToGameSceneSprite;

	//! デモシーンへのスプライト
	class ChangeableSpriteComponent* m_ToShaderDemoSceneSprite;

	//! AIデモシーンへのスプライト
	class ChangeableSpriteComponent* m_ToAIDemoSceneSprite;

	//! グラフ作成シーンへのスプライト
	class ChangeableSpriteComponent* m_ToMakeGraphSceneSprite;
public:
	ModeSelectSceneUI();
	~ModeSelectSceneUI();

	// ゲッター
	inline SelectScene GetSelectSceneMode() const { return m_SendMode; }

protected:

	/**
	* @brief モード選択UI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief モード選択UI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief モード選択UI独自の入力処理
	*/
	virtual void ProcessInputActor();
};