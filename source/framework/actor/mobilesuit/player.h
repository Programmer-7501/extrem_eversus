/**
 * @file player.h
 * @brief プレイヤークラス
 */

#pragma once

#include"mobilesuit.h"
#include"../../../directx/render_data/cbuffer_data.h"

namespace PlayerData
{
	//! ビールライフルの最大インスタンス数
	static constexpr int k_MaxBeamRifleInstanceNum = 20;

	//! ビームライフルの最大数
	static constexpr int k_MaxBeamRifleNum = 10;

	//! キャノンの最大インスタンス数
	static constexpr int k_MaxCannonInstanceNum = 2;

	//! キャノンの最大数
	static constexpr int k_MaxCannonNum = 1;
}

 /**
 * @brief プレイヤークラス
 */
class Player : public MobileSuit
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

	//! ロックオンカーソルコンポーネント
	class LockonCursolBillboardComponent* m_LockonCursolBillboardComponent;

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

public:
	Player();
	~Player();

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
private:

	/**
	* @brief プレイヤー独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief プレイヤー独自の破棄処理
	*/
	void UninitActor()override;

	/**
	* @brief プレイヤー独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief プレイヤー独自の入力処理
	*/
	void ProcessInputActor() override;
};