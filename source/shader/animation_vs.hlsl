#include "common.hlsl"

// 頂点シェーダー
void main(in ANIME_VS_IN In, out POS_NORMAL_UV_PS_IN Out)
{
    // 通常の座標変換 
    matrix wvp;
    wvp = mul(World, drawData.View);
    wvp = mul(wvp, drawData.Projection);
    
    //ボーン行列による頂点変換
    matrix bonetransform;
 
    bonetransform = mul(Bones[(int) In.BoneIds1.x], In.Weights1.x);
    bonetransform += mul(Bones[(int) In.BoneIds1.y], In.Weights1.y);
    bonetransform += mul(Bones[(int) In.BoneIds1.z], In.Weights1.z);
    bonetransform += mul(Bones[(int) In.BoneIds1.w], In.Weights1.w);
    
    bonetransform += mul(Bones[(int) In.BoneIds2.x], In.Weights2.x);
    bonetransform += mul(Bones[(int) In.BoneIds2.y], In.Weights2.y);
    bonetransform += mul(Bones[(int) In.BoneIds2.z], In.Weights2.z);
    bonetransform += mul(Bones[(int) In.BoneIds2.w], In.Weights2.w);
    
    float4 posl = mul(In.Position, bonetransform);
    Out.Position = mul(posl, wvp);
    float3 norml = mul(In.Normal, (float3x3) bonetransform);
    In.Normal = mul(norml, (float3x3) World);
    Out.Normal = normalize(In.Normal);
    Out.TexCoord = In.TexCoord;
}