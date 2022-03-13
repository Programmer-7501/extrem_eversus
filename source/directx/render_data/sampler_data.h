/**
* @file sampler_data.h
* @brief サンプラーステートの情報集約
*/

#pragma once

/**
* @namespace SamplerData
* @brief サンプラーステートの情報
*/
namespace SamplerData
{
	enum Index
	{
		//! デフォルトのサンプラーインデックス
		DEFAULTSAMPLER_INDEX,
		//! シャドウマップの比較サンプリング用サンプラーステート
		SHADOWMAPSAMPLER_INDEX,
		//! サンプラーステートの最大数
		MAXSAMPLERNUM,
	};
}
