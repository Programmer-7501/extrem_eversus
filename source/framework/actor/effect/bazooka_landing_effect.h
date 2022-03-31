/**
 * @file bazooka_landing_effect.h
 * @brief バズーカ着弾エフェクト
 */

#pragma once

#include"effect.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief バズーカ着弾エフェクトクラス
 */
class BazookaLandingEffect : public Effect
{
private:
	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter;

public:
	BazookaLandingEffect();
	virtual ~BazookaLandingEffect() override;

	/**
	* @brief アクティブ処理
	*/
	virtual void SetActive(bool flag)override;
private:

	/**
	* @brief バズーカ着弾エフェクト独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief バズーカ着弾エフェクト独自の入力処理
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief バズーカ着弾エフェクト独自の更新処理
	*/
	virtual void UpdateActor()override;
};