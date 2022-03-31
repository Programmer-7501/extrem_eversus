/**
 * @file bullet_data.h
 * @brief 弾のデータ
 */

#pragma once

namespace BeamRifleData
{
	//! デフォルトの速度
	static constexpr float k_DefaultSpeed = 0.75f;

	//! デフォルトの終了フレーム
	static constexpr int k_DefaultFinishFrame = 300;

	//! 弾の色
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.8f, 1.0f);
	//! 1フレーム中に発生させるパーティクル数
	static constexpr int k_DefaultSpawnNum = 2;
	//! ライフ
	static constexpr float k_DefaultLife = 5.0f;
	//! パーティクルサイズ
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(1.0f, 1.0f);
	//! 使用エミッタータイプ
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_SPHERE;

	//! 最大回転量
	static constexpr float k_DefaultMaxRotate = 0.25f;
}

namespace BazookaData
{
	//! モデルファイル名
	static const char* k_ModelFileName = "asset/model/bazooka/bazooka.bin";

	//! デフォルトの速度
	static constexpr float k_DefaultSpeed = 0.75f;

	//! モデルのオフセットスケール
	static const Conv_XM::Vector3f k_OffsetModelScale = Conv_XM::Vector3f(0.2f, 0.2f, 0.2f);
}