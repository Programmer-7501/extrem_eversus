
#include "common.hlsl"

Texture2D g_AlbedoColorTexture : register(t0); // �A���x�h�J���[
Texture2D g_NormalTexture : register(t1); // �@��
Texture2D g_SpecularTexture : register(t2); // �X�؃L�������x
Texture2D g_DepthTexture : register(t3); // �[�x�l�ۑ��p

Texture2D g_ShadowMapTexture : register(t10); // �V���h�E�}�b�v

sampler g_SamplerState : register(s0);

SamplerComparisonState g_ShadowMapSamplerState : register(s1); // �V���h�E�}�b�v�p�T���v���[�X�e�[�g

float3 CaluclationWroldPosition(float depth, float2 texCood, float4x4 inverseViewProjectionMatrix)
{
    float4 worldPosition;

    // �܂����K���X�N���[�����W�n�ł̍��W���v�Z����B
    // z���W�͐[�x�e�N�X�`��������������Ă���B
    worldPosition.z = depth;
    // xy���W��UV���W����v�Z����B
    worldPosition.xy = (texCood * float2(2.0f, -2.0f)) + float2(-1.0f, 1.0f);
    worldPosition.w = 1.0f;
    
    // �r���[�v���W�F�N�V�����s��̋t�s�����Z���āA���[���h���W�ɖ߂��B
    worldPosition = mul(worldPosition, inverseViewProjectionMatrix);
    worldPosition.xyz /= worldPosition.w;
    
    return worldPosition.xyz;
}

// �x�b�N�}�����z���v�Z����
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// �t���l�����v�Z�BSchlick�ߎ����g�p
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/**
* @brief �t���l�����˂��l�������g�U���˂��v�Z
* @param[in] float3(normal) �@��
* @param[in] float3(lightVector) �����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B
* @param[in] float3(eyeVector) �����Ɍ������x�N�g���B
* @param[in] float3(roughness) �e���B0�`1�͈̔́B
* details �t���l�����˂��ア�Ƃ��ɂ͊g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��͊g�U���˂��������Ȃ�
* @return float �g�U����
*/
float CalcDiffuseFromFresnel(float3 normal, float3 lightVector, float3 eyeVector, float roughness)
{
    //0�`1�͈̔͂Ɏ��߂�
    roughness = saturate(roughness);
    
    // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 halfVector = normalize(lightVector + eyeVector);

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotLH = saturate(dot(lightVector, halfVector));

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A
    // �������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // �@���ƌ����Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNL = saturate(dot(normal, lightVector));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNV = saturate(dot(normal, eyeVector));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (FL * FV * energyFactor);
}

/**
* @brief Cook-Torrance���f���̋��ʔ��˂��v�Z
* @param[in] float3(lightVector) �����Ɍ������x�N�g��
* @param[in] float3(eyeVector) ���_�Ɍ������x�N�g���B
* @param[in] float3(normal) �@���x�N�g���B
* @param[in] float3(metallic) �����x�B0�`1�͈̔́B
* @return float �g�U����
*/
float CookTorranceSpecular(float3 lightVector, float3 eyeVector, float3 normal, float metallic)
{
    float microfacet = 0.76f;

    // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
    // �����x�������قǃt���l�����˂͑傫���Ȃ�
    float f0 = metallic;

    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(lightVector + eyeVector);

    // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
    float NdotH = saturate(dot(normal, H));
    float VdotH = saturate(dot(eyeVector, H));
    float NdotL = saturate(dot(normal, lightVector));
    float NdotV = saturate(dot(normal, eyeVector));

    // D�����x�b�N�}�����z��p���Čv�Z����
    float D = Beckmann(microfacet, NdotH);

    // F����Schlick�ߎ���p���Čv�Z����
    float F = SpcFresnel(f0, VdotH);

    // G�������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m�������߂�
    float m = k_PI * NdotV * NdotH;

    // �����܂ŋ��߂��A�l�𗘗p���āACook-Torrance���f���̋��ʔ��˂����߂�
    return max(F * D * G / m, 0.0);
}


