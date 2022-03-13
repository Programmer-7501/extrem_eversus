
#include"input_struct.hlsl"
#include"struct.hlsl"
#include"shader_data.h"

static const float k_PI = 3.1415926f; // ��

/**
* @brief ���[���h�s��
*/
cbuffer WorldBuffer : register(b0)
{
    float4x4 World;
}

/**
* @brief �J�������
*/
cbuffer DrawDataBuffer : register(b1)
{
    DrawData drawData;
}

/**
* @brief �p�[�e�B�N���G�~�b�^�[
*/
cbuffer ParticleEmitterArrayBuffer : register(b2)
{
    ParticleEmitterData particleEmitterData[EMITTER_MAXNUM];
}

/**
* @brief ���S�p�[�e�B�N����
*/
cbuffer DeadParticlesNumBuffer : register(b3)
{
    uint DeadParticlesNum;
    uint3 pad;
}

/**
* @brief �f�B���N�V���i�����C�g
*/
cbuffer DirectionalLightBuffer : register(b4)
{
    DirectionalLightData directionalLight;
}

/**
* @brief �|�C���g���C�g
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