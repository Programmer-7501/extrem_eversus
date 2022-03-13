/**
 * @file life_ui.h
 * @brief ライフUI
 */

#pragma once

#include"../actor.h"

namespace LifeUIData
{
	//! 数字の桁数
	static constexpr int k_Digit = 3;
}

 /**
 * @brief ライフUIクラス
 */
class LifeUI : public Actor
{
protected:
	//! ライフコンポーネント
	class LifeComponent* m_LifeComponent;

	//! 数字表示用スプライト
	class NumberSpriteComponent* m_NumberSpriteComponent[LifeUIData::k_Digit];

	class MobileSuit* m_MobileSuit;
public:
	LifeUI();
	~LifeUI();

	// セッター
	inline void SetMobileSuit(class MobileSuit* mobileSuit) { m_MobileSuit = mobileSuit; }
protected:

	/**
	* @brief ライフUI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ライフUI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ライフUI独自の更新処理
	*/
	virtual void UpdateActor()override;
};