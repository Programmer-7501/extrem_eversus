/**
* @file srv_data.h
* @brief シェーダーリソースビューの情報集約
*/

#pragma once

/**
* @namespace SRVData
* @brief シェーダーリソースビューの情報
*/
namespace SRVData
{
	//! デフォルトのシェーダーリソースビューインデックス
	static constexpr int k_DefaultShaderResourceViewIndex = 0;

	static constexpr int k_ParticleDataShaderResourceViewIndex = 1;
	static constexpr int k_AlliveParticleIndexShaderResourceViewIndex = 2;

	//! 乱数テクスチャ用のインデックス
	static constexpr int k_RandomTextureShaderResourceViewIndex = 3;

	//! メッシュフィールド用テクスチャ
	static constexpr int k_MeshField001ShaderResourceViewIndex = 4;
	//! メッシュフィールド用テクスチャ
	static constexpr int k_MeshField002ShaderResourceViewIndex = 5;


	//! シャドウマップ用シェーダーリソースビューインデックス
	static constexpr int k_ShadowMapShaderResourceViewIndex = 10;
}
