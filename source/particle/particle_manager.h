/**
* @file particle_manager.h
* @brief パーティクル管理
*/

#pragma once

#include"../function/singleton.h"
#include"../directx/render_data/cbuffer_data.h"
#include"../shader/shader_data.h"

namespace ParticleManagerData
{
	//! ビームパーティクルの名前
	static const char* k_BeamParticleName = "BeamParticle";

	//! ビームパーティクルの最大数
	static constexpr int k_MaxBeamRifleParticleNum = 16384;
}

/**
* @brief パーティクル管理クラス
*/
class ParticleManager final : public Singleton<ParticleManager>
{
private:
	//! パーティクルマップ
	std::unordered_map<std::string, class Particle*> m_ParticleMap;

	//! エミッター構造体
	CBufferData::ParticleEmitter m_Emitters[EMITTER_MAXNUM] = {};

	//! エミッターインデックスカウント
	int m_EmitterIndexCount = 0;

	//! エミッター配列のコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_EmitterArrayConstantBuffer = nullptr;

	//! 死亡パーティクル数入力コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> DeadParticleNumConstantBuffer = nullptr;

	//! 使用エミッターコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_EmitterConstantBuffer = nullptr;


	//! 乱数用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RandomTexture;
	//! 乱数用テクスチャSRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RandomTextureSRV;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext = nullptr;

	//! エミッター配列を更新する必要があるかどうか
	bool m_EmitterArrayIsUpdate = true;


	//! エミッターコンピュートシェーダー
	ID3D11ComputeShader* m_EmitterComputeShader[CBufferData::ET_MAX] = {};


	// デバッグ用のバッファ
	ID3D11Buffer* m_DebugCounterBuffer = nullptr;

	// デバッグ用フラグ
	bool m_IsDebugCountView = false;

private:
	friend class Singleton<ParticleManager>;

	ParticleManager() = default;
	~ParticleManager() = default;

public:

	/**
	* @brief 初期化処理
	*/
	void Init();

	/**
	* @brief 終了処理
	*/
	void Uninit();

	/**
	* @brief リセット処理
	*/
	void Reset();


	/**
	* @brief パーティクルの発生
	* @param[in] const std::string&(particleName) パーティクルネーム
	* @param[in] int(emitterIndex) パーティクルエミッターインデックス番号
	*/
	void SpawnParticle(const std::string& particleName, int emitterIndex);


	/**
	* @brief 更新処理
	*/
	void Update();


	/**
	* @brief 描画処理
	*/
	void Draw() const;

	/**
	* @brief エミッター配列のコンスタントバッファの更新
	* @details 
	*/
	void UpdateParticleEmitterArrayConstantBuffer();

	/**
	* @brief パーティクルの作成、登録
	* @param[in] const std::string&(particleName) パーティクルネーム
	* @param[in] int(maxParticleNum) パーティクルの最大数
	* @param[in] ID3D11ShaderResourceView*(srv) パーティクルに使用するテクスチャ
	*/
	void CreateParticle(const std::string& particleName,int maxParticleNum, ID3D11ShaderResourceView* srv, bool invisibleAddFlag = true);

	/**
	* @brief エミッターのコンスタントバッファの更新
	* @param[in] int(emitterIndex) パーティクルエミッターインデックス番号
	* @param[in] const CBufferData::ParticleEmitter&(particleEmitter) パーティクルエミッター構造体
	*/
	void UpdateParticleEmitterConstantBuffer(int emitterIndex, const CBufferData::ParticleEmitter& particleEmitter);

	/**
	* @brief パーティクルエミッター登録
	* @param[in] const CBufferData::ParticleEmitter&(particleEmitter) パーティクルエミッター
	* @return 登録されたインデックス番号(エラーなら-1)
	* @details 初期化処理中に使用する
	*/
	inline int RegisterParticleEmitter(const CBufferData::ParticleEmitter& particleEmitter)
	{
		if (m_EmitterIndexCount >= EMITTER_MAXNUM)
		{
			Logger::GetInstance().SetLog("ParticleManager::SetParticleEmitter 最大数を超えたエミッターの登録");
			return -1;
		}

		// エミッターを登録
		m_Emitters[m_EmitterIndexCount] = particleEmitter;
		m_Emitters[m_EmitterIndexCount].MyIndex = m_EmitterIndexCount;
		m_EmitterIndexCount++;

		return (m_EmitterIndexCount - 1);
	}


	inline void SetDebugCountView(bool flag) { m_IsDebugCountView = flag; }

private:

	/**
	* @brief エミッターのコンスタントバッファの設定
	* @param[in] int(emitterIndex) パーティクルエミッターインデックス番号
	* @return int パーティクル発生数
	*/
	int SetParticleEmitterConstantBuffer(int emitterIndex);

	/**
	* @brief UAVのカウント読み込み
	* @param[in] ID3D11UnorderedAccessView*(uav) 読み込むUAV
	* @return int カウント数
	*/
	int ReadCounter(ID3D11UnorderedAccessView* uav);
};