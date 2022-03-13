
#include "common.hlsl"

void main(in POS_UV_VS_IN In, out POS_UV_PS_IN Out)
{
    float4x4 wvp;
    wvp = mul(World, drawData.View);
    wvp = mul(wvp, drawData.Projection);

    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;
}