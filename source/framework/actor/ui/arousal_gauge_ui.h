/**
 * @file arousal_gauge_ui.h
 * @brief 覚醒ゲージUI
 */

#pragma once

#include"../actor.h"

 /**
 * @brief 覚醒ゲージUIクラス
 */
class ArousalGaugeUI : public Actor
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
	//! 覚醒コンポーネント
	class ArousalComponent* m_ArousalComponent;

	//! UI表示用スプライト
	class GaugeSpriteComponent* m_GaugeSpriteComponent;

	class MobileSuit* m_MobileSuit;
public:
	ArousalGaugeUI();
	~ArousalGaugeUI();

	// セッター
	inline void SetMobileSuit(class MobileSuit* mobileSuit) { m_MobileSuit = mobileSuit; }
protected:

	/**
	* @brief 覚醒ゲージUI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief 覚醒ゲージUI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief 覚醒ゲージUI独自の更新処理
	*/
	virtual void UpdateActor()override;
};