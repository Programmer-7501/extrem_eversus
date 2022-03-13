/**
 * @file radar.h
 * @brief レーダー
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief レーダークラス
 */
class Radar : public Actor
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
	//! プレイヤーモビルスーツ
	class MobileSuit* m_PlayerMobileSuit;

	//! 味方モビルスーツ
	class MobileSuit* m_AllienMobileSuit;

	//! 敵モビルスーツ
	class MobileSuit* m_Enemy001MobileSuit;

	//! 敵モビルスーツ
	class MobileSuit* m_Enemy002MobileSuit;

	//! レーダースプライトコンポーネント
	class SpriteComponent* m_BackGroundSpriteComponent;

	//! 自分駒スプライトコンポーネント
	class ChangeableSpriteComponent* m_PlayerPieceSpriteComponent;

	//! 味方駒スプライトコンポーネント
	class ChangeableSpriteComponent* m_AllienPieceSpriteComponent;

	//! 敵001駒スプライトコンポーネント
	class ChangeableSpriteComponent* m_Enemy001PieceSpriteComponent;

	//! 敵002駒スプライトコンポーネント
	class ChangeableSpriteComponent* m_Enemy002PieceSpriteComponent;
public:
	Radar();
	~Radar();

	inline void SetParameter(class MobileSuit* player, class MobileSuit* allien, class MobileSuit* enemy001, class MobileSuit* enemy002)
	{
		m_PlayerMobileSuit = player;
		m_AllienMobileSuit = allien;
		m_Enemy001MobileSuit = enemy001;
		m_Enemy002MobileSuit = enemy002;
	}

protected:

	/**
	* @brief レーダー独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief レーダー独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief レーダーUI独自の更新処理
	*/
	virtual void UpdateActor() override;
};