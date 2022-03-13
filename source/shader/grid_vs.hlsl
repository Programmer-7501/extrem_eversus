
#include "common.hlsl"

void main(in POS_COLOR_VS_IN In, out POS_COLOR_PS_IN Out)
{
    float4x4 wvp;
    wvp = mul(World, drawData.View);
    wvp = mul(wvp, drawData.Projection);

    Out.Position = mul(In.Position, wvp);
    Out.Color = In.Color;
}