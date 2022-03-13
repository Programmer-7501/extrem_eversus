/**
 * @file mobilesuit_base.h
 * @brief モビルスーツのベース
 */

#pragma once

#include"../actor.h"

 /**
 * @brief モビルスーツのベースクラス
 */
class MobileSuit : public Actor
{
public:
	/**
	* @brief サウンド読み込み
	*/
	static void LoadSound();

	/**
	* @brief サウンド破棄
	*/
	static void UnloadSound();
protected:
	//! リジッドボディコンポーネント
	class RigidBodyComponent* m_RigidBodyComponent;

	//! ブーストコンポーネント
	class BoostComponent* m_BoostComponent;

	//! ライフコンポーネント
	class LifeComponent* m_LifeComponent;

	//! 覚醒コンポーネント
	class ArousalComponent* m_ArousalComponent;

	//! 当たり判定コリジョン
	class OBBCollisionComponent* m_OBBCollisionComponent;

	//! オブジェクトとの当たり判定コリジョン
	class AABBCollisionComponent* m_AABBCollisionComponent;

	//! ブーストダッシュ中か否か
	bool m_IsBoostDash;

	//! ビームライフルバレットマネージャー
	class BulletManager* m_BeamRifleBulletManager;

	//! キャノンマネージャー
	class BulletManager* m_CannonManager;

	//! 味方
	class MobileSuit* m_AlliesMobileSuit;

	//! 1つ目のエネミー
	class MobileSuit* m_EnemyMobileSuit001;

	//! 2つ目のエネミー
	class MobileSuit* m_EnemyMobileSuit002;

	//! ターゲット番号
	int m_TargetNum;

	//! ビームライフルショットカウントフレーム
	int m_BeamRifleCountFrame;
	//! ビームライフルを撃っている状態
	bool m_IsBeamRifleShot;
	//! ビームライフルを撃ったかどうか
	bool m_IsFinishBeamRifleShot;
	//! 狙う方向
	Conv_XM::Vector3f m_BeamRifleDirection;
	//! ビームライフルクールタイム
	int m_BeamRifleCoolTime;

	//! 爆発エフェクト
	class EffectManager* m_MobileSuitEffectManager;

	//! ロックオンしているかどうか
	bool m_IsLockOn;

	//! ダウンしているかどうか
	bool m_DownFlag;
	//! ダウン値
	float m_DownNum;
	//! ダウン復帰カウント
	float m_DownCount;

	//! ダメージを受けたかどうか
	bool m_IsDamage;

	//! ダウン値回復カウント
	int m_DownNumCoolTime;

	//! ビームサーベル
	class BeamSaber* m_BeamSaber;

	//! 格闘用判別
	ActorType m_ColedType;

	// コストマネージャークラス
	class CostManager* m_CostManager;

	//! コスト
	int m_MyCost;
public:
	MobileSuit();
	~MobileSuit();

	// パラメーターをリセットする
	virtual void Reset();

	/**
	* @brief ビームライフルショット
	*/
	virtual void BeamRifleShot();

	/**
	* @brief ビームライフルのクールタイム削除
	*/
	virtual void ResetBeamRifleCoolTime();

	// セッター,ゲッター
	inline void SetIsBoostDash(bool flag) { m_IsBoostDash = flag; }
	inline bool GetIsBoostDash() const { return m_IsBoostDash; }

	inline class BulletManager* GetBeamRifleBulletManager() const { return m_BeamRifleBulletManager; }
	inline void SetBeamRifleBulletManager(class BulletManager* beamRifleBulletManager) { m_BeamRifleBulletManager = beamRifleBulletManager; }

	inline class BulletManager* GetCannonBulletManager() const { return m_CannonManager; }
	inline void SetCannonBulletManager(class BulletManager* cannonBulletManager) { m_CannonManager = cannonBulletManager; }

	inline void SetAllies(class MobileSuit* allies001) { m_AlliesMobileSuit = allies001; }
	inline class MobileSuit* GetAlliesMobileSuit() const { return m_AlliesMobileSuit; }

	inline void SetEnemy(class MobileSuit* enemy001, class MobileSuit* enemy002)
	{
		m_EnemyMobileSuit001 = enemy001;
		m_EnemyMobileSuit002 = enemy002;
	}

	inline class MobileSuit* GetEnemyMobileSuit001() const { return m_EnemyMobileSuit001; }
	inline class MobileSuit* GetEnemyMobileSuit002() const { return m_EnemyMobileSuit002; }

	inline int GetTargetNumber() const { return m_TargetNum; }
	inline void SetTargetNumber(int targetNum) { m_TargetNum = targetNum; }

	inline bool GetIsBeamRifleShot() const { return m_IsBeamRifleShot; }

	inline class LifeComponent* GetLifeComponent() const { return m_LifeComponent; }

	inline void SetMobileSuitExplosionEffectManager(class EffectManager* mobileSuitExplosionEffectManager) { m_MobileSuitEffectManager = mobileSuitExplosionEffectManager; }

	inline bool GetIsLockOn() const { return m_IsLockOn; }

	inline bool GetDownFlag() const { return m_DownFlag; }

	inline void SetIsDamageFlag(bool flag) { m_IsDamage = flag; }
	inline bool GetIsDamageFlag() { return m_IsDamage; }

	inline void SetBeamSaber(class BeamSaber* beamSaber) { m_BeamSaber = beamSaber; }
	class BeamSaber* GetBeamSaber() { return m_BeamSaber; }

	virtual void SetIsMoveFlag(bool flag);

	inline bool GetIsBeamRifleShot() { return m_IsBeamRifleShot; }
protected:

	/**
	* @brief モビルスーツ独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief モビルスーツ独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief v独自の更新処理
	*/
	virtual void UpdateActor()override;
};