/**
 * @file mobilesuit_explosion_effect.h
 * @brief モビルスーツの爆発エフェクト
 */

#pragma once

#include"effect.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief モビルスーツの爆発エフェクトクラス
 */
class MobileSuitExplosionEffect : public Effect
{
private:
	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter;

public:
	MobileSuitExplosionEffect();
	virtual ~MobileSuitExplosionEffect() override;

	/**
	* @brief アクティブ処理
	*/
	virtual void SetActive(bool flag)override;
private:

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
};