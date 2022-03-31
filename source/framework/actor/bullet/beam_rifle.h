/**
 * @file beam_rifle.h
 * @brief ビームライフル
 */

#pragma once

#include"bullet.h"

 /**
 * @brief ビームライフル
 */
class BeamRifle : public Bullet
{
protected:
	//! 使用パーティクルネーム
	std::string m_UseParticleName;

	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter;

public:
	BeamRifle();
	~BeamRifle();

	/**
	* @brief 使用パーティクルの名前のセット
	* @param[in] const std::string&(particleName) 使用パーティクルの名前
	*/
	void SetUseParticleName(const std::string& particleName) { m_UseParticleName = particleName; }

protected:

	/**
	* @brief ビームライフル独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ビームライフル独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ビームライフル独自の入力処理
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief ビームライフル独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 当たり判定処理
	* @details 衝突時に呼び出される関数
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;
};