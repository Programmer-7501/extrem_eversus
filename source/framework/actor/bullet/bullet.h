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

	//! 使用パーティクルネーム
	std::string m_UseParticleName;

	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter;

	//! パーティクルエミッターインデックス
	int m_ParticleEmitterIndex;

	//! フレームカウント
	int m_FrameCount;

	//! 終了フレーム
	int m_FinishFrame;

	//! エフェクトマネージャー
	class EffectManager* m_LandingEffectManager;

	//! 狙っている敵
	class MobileSuit* m_TargetMobileSuit;

	//! 速度
	Conv_XM::Vector3f m_Velocity;

	//! 追尾フラグ
	bool m_TargetFlag;

	float m_RotationLimit;
public:
	Bullet();
	~Bullet();


	/**
	* @brief 速度のセット
	* @param[in] const Conv_XM::Vector3f&(velocity) 速度
	*/
	virtual void SetVelocity(const Conv_XM::Vector3f& velocity);

	/**
	* @brief 終了フレームのセット
	* @param[in] int(finishFrame) 終了フレーム
	*/
	inline void SetFinishFrame(int finishFrame) { m_FinishFrame = finishFrame; }

	/**
	* @brief 使用パーティクルの名前のセット
	* @param[in] const std::string&(particleName) 使用パーティクルの名前
	*/
	void SetUseParticleName(const std::string& particleName) { m_UseParticleName = particleName; }

	/**
	* @brief アクティブ処理
	*/
	virtual void SetActive(bool flag)override;

	inline void SetLandingEffectManager(class EffectManager* landingEffectManager) { m_LandingEffectManager = landingEffectManager; }

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