float3 CalcShadowRate(float3 worldPos, float3 albed)
{
    float shadow = 0.0f;
    float3 shadowColor = albed;

    float4 posInLVP = mul(float4(worldPos, 1.0f), shadowData.LightViewProjection);
    float2 shadowMapUV = posInLVP.xy / posInLVP.w;
    float zInLVP = posInLVP.z / posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float4x4 mat = mul(shadowData.LightViewProjection, shadowData.ShadowBiasMatrix);
    float4 SdwCoord = mul(float4(worldPos, 1.0f), mat);
    // �V���h�E�}�b�v�̐[�x�l�Ɣ�r����.
    float3 shadowCoord = SdwCoord.xyz / SdwCoord.w;
    
     // �ő�[�x�X�΂����߂�.
    float maxDepthSlope = max(abs(ddx(shadowCoord.z)), abs(ddy(shadowCoord.z)));

    float shadowThreshold = 1.0f; // �V���h�E�ɂ��邩�ǂ�����臒l
    float bias = 0.01f; // �Œ�o�C�A�X.
    float slopeScaledBias = 0.01f; // �[�x�X��
    float depthBiasClamp = 0.1f; // �o�C�A�X�N�����v�l

    float shadowBias = bias + slopeScaledBias * maxDepthSlope;
    shadowBias = min(shadowBias, depthBiasClamp);
    
    // �V���h�E�}�b�vUV���͈͓�������
    if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
        && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f)
    {
        //float shadowBias = 0.00001f; // �V���h�E�o�C�A�X
         // SampleCmpLevelZero()�֐����g�p���āA�Օ������擾����
        float shadow = g_ShadowMapTexture.SampleCmpLevelZero(
            g_ShadowMapSamplerState, // �g�p����T���v���[�X�e�[�g
            shadowMapUV, // �V���h�E�}�b�v�ɃA�N�Z�X����UV���W
            (SdwCoord.z - shadowBias) // ��r����Z�l�B���̒l����r����e�N�Z���̒l���
                                // �傫�����1.0�A���������0.0
                                // �����4�e�N�Z�����s���A4�e�N�Z���̕��ς�Ԃ��Ă���
        );

        // �V���h�E�J���[�ƒʏ�J���[���Օ����Ő��`��Ԃ���
        // �V���h�E�J���[���v�Z
        shadowColor *= 0.5f;

        // �Օ������g���Đ��`�⊮�A
        shadowColor = lerp(albed, shadowColor, shadow);
    }

    return shadowColor;
}

void main(in POS_UV_PS_IN In, out float4 outDiffuse : SV_Target)
{
    // �A���x�h�J���[
    float4 albedoColor = g_AlbedoColorTexture.Sample(g_SamplerState, In.TexCoord);
    // �X�؃L�����J���[(�A���x�h�J���[�ɂ���)
    float3 specularColor = albedoColor.xyz;
    // �@�����v�Z
    float3 normal = g_NormalTexture.Sample(g_SamplerState, In.TexCoord).xyz;
    //�@����0�`1�͈̔͂���A-1�`1�͈̔͂ɕ�������B
    normal = (normal * 2.0f) - 1.0f;
    // �����x
    float metallic = g_SpecularTexture.Sample(g_SamplerState, In.TexCoord).r;
    // ���炩��
    float smooth = g_SpecularTexture.Sample(g_SamplerState, In.TexCoord).a;
    // �r��
    float roughness = 1.0f - smooth;

    // �[�x���擾
    float depth = g_DepthTexture.Sample(g_SamplerState, In.TexCoord).r;
    
    // �[�x��񂩂烏�[���h���W���v�Z����B
    float3 worldPos = CaluclationWroldPosition(depth, In.TexCoord, drawData.CameraViewProjectionInverse);
        
    float3 toEye = normalize(drawData.CameraPosition - worldPos);
   
    // �@���ƃf�B���N�V���i�����C�g�̓��ς����߂�
    float ndDot = dot(normal, directionalLight.Direction);
    // �e臒l
    float ShadowThreshold = -0.2f;
    if (ndDot < ShadowThreshold)
    {
        // �e�̌v�Z
        albedoColor.rgb = CalcShadowRate(worldPos, albedoColor.rgb);
    }
    
    // �t���l�����˂��l�������g�U���˂��v�Z
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionalLight.Direction, toEye, roughness);

    // ���K��Lambert�g�U���˂����߂�
    float NdotL = saturate(dot(normal, -directionalLight.Direction));
    float3 lambertDiffuse = directionalLight.Color * NdotL / k_PI;
    
    // �ŏI�I�Ȋg�U���ˌ����v�Z����
    float3 diffuse = albedoColor.rgb * diffuseFromFresnel * lambertDiffuse;
    
    // Cook-Torrance���f���𗘗p�������ʔ��˗����v�Z����
    // Cook-Torrance���f���̋��ʔ��˗����v�Z����
    float3 specular = CookTorranceSpecular(-directionalLight.Direction, toEye, normal, smooth) * directionalLight.Color;

    // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
    // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
    specular *= lerp(float3(1.0f, 1.0f, 1.0f), specularColor, metallic);

    float3 lig = 0;
    // ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
    // ���炩����������΁A�g�U���˂͎キ�Ȃ�
    lig += diffuse * roughness + specular;
    
      // �����ɂ���グ
    lig += directionalLight.Ambient * albedoColor.rgb;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    outDiffuse = finalColor;
}