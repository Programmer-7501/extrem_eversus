/**
 * @file enemy.h
 * @brief エネミークラス
 */

#pragma once

#include"mobilesuit.h"
#include"../../../directx/render_data/cbuffer_data.h"

#include"../../../ai/astar.h"

 /**
 * @brief  エネミークラス
 */
class Enemy : public MobileSuit
{
public:
	enum EnemyNumber
	{
		ENEMY001,
		ENEMY002,
		ENEMYMAX,
	};
public:
	static class AnimationModel* m_AnimationModel;

	/**
	* @brief モデル読み込み
	*/
	static void LoadModel();

	/**
	* @brief モデル破棄
	*/
	static void UnloadModel();
private:
	//! ロックオンカメラコンポーネント
	class LockOnCameraComponent* m_LockOnCameraComponent;

	//! アニメーションモデル
	class AnimationModelComponent* m_AnimationModelComponent;

	//! ブーストダッシュ時のオフセットローテーション
	Conv_XM::Vector4f m_BoostDashOffsetRotation;

	//! 通常時のオフセットローテーション
	Conv_XM::Vector4f m_OffsetRotation;

	//! クォータニオン合成用カウント
	float m_QuaternionCount;

	//! バーナーエミッター構造体
	CBufferData::ParticleEmitter m_BurnerEmitter001;
	CBufferData::ParticleEmitter m_BurnerEmitter002;

	//! 一フレーム前のポジション保存
	Conv_XM::Vector3f m_StorePosition;

	// 経路探索用グラフ
	WeightedGraph m_WeightedGraph;

	//! エネミー番号
	EnemyNumber m_EnemyNumber;
public:
	Enemy();
	~Enemy();


	/**
	* @brief ビームライフルショット
	*/
	virtual void BeamRifleShot() override;

	/**
	* @brief ビームライフルのクールタイム削除
	*/
	virtual void ResetBeamRifleCoolTime() override;

	/**
	* @brief 当たり判定処理
	* @details 衝突時に呼び出される関数
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;

	void SetActive(bool flag)override;

	inline void SetCostManager(class CostManager* costManager) { m_CostManager = costManager; }

	void SetIsMoveFlag(bool flag) override;

	inline void SetWeightedGraph(WeightedGraph weightedGraph) { m_WeightedGraph = weightedGraph; }

	inline void SetEnemyNumber(EnemyNumber enemyNumber) { m_EnemyNumber = enemyNumber; }
private:

	/**
	* @brief エネミー独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief エネミー独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief エネミー独自の入力処理
	*/
	void ProcessInputActor() override;

};