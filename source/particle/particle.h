/**
 * @file particle_component.h
 * @brief パーティクルコンポーネント
 */

#pragma once

#include"../directx/render_data/cbuffer_data.h"

 /**
 * @brief パーティクルコンポーネント
 */
class Particle
{
private:
	/**
	* @brief パーティクルデータ構造体
	*/
	struct ParticleData
	{
		//! 座標
		Conv_XM::Vector3f Position;
		//! 速度
		Conv_XM::Vector3f Velocity;
		//! ライフ
		float Life;
		//! サイズ
		Conv_XM::Vector2f Size;
		//! ワールドマトリクス
		Conv_XM::Matrix4x4f WorldMatrix;
		//! カラー
		Conv_XM::Vector4f Color;
		//! エミッターインデックス
		int EmitterIndex;
	};
private:
	//! インスタンス情報バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawArgs;
	//! 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! インスタンス情報UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_DrawArgsUAV;

	//! パーティクル情報バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ParticleBuffer;

	//! パーティクル情報入出力UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_ParticleUAV;

	//! パーティクル情報出力SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ParticleSRV;

	//! 死亡パーティクルインデックス
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DeadParticleIndexBuffer;

	//! 死亡パーティクル入出力UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_DeadParticleIndexAppendConsumeUAV;

	//! 使用パーティクルインデックス
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_AlliveParticleIndexBuffer;

	//! 使用パーティクル入出力UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_AlliveParticleIndexUAV;

	//! 使用パーティクル出力SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AlliveParticleIndexSRV;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;
	//! 頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;
	//! テクスチャ
	ID3D11ShaderResourceView* m_ShaderResourceView;

	//! 初期化コンピュートシェーダー
	ID3D11ComputeShader* m_InitComputeShader;

	//! 更新コンピュートシェーダー
	ID3D11ComputeShader* m_UpdateComputeShader;

	//! シーンレンダーマネージャー
	class SceneRenderManager* m_SceneRenderManager;

	//! パーティクルの最大数(256の倍数のみ)
	int m_MaxParticleNum;

	//! 更新ディスパッチ数
	int m_Dispatch;

	//! 加算合成フラグ
	bool m_BlendStateInvisibleAddFlag;
public:

	/**
	* @brief コンストラクタ
	* @param[in] int(maxParticleNum) パーティクルの最大数
	* @details パーティクルの最大数は256の倍数のみ
	*/
	Particle(int maxParticleNum);
	~Particle();

	/**
	* @brief 初期化処理
	*/
	bool Init();

	/**
	* @brief 更新処理
	*/
	void Update();

	/**
	* @brief 描画処理
	*/
	void Draw() const;

	/**
	* @brief エミッター起動
	* @param[in] int(spawnNum) 発生させるパーティクル数
	* @details パーティクルマネージャーでエミッターと死亡パーティクル数をコンスタントバッファにセットしておく必要がある
	*/
	void Emitter(int spawnNum);

	/**
	* @brief テクスチャ設定
	* @param[in] ID3D11ShaderResourceView*(srv) テクスチャのシェーダーリソースビュー
	*/
	inline void SetTexture(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }

	/**
	* @brief 死亡パーティクルインデックスUAVを取得
	* @return ID3D11UnorderedAccessView* 死亡パーティクルインデックスUAV
	*/
	inline ID3D11UnorderedAccessView* GetDeadParticleIndexAppendConsumeUAV() const { return m_DeadParticleIndexAppendConsumeUAV.Get();}

	/**
	* @brief 生存パーティクルインデックスUAVを取得
	* @return ID3D11UnorderedAccessView* 生存パーティクルインデックスUAV
	*/
	inline ID3D11UnorderedAccessView* GetAlliveParticleIndexAppendConsumeUAV() const { return m_AlliveParticleIndexUAV.Get(); }

	/**
	* @brief パーティクルの最大数を取得
	* @return int パーティクルの最大数
	*/
	inline int GetMaxParticleNumber() const { return m_MaxParticleNum; }

	/**
	* @brief 加算合成フラグセット
	* @param[in] bool(flag) 加算合成フラグ
	*/
	inline void SetBlendStateInvisibleAddFlag(bool flag) { m_BlendStateInvisibleAddFlag = flag; }
private:

	/**
	* @brief パーティクルデータ初期化
	* @return bool 成功したかどうか
	*/
	bool InitParticleData();

	/**
	* @brief パーティクルデータ更新
	*/
	void UpdateParticle() const;

	/**
	* @brief デバイスコンテキストの取得
	* @return bool 成功したかどうか
	*/
	bool GetDeviceContext();

	/**
	* @brief シェーダーの取得
	*/
	bool GetShader();
};