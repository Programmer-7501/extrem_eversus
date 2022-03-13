/**
 * @file title_scene_ui.h
 * @brief タイトルUI
 */

#pragma once

#include"../actor.h"

 /**
 * @brief タイトルUIクラス
 */
class OtherUI : public Actor
{
public:
	static ID3D11ShaderResourceView* m_BeamRifleSRV;
	static ID3D11ShaderResourceView* m_CannonSRV;

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
	class SpriteComponent* m_BeamRifle;
	//! タイトルのスプライト
	class SpriteComponent* m_Cannon;

	//! フレームカウント
	int m_FrameCount;

public:
	OtherUI();
	~OtherUI() override {};

protected:

	/**
	* @brief モード選択UI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief モード選択UI独自の初期化処理
	*/
	virtual void InitActor() override;

};