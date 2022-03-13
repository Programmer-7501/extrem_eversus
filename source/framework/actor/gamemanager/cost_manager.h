/**
 * @file cost_manager.h
 * @brief コスト管理
 */

#pragma once

#include"../actor.h"

 /**
 * @brief コスト管理クラス
 */
class CostManager : public Actor
{
public:
	static void LoadTexture();

	static void UnloadTexture();
private:
	//! 自分のコスト
	int m_MyCost;

	//! 敵のコスト
	int m_EnemyCost;

	//! 自分のゲージスプライトコンポーネント
	class GaugeSpriteComponent* m_MyGaugeSpriteComp;

	//! 敵のゲージスプライトコンポーネント
	class GaugeSpriteComponent* m_EnemyGaugeSpriteComp;

	//! TEAMスプライトコンポーネント
	class SpriteComponent* m_TeamSpriteComponent;

	//! ENEMYスプライトコンポーネント
	class SpriteComponent* m_EnemySpriteComponent;

public:
	CostManager();
	~CostManager();

	/**
	* @brief コストを元に戻す
	*/
	void Reset();

	/**
	* @brief 自分のコストを減らす
	*/
	inline void DecreaseMyCost(int cost) { m_MyCost -= cost; }

	/**
	* @brief 敵のコストを減らす
	*/
	inline void DecreaseEnemyCost(int cost) { m_EnemyCost -= cost; }

	// コスト取得
	inline int GetMyCost() { return m_MyCost; }
	inline int GetEnemyCost() { return m_EnemyCost; }

protected:

	/**
	* @brief コスト管理の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief コスト管理の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief コスト管理の更新処理
	*/
	virtual void UpdateActor()override;
};