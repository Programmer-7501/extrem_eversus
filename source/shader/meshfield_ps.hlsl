
#include "common.hlsl"

Texture2D g_GrassTexture : register(t0);
Texture2D g_RockTexture : register(t4);
Texture2D g_BorderTexture : register(t5);
sampler g_SamplerState : register(s0);

GBufferOut main(in MESHFIELD_PS_IN IN)
{
    GBufferOut gBufferOut = (GBufferOut) 0;
    
    float4 grassColor = g_GrassTexture.Sample(g_SamplerState, IN.TexCoord);
    float4 rockColor = g_RockTexture.Sample(g_SamplerState, IN.TexCoord);
    float4 borderColor = g_BorderTexture.Sample(g_SamplerState, IN.TexCoord);
    float4 textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float blend1 = IN.TexBlend.x;
    float blend2 = IN.TexBlend.y;
    float blend3 = IN.TexBlend.z;
    //float blend4 = IN.TexBlend.w;
    
    textureColor += grassColor * blend1;
    textureColor += rockColor * blend2;
    textureColor += borderColor * blend3;
    
    // �A���x�h�J���[���o��
    gBufferOut.Albedo = textureColor;
 
    // �@�����o��
    gBufferOut.Normal = float4((IN.Normal / 2.0f) + 0.5f, 1.0f);

    
    //�X�؃L�������x���o��
    gBufferOut.Specular.r = material.Metaric; // ���^���b�N
    gBufferOut.Specular.a = material.Smoothness; // ���炩��
    
    return gBufferOut;
}