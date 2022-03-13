
#include"input_struct.hlsl"
#include"struct.hlsl"
#include"shader_data.h"

static const float k_PI = 3.1415926f; // π

/**
* @brief ワールド行列
*/
cbuffer WorldBuffer : register(b0)
{
    float4x4 World;
}

/**
* @brief カメラ情報
*/
cbuffer DrawDataBuffer : register(b1)
{
    DrawData drawData;
}

/**
* @brief パーティクルエミッター
*/
cbuffer ParticleEmitterArrayBuffer : register(b2)
{
    ParticleEmitterData particleEmitterData[EMITTER_MAXNUM];
}

/**
* @brief 死亡パーティクル数
*/
cbuffer DeadParticlesNumBuffer : register(b3)
{
    uint DeadParticlesNum;
    uint3 pad;
}

/**
* @brief ディレクショナルライト
*/
cbuffer DirectionalLightBuffer : register(b4)
{
    DirectionalLightData directionalLight;
}

/**
* @brief ポイントライト
*/
cbuffer PointLightBuffer : register(b5)
{
    PointLightData pointLight;
}

cbuffer MaterialBuffer : register(b6)
{
    MaterialData material;
}

cbuffer ShadowDataBuffer : register(b7)
{
    ShadowData shadowData;
};

cbuffer ParticleEmitterBuffer : register(b8)
{
    ParticleEmitterData UseParticleEmitter;
};

cbuffer BoneBuffer : register(b9)
{
    float4x4 Bones[MAX_BONE_NUM];
};