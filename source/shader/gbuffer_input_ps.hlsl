
#include "common.hlsl"

Texture2D g_Texture : register(t0);
sampler g_SamplerState : register(s0);

GBufferOut main(in POS_NORMAL_UV_PS_IN IN)
{
    GBufferOut gBufferOut = (GBufferOut)0;
    // �A���x�h�J���[���o��
    gBufferOut.Albedo = g_Texture.Sample(g_SamplerState, IN.TexCoord);
 
    // �@�����o��
    gBufferOut.Normal = float4((IN.Normal / 2.0f) + 0.5f, 1.0f);
    
    //�X�؃L�������x���o��
    gBufferOut.Specular.r = material.Metaric;// ���^���b�N
    gBufferOut.Specular.a = material.Smoothness; // ���炩��
    
    return gBufferOut;
}