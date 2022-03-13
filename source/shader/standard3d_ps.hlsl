
#include "common.hlsl"

Texture2D g_Texture : register(t0);
sampler g_SamplerState : register(s0);

void main(in POS_NORMAL_UV_PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
}