/**
 * @file boost_gauge_ui.h
 * @brief ブーストゲージUI
 */

#pragma once

#include"../actor.h"

 /**
 * @brief ブーストゲージUIクラス
 */
class BoostGaugeUI : public Actor
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
	//! ブーストコンポーネント
	class BoostComponent* m_BoostComponent;

	//! UI表示用スプライト
	class GaugeSpriteComponent* m_GaugeSpriteComponent;

	//! UI表示用スプライト
	class SpriteComponent* m_BoostSpriteComponent;

	//! モビルスーツアクター
	class MobileSuit* m_MobileSuit;
public:
	BoostGaugeUI();
	~BoostGaugeUI();

	// セッター
	inline void SetMobileSuit(class MobileSuit* mobileSuit) { m_MobileSuit = mobileSuit; }
protected:

	/**
	* @brief ブーストゲージUI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ブーストゲージUI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ブーストゲージUI独自の更新処理
	*/
	virtual void UpdateActor()override;
};