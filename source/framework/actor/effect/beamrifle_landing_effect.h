/**
 * @file beamrifle_landing_effect.h
 * @brief ビームライフル着弾エフェクト
 */

#pragma once

#include"effect.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief ビームライフル着弾エフェクトクラス
 */
class BeanRifleLandingEffect : public Effect
{
private:
	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter;

public:
	BeanRifleLandingEffect();
	virtual ~BeanRifleLandingEffect() override;

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