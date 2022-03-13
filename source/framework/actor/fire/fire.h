/**
 * @file fire.h
 * @brief 炎
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief 炎クラス
 */
class Fire : public Actor
{
protected:

	//! 使用パーティクルネーム
	std::string m_UseParticleName;

	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter;

public:
	Fire();
	~Fire()override {};

	/**
	* @brief 使用パーティクルの名前のセット
	* @param[in] const std::string&(particleName) 使用パーティクルの名前
	*/
	void SetUseParticleName(const std::string& particleName) { m_UseParticleName = particleName; }
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
};