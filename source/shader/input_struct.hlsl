

struct POS_UV_VS_IN
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct POS_UV_PS_IN
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct POS_COLOR_VS_IN
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct POS_COLOR_PS_IN
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

struct POS_COLOR_UV_VS_IN
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

struct POS_COLOR_UV_PS_IN
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

struct POS_NORMAL_UV_VS_IN
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct POS_NORMAL_UV_PS_IN
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct PARTICLE_VS_IN
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    uint InstanceID : SV_InstanceID;
};


struct PARTICLE_PS_IN
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

struct ANIME_VS_IN
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    uint4 BoneIds1 : TEXCOORD1;
    uint4 BoneIds2 : TEXCOORD2;
    float4 Weights1 : TEXCOORD3;
    float4 Weights2 : TEXCOORD4;
};

struct CSInput
{
    uint3 GroupThread : SV_GroupThreadID;
    uint3 Group : SV_GroupID;
    uint GroupIndex : SV_GroupIndex;
    uint3 Dispatch : SV_DispatchThreadID;
};


// �s�N�Z���V�F�[�_�[����̏o��
struct GBufferOut
{
    float4 Albedo : SV_Target0; //�A���x�h
    float4 Normal : SV_Target1; // �@��
    float4 Specular : SV_Target2; //�X�؃L����(r�Ƀ��^���b�N�Aa�Ɋ��炩��)
};

struct MESHFIELD_VS_IN
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 TexBlend : TEXCOORD1;
};

struct MESHFIELD_PS_IN
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 TexBlend : TEXCOORD1;
};

struct INSTANCING_VS_IN
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4x4 WorldMatrix : MATRIX; // �C���X�^���X���Ƃɐݒ肳���s��
};

struct INSTANCING_COLOR_VS_IN
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4x4 WorldMatrix : MATRIX; // �C���X�^���X���Ƃɐݒ肳���s��
    float4 Color : COLOR; // �C���X�^���X���Ƃɐݒ肳���F
};


struct INSTANCING_COLOR_PS_IN
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Color : COLOR; // �C���X�^���X���Ƃɐݒ肳���F
};