
#include "common.hlsl"


void main(in POS_NORMAL_UV_VS_IN In, out POS_NORMAL_UV_PS_IN Out)
{
    float4x4 wvp;
    wvp = mul(World, drawData.View);
    wvp = mul(wvp, drawData.Projection);

    Out.Position = mul(In.Position, wvp);
    
    In.Normal = mul(In.Normal, (float3x3) World);
    Out.Normal = normalize(In.Normal);
    
    Out.TexCoord = In.TexCoord;
}