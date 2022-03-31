/**
 * @file enemy_beam_rifle.h
 * @brief 敵のビームライフル
 */

#pragma once

#include"enemy_bullet.h"

 /**
 * @brief 敵のビームライフル
 */
class EnemyBeamRifle : public EnemyBullet
{
protected:
	//! 使用パーティクルネーム
	std::string m_UseParticleName;

	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter;
public:
	EnemyBeamRifle();
	~EnemyBeamRifle();

	/**
	* @brief 使用パーティクルの名前のセット
	* @param[in] const std::string&(particleName) 使用パーティクルの名前
	*/
	void SetUseParticleName(const std::string& particleName) { m_UseParticleName = particleName; }

protected:

	/**
	* @brief 敵のビームライフル独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief 敵のビームライフル独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief 敵のビームライフル独自の入力処理
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief 敵のビームライフル独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 当たり判定処理
	* @details 衝突時に呼び出される関数
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;
};