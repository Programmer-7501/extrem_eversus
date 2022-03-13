
#include"shader_data.h"

struct DrawData
{
    float4x4 View;
    float4x4 Projection;
    float4x4 CameraViewProjectionInverse;
    float3 CameraPosition;
    float RandomNumber;
};

/**
* @brief パーティクルデータ構造体
*/
struct ParticleData
{
	// 座標
    float3 Position;
	// 速度
    float3 Velocity;
	// ライフ
    float Life;
	// サイズ
    float2 Size;
	// ワールドマトリクス
    float4x4 WorldMatrix;
	// カラー
    float4 Color;
    //! エミッターインデックス
    int EmitterIndex;
};

/**
* @brief パーティクルエミッター構造体
*/
struct ParticleEmitterData
{
    // エミッターポジション
    float3 EmitterPosition;
    // ライフによってサイズを小さくするかどうか
    uint SizeOverLifeTime;
    
    // パーティクル速度
    float3 ParticleVelocity;
    float pad2;
    
    // ポジションのばらつき
    float3 PositionVariance;
    float pad3;
    
    // 色
    float4 Color;
    
    //　このフレームに発生させるパーティクル数
    int MaxSpawnParticlesThisFrame;
    //　パーティクルのライフタイム
    float ParticleLifeSpan;
    // 使用するかどうか
    int IsUse;
    // 自身のインデックス番号
    uint MyIndex;
    
    //　速度
    float Speed;
    // エミッターの種類
    uint EmitterType;
    // パーティクルのサイズ
    float2 Size;
    
    // 速度のばらつき
    float3 VelocityVariance;
    // 重力
    float Gravity;
};

/**
* @brief ディレクショナルライトデータ構造体
*/
struct DirectionalLightData
{
	//! ディレクショナルライトの角度
    float3 Direction;
    float pad0;
	//! ディレクショナルライトの色
    float3 Color;
    float pad1;
	//! アンビエントライト
    float3 Ambient;
    float pad2;
};


/**
* @brief ポイントライトデータ構造体
*/
struct PointLightData
{
	//! ポイントライトのポジション
    float3 Position;
    float pad;
	//! ポイントライトの色
    float3 Color;
	//! ポイントライトの有効範囲
    float Range;
};

/**
* @brief マテリアル構造体
*/
struct MaterialData
{
    float4 Ambient;
    float4 Diffuse;
    float Metaric;
    float Smoothness;
    float2 pad;
};

struct ShadowData
{
    float4x4 LightViewProjection;
    float4x4 ShadowBiasMatrix;
};