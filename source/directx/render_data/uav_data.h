/**
* @file uav_data.h
* @brief アンオーダーアクセスビューの情報集約
*/

#pragma once

/**
* @namespace SRVData
* @brief アンオーダーアクセスビューの情報
*/
namespace UAVData
{
	// uavインデックスデータ
	enum Index
	{
		//! パーティクルのインスタンス情報インデックス
		PARTICLEDRAWARGS_INDEX,
		//! パーティクル情報インデックス
		PARTICLEDATA_INDEX,
		//! 死亡パーティクルインデックスの情報インデックス
		DEADPARTICLE_INDEX,
		//! 生存パーティクルインデックスの情報インデックス
		ALLIVEPARTICLE_INDEX,
	};
}
