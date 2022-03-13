
#include "common.hlsl"

RWStructuredBuffer<ParticleData> ParticleDatas : register(u1);
ConsumeStructuredBuffer<uint> DeadParticleIndex : register(u2);

Texture2D g_Texture : register(t3);
sampler g_SamplerState : register(s0);

[numthreads(PARTICLE_EMITTER_THREDNUM, 1, 1)]
void main(in CSInput IN)
{
    if ((IN.Dispatch.x < DeadParticlesNum) && (IN.Dispatch.x < (uint) UseParticleEmitter.MaxSpawnParticlesThisFrame))
    {
        ParticleData particleData = (ParticleData) 0;

        particleData.Life = UseParticleEmitter.ParticleLifeSpan;
        
                
        float2 uv = float2((float) IN.Dispatch.x / PARTICLE_EMITTER_SPAWN_MAX, drawData.RandomNumber);
        float2 uv2 = float2(drawData.RandomNumber, ((float) IN.Dispatch.x + 1) / (PARTICLE_EMITTER_SPAWN_MAX + 1));
        float3 random = g_Texture.SampleLevel(g_SamplerState, uv, 0).rgb;
        float3 random2 = g_Texture.SampleLevel(g_SamplerState, uv2, 0).rgb;
        
        random = 2.0f * random - 1.0f;
        random2 = 2.0f * random2 - 1.0f;
        particleData.Position = UseParticleEmitter.EmitterPosition + random * UseParticleEmitter.PositionVariance;
     
        particleData.Velocity = UseParticleEmitter.ParticleVelocity * UseParticleEmitter.Speed + UseParticleEmitter.VelocityVariance * random2;
        particleData.Size = UseParticleEmitter.Size;
        particleData.Color = UseParticleEmitter.Color;
        particleData.EmitterIndex = UseParticleEmitter.MyIndex;

        uint index = DeadParticleIndex.Consume();

        ParticleDatas[index] = particleData;
    }
}