
#include "common.hlsl"


void main(in INSTANCING_COLOR_VS_IN In, out INSTANCING_COLOR_PS_IN Out)
{
    float4x4 wvp;
    wvp = mul(In.WorldMatrix, drawData.View);
    wvp = mul(wvp, drawData.Projection);

    Out.Position = mul(In.Position, wvp);
    
    In.Normal = mul(In.Normal, (float3x3) In.WorldMatrix);
    Out.Normal = normalize(In.Normal);
    
    Out.TexCoord = In.TexCoord;
    
    Out.Color = In.Color;
}