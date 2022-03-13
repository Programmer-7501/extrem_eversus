
#include "common.hlsl"

StructuredBuffer<ParticleData> ParticleDatas : register(t1);

StructuredBuffer<uint> AlliveParticleIndex : register(t2);

void main(in PARTICLE_VS_IN In, out PARTICLE_PS_IN Out)
{
    uint index = AlliveParticleIndex[In.InstanceID];
    
    float4x4 wvp;
    wvp = mul(ParticleDatas[index].WorldMatrix, drawData.View);
    wvp = mul(wvp, drawData.Projection);

    Out.Color = ParticleDatas[index].Color;
    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;
}