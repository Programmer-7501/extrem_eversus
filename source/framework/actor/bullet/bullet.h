/**
 * @file bullet.h
 * @brief バレット
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief バレットクラス
 */
class Bullet : public Actor
{
protected:
	//! リジッドボディコンポーネント
	class RigidBodyComponent* m_RigidBodyComponent;

	//! OBBコリジョンコンポーネント
	class OBBCollisionComponent* m_OBBCollisionComponent;

	//! 速度
	float m_Speed;

	//! フレームカウント
	int m_FrameCount;

	//! 終了フレーム
	int m_FinishFrame;

	//! 狙っている敵
	class MobileSuit* m_TargetMobileSuit;

	//! 向き
	Conv_XM::Vector3f m_Direction;

	//! 追尾フラグ
	bool m_TargetFlag;

	//! 回転上限
	float m_RotationLimit;

	//! エフェクトマネージャー
	class EffectManager* m_LandingEffectManager;

public:
	Bullet();
	~Bullet();


	/**
	* @brief 速度のセット
	* @param[in] const Conv_XM::Vector3f&(direction) 方向
	*/
	virtual void SetDirection(const Conv_XM::Vector3f& direction);

	/**
	* @brief 終了フレームのセット
	* @param[in] int(finishFrame) 終了フレーム
	*/
	inline void SetFinishFrame(int finishFrame) { m_FinishFrame = finishFrame; }

	/**
	* @brief アクティブ処理
	*/
	virtual void SetActive(bool flag)override;

	/**
	* @brief ターゲットモビルスーツの設定
	* @param[in] class MobileSuit*(mobileSuit) ターゲットのモビルスーツ
	*/
	inline void SetTargetMobileSuit(class MobileSuit* mobileSuit) 
	{
		if (mobileSuit == nullptr)
		{

		}
		else
		{
			m_TargetFlag = true;
			m_TargetMobileSuit = mobileSuit;
		}
	}


	/**
	* @brief 着弾エフェクト設定
	*/
	inline void SetLandingEffectManager(class EffectManager* landingEffectManager) { m_LandingEffectManager = landingEffectManager; }
protected:

	/**
	* @brief バレット独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief バレット独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief バレット独自の入力処理
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief v独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 当たり判定処理
	* @details 衝突時に呼び出される関数
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;

	/**
	* @brief ターゲットを追跡する
	*/
	void TargetChase();
};