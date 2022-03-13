/**
 * @file title_scene_ui.h
 * @brief タイトルUI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief タイトルUIクラス
 */
class TitleSceneUI : public Actor
{
public:
	static ID3D11ShaderResourceView* m_TitleSprite001SRV;
	static ID3D11ShaderResourceView* m_TitleSprite002SRV;
	static ID3D11ShaderResourceView* m_TitleSprite003SRV;
	static ID3D11ShaderResourceView* m_PressEnterButtonSRV;

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

	//! タイトルのスプライト
	class SpriteComponent* m_TitleSprite001;
	//! タイトルのスプライト
	class SpriteComponent* m_TitleSprite002;
	//! タイトルのスプライト
	class SpriteComponent* m_TitleSprite003;

	//! プレスエンターボタン
	class SpriteComponent* m_PressEnterButton;

	//! フレームカウント
	int m_FrameCount;

public:
	TitleSceneUI();
	~TitleSceneUI() override {};

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