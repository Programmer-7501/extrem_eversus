/**
 * @file remaining_bullets_ui.h
 * @brief 残弾数表示UI
 */

#pragma once

#include"../actor.h"
#include<chrono>

namespace RemaingBulletsUIData
{
	//! 数字の桁数
	static constexpr int k_MaxDigit = 3;
}

/**
* @brief 残弾数表示UIクラス
*/
class RemaingBulletsUI : public Actor
{
protected:
	//! 数字表示用スプライト
	class NumberSpriteComponent* m_NumberSpriteComponent[RemaingBulletsUIData::k_MaxDigit];

	//! バレットマネージャー
	class BulletManager* m_BulletManager;

	//! 一番左の数字のポジション
	Conv_XM::Vector2f m_LeftPosition;

	//! 桁数
	int m_Digit;
public:
	RemaingBulletsUI();
	~RemaingBulletsUI();

	inline void SetBulletManager(class BulletManager* bulletManager) { m_BulletManager = bulletManager; }

	inline void SetLeftPosition(const Conv_XM::Vector2f& leftPosition) { m_LeftPosition = leftPosition; }
protected:

	/**
	* @brief ブーストUI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ブーストUI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ブーストUI独自の更新処理
	*/
	virtual void UpdateActor()override;
};