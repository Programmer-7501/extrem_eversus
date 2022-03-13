/**
 * @file game_manager.h
 * @brief ゲーム管理
 */

#pragma once

#include"../actor.h"

 /**
 * @brief ゲーム管理クラス
 */
class GameManager : public Actor
{
public:

	enum SceneChange
	{
		SCENE_CHANGE_NONE,
		SCENE_CHANGE_WIN,
		SCENE_CHANGE_LOSE,
	};

private:
	enum IdentificationNumber
	{
		PLAYER,
		ALIEN,
		ENEMY001,
		ENEMY002,
		MAXMOBILESUIT
	};

public:

	/**
	* @brief テクスチャ読み込み
	*/
	static void LoadTexture();

	/**
	* @brief テクスチャ破棄
	*/
	static void UnloadTexture();

private:
	//! タイムリミットUI
	class TimeLimitUI* m_TimeLimitUI;

	//! コストマネージャー
	class CostManager* m_CostManager;

	//! モビルスーツ
	class MobileSuit* m_MobileSuit[MAXMOBILESUIT];

	//! リスポーン管理タイム
	int m_RespawnTime[MAXMOBILESUIT];

	//! カウントフレーム
	int m_CountFrame;

	//! カウントダウン用スプライト
	class NumberSpriteComponent* m_CountDownSpriteComponent;

	//! standbyスプライト
	class SpriteComponent* m_StandbySpriteComponent;

	//! goスプライト
	class SpriteComponent* m_GoSpriteComponent;

	//! シーン切り替え用
	SceneChange m_SceneChange;
public:
	GameManager();
	~GameManager();

	inline void SetParameter
	(
		class TimeLimitUI* timeLimitUI,
		class CostManager* costManager,
		class MobileSuit* player,
		class MobileSuit* alliens,
		class MobileSuit* enemy001,
		class MobileSuit* enemy002
		)
	{
		m_TimeLimitUI = timeLimitUI;
		m_CostManager = costManager;
		m_MobileSuit[PLAYER] = player;
		m_MobileSuit[ALIEN] = alliens;
		m_MobileSuit[ENEMY001] = enemy001;
		m_MobileSuit[ENEMY002] = enemy002;
	}

	inline SceneChange GetSceneChange() { return m_SceneChange; }

protected:

	/**
	* @brief ゲームの読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ゲームの初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ゲームの更新処理
	*/
	virtual void UpdateActor()override;

private:

	/**
	* @brief ゲームの開始の管理
	*/
	void GameStartManager();

	/**
	* @brief リスポーンの管理
	*/
	void RespawnManager();
};