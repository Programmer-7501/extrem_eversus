
#include "common.hlsl"

void main(in POS_COLOR_PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = In.Color;
}