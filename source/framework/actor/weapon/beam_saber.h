/**
 * @file beam_saber.h
 * @brief ビームサーベル
 */

#pragma once

#include"../actor.h"
#include"../../../directx/render_data/cbuffer_data.h"

namespace BeamSaberData
{

	//! エミッター数
	static constexpr int k_MainEmitterNum = 40;
}
 /**
 * @brief ビームサーベルクラス
 */
class BeamSaber : public Actor
{
public:
	//! ビームサーベル柄モデル
	static class Model* m_BeamSaber_Handle_Model;

	static void LoadModel();
	static void UnloadModel();

public:
	enum BeamSaberState
	{
		STATE_NONE,
		STATE_NKAKUTOU001,
		STATE_NKAKUTOU002,
		STATE_NKAKUTOU003,
	};

protected:
	//! OBBコリジョンコンポーネント
	class OBBCollisionComponent* m_OBBCollisionComponent;

	//! ビームサーベル柄モデル
	class ModelComponent* m_ModelComponent;

	//! 使用パーティクルネーム
	std::string m_UseParticleName;

	//! パーティクルエミッター構造体
	CBufferData::ParticleEmitter m_Emitter[BeamSaberData::k_MainEmitterNum * 2];

	//! パーティクルエミッターインデックス
	int m_ParticleEmitterIndex;

	//! エフェクトマネージャー
	class EffectManager* m_LandingEffectManager;

	//! カラー
	Conv_XM::Vector4f m_Color;

	//! 所有モビルスーツ
	class MobileSuit* m_OwnerMobileSuit;

	//! 状態
	BeamSaberState m_BeamSaberState;

	//! ビームサーベルアニメーションカウント
	int m_BeamSaberAnimationCount;

	//! ビームサーベルエフェクト時間
	float m_BeamSaberMainEffectTime;
	float m_BeamSaberSubEffectTime;
public:
	BeamSaber();
	~BeamSaber();

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

	inline void SetColor(const Conv_XM::Vector4f color) { m_Color = color; }

	inline void SetOwnerMobileSuit(class MobileSuit* ownerMobileSuit) { m_OwnerMobileSuit = ownerMobileSuit; }

	//! リセット
	void Reset();

	//! N格闘1
	void StartNKakutou001();

	//! N格闘2
	void StartNKakutou002();

	//! N格闘3
	void StartNKakutou003();

	void SetParticleTime(float mainTime, float effectTime);

protected:

	/**
	* @brief ビームサーベル独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief ビームサーベル独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief ビームサーベル独自の入力処理
	*/
	virtual void ProcessInputActor()override;
};