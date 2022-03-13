
#include "common.hlsl"

Texture2D g_Texture : register(t0);
sampler g_SamplerState : register(s0);

GBufferOut main(in POS_NORMAL_UV_PS_IN IN)
{
    GBufferOut gBufferOut = (GBufferOut)0;
    // アルベドカラーを出力
    gBufferOut.Albedo = g_Texture.Sample(g_SamplerState, IN.TexCoord);
 
    // 法線を出力
    gBufferOut.Normal = float4((IN.Normal / 2.0f) + 0.5f, 1.0f);
    
    //スぺキュラ強度を出力
    gBufferOut.Specular.r = material.Metaric;// メタリック
    gBufferOut.Specular.a = material.Smoothness; // 滑らかさ
    
    return gBufferOut;
}