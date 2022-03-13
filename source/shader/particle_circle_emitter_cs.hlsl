
#include "common.hlsl"

RWStructuredBuffer<ParticleData> ParticleDatas : register(u1);
ConsumeStructuredBuffer<uint> DeadParticleIndex : register(u2);

Texture2D g_Texture : register(t3);
sampler g_SamplerState : register(s0);

float randSimple(float2 v)
{
    return frac(sin(dot(v.xy, float2(12.9898, 78.233))) * 43758.5453);
}

/**
* @brief スフィア上に存在するランダムな点
* @param[in] float(rand1) 0から1のランダムナンバー
* @param[in] float(rand2) 0から1のランダムナンバー
*/
float3 RandomOnSphere(float rand1, float rand2)
{
    float cosTheta = (2.0f * rand1) - 1.0f;
    float sinTheta = sqrt(1.0f - (cosTheta * cosTheta));
    float phi = 2.0f * k_PI * rand2;
    return float3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}

/**
* @brief スフィア内に存在するランダムな点
* @param[in] float(maxradius) 最大半径
* @param[in] float(rand1) 0から1のランダムナンバー
* @param[in] float(rand2) 0から1のランダムナンバー
* @param[in] float(rand3) 0から1のランダムナンバー
*/
float3 RandomInSphere(float maxradius, float rand1, float rand2, float rand3)
{
    float cosTheta = 2.0f * rand1 - 1.0f;
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
    float phi = 2.0f * k_PI * rand2;
    float radius = pow(maxradius * rand3, 1.0f / 3.0f);
    return float3(radius * sinTheta * cos(phi), radius * sinTheta * sin(phi), radius * cosTheta);
}

[numthreads(PARTICLE_EMITTER_THREDNUM, 1, 1)]
void main(in CSInput IN)
{
    if ((IN.Dispatch.x < DeadParticlesNum) && (IN.Dispatch.x < (uint) UseParticleEmitter.MaxSpawnParticlesThisFrame))
    {
        ParticleData particleData = (ParticleData) 0;

        particleData.Life = UseParticleEmitter.ParticleLifeSpan;
        particleData.Position = UseParticleEmitter.EmitterPosition;
        
        float2 uv = float2((float) IN.Dispatch.x / PARTICLE_EMITTER_SPAWN_MAX, drawData.RandomNumber);
        float4 random = g_Texture.SampleLevel(g_SamplerState, uv, 0);
        float randomm = randSimple(uv);
        float2 uv2 = float2((float) (IN.Dispatch.x + 1) / (PARTICLE_EMITTER_SPAWN_MAX + 1), drawData.RandomNumber);
        float randommm = randSimple(uv2);

     
        //float3 sphere = RandomOnSphere(random.z, random.w);
        float3 sphere = RandomOnSphere(randomm, randommm);

        particleData.Velocity = sphere * UseParticleEmitter.Speed;
        particleData.Size = UseParticleEmitter.Size;
        particleData.Color = UseParticleEmitter.Color;
        particleData.EmitterIndex = UseParticleEmitter.MyIndex;

        uint index = DeadParticleIndex.Consume();

        ParticleDatas[index] = particleData;
    }
}