
#include "common.hlsl"

AppendStructuredBuffer<uint> DeadParticleIndex : register(u2);

[numthreads(PARTICLE_UPDATE_THREDNUM, 1, 1)]
void main(in CSInput IN)
{
    DeadParticleIndex.Append(IN.Dispatch.x);
}