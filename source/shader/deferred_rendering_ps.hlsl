
#include "common.hlsl"

Texture2D g_AlbedoColorTexture : register(t0); // アルベドカラー
Texture2D g_NormalTexture : register(t1); // 法線
Texture2D g_SpecularTexture : register(t2); // スぺキュラ強度
Texture2D g_DepthTexture : register(t3); // 深度値保存用

Texture2D g_ShadowMapTexture : register(t10); // シャドウマップ

sampler g_SamplerState : register(s0);

SamplerComparisonState g_ShadowMapSamplerState : register(s1); // シャドウマップ用サンプラーステート

float3 CaluclationWroldPosition(float depth, float2 texCood, float4x4 inverseViewProjectionMatrix)
{
    float4 worldPosition;

    // まず正規化スクリーン座標系での座標を計算する。
    // z座標は深度テクスチャから引っ張ってくる。
    worldPosition.z = depth;
    // xy座標はUV座標から計算する。
    worldPosition.xy = (texCood * float2(2.0f, -2.0f)) + float2(-1.0f, 1.0f);
    worldPosition.w = 1.0f;
    
    // ビュープロジェクション行列の逆行列を乗算して、ワールド座標に戻す。
    worldPosition = mul(worldPosition, inverseViewProjectionMatrix);
    worldPosition.xyz /= worldPosition.w;
    
    return worldPosition.xyz;
}

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/**
* @brief フレネル反射を考慮した拡散反射を計算
* @param[in] float3(normal) 法線
* @param[in] float3(lightVector) 光源に向かうベクトル。光の方向と逆向きのベクトル。
* @param[in] float3(eyeVector) 視線に向かうベクトル。
* @param[in] float3(roughness) 粗さ。0～1の範囲。
* details フレネル反射が弱いときには拡散反射が大きくなり、フレネル反射が強いときは拡散反射が小さくなる
* @return float 拡散反射
*/
float CalcDiffuseFromFresnel(float3 normal, float3 lightVector, float3 eyeVector, float roughness)
{
    //0～1の範囲に収める
    roughness = saturate(roughness);
    
    // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 halfVector = normalize(lightVector + eyeVector);

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
    float dotLH = saturate(dot(lightVector, halfVector));

    // 光源に向かうベクトルとハーフベクトル、
    // 光が平行に入射したときの拡散反射量を求めている
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // 法線と光源に向かうベクトルを利用して拡散反射率を求める
    float dotNL = saturate(dot(normal, lightVector));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // 法線と視点に向かうベクトルを利用して拡散反射率を求める
    float dotNV = saturate(dot(normal, eyeVector));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (FL * FV * energyFactor);
}

/**
* @brief Cook-Torranceモデルの鏡面反射を計算
* @param[in] float3(lightVector) 光源に向かうベクトル
* @param[in] float3(eyeVector) 視点に向かうベクトル。
* @param[in] float3(normal) 法線ベクトル。
* @param[in] float3(metallic) 金属度。0～1の範囲。
* @return float 拡散反射
*/
float CookTorranceSpecular(float3 lightVector, float3 eyeVector, float3 normal, float metallic)
{
    float microfacet = 0.76f;

    // 金属度を垂直入射の時のフレネル反射率として扱う
    // 金属度が高いほどフレネル反射は大きくなる
    float f0 = metallic;

    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(lightVector + eyeVector);

    // 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(normal, H));
    float VdotH = saturate(dot(eyeVector, H));
    float NdotL = saturate(dot(normal, lightVector));
    float NdotV = saturate(dot(normal, eyeVector));

    // D項をベックマン分布を用いて計算する
    float D = Beckmann(microfacet, NdotH);

    // F項をSchlick近似を用いて計算する
    float F = SpcFresnel(f0, VdotH);

    // G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m項を求める
    float m = k_PI * NdotV * NdotH;

    // ここまで求めた、値を利用して、Cook-Torranceモデルの鏡面反射を求める
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
    // シャドウマップの深度値と比較する.
    float3 shadowCoord = SdwCoord.xyz / SdwCoord.w;
    
     // 最大深度傾斜を求める.
    float maxDepthSlope = max(abs(ddx(shadowCoord.z)), abs(ddy(shadowCoord.z)));

    float shadowThreshold = 1.0f; // シャドウにするかどうかの閾値
    float bias = 0.01f; // 固定バイアス.
    float slopeScaledBias = 0.01f; // 深度傾斜
    float depthBiasClamp = 0.1f; // バイアスクランプ値

    float shadowBias = bias + slopeScaledBias * maxDepthSlope;
    shadowBias = min(shadowBias, depthBiasClamp);
    
    // シャドウマップUVが範囲内か判定
    if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
        && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f)
    {
        //float shadowBias = 0.00001f; // シャドウバイアス
         // SampleCmpLevelZero()関数を使用して、遮蔽率を取得する
        float shadow = g_ShadowMapTexture.SampleCmpLevelZero(
            g_ShadowMapSamplerState, // 使用するサンプラーステート
            shadowMapUV, // シャドウマップにアクセスするUV座標
            (SdwCoord.z - shadowBias) // 比較するZ値。この値が比較するテクセルの値より
                                // 大きければ1.0、小さければ0.0
                                // それを4テクセル分行い、4テクセルの平均を返してくる
        );

        // シャドウカラーと通常カラーを遮蔽率で線形補間する
        // シャドウカラーを計算
        shadowColor *= 0.5f;

        // 遮蔽率を使って線形補完、
        shadowColor = lerp(albed, shadowColor, shadow);
    }

    return shadowColor;
}

void main(in POS_UV_PS_IN In, out float4 outDiffuse : SV_Target)
{
    // アルベドカラー
    float4 albedoColor = g_AlbedoColorTexture.Sample(g_SamplerState, In.TexCoord);
    // スぺキュラカラー(アルベドカラーにする)
    float3 specularColor = albedoColor.xyz;
    // 法線を計算
    float3 normal = g_NormalTexture.Sample(g_SamplerState, In.TexCoord).xyz;
    //法線を0～1の範囲から、-1～1の範囲に復元する。
    normal = (normal * 2.0f) - 1.0f;
    // 金属度
    float metallic = g_SpecularTexture.Sample(g_SamplerState, In.TexCoord).r;
    // 滑らかさ
    float smooth = g_SpecularTexture.Sample(g_SamplerState, In.TexCoord).a;
    // 荒さ
    float roughness = 1.0f - smooth;

    // 深度を取得
    float depth = g_DepthTexture.Sample(g_SamplerState, In.TexCoord).r;
    
    // 深度情報からワールド座標を計算する。
    float3 worldPos = CaluclationWroldPosition(depth, In.TexCoord, drawData.CameraViewProjectionInverse);
        
    float3 toEye = normalize(drawData.CameraPosition - worldPos);
   
    // 法線とディレクショナルライトの内積を求める
    float ndDot = dot(normal, directionalLight.Direction);
    // 影閾値
    float ShadowThreshold = -0.2f;
    if (ndDot < ShadowThreshold)
    {
        // 影の計算
        albedoColor.rgb = CalcShadowRate(worldPos, albedoColor.rgb);
    }
    
    // フレネル反射を考慮した拡散反射を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionalLight.Direction, toEye, roughness);

    // 正規化Lambert拡散反射を求める
    float NdotL = saturate(dot(normal, -directionalLight.Direction));
    float3 lambertDiffuse = directionalLight.Color * NdotL / k_PI;
    
    // 最終的な拡散反射光を計算する
    float3 diffuse = albedoColor.rgb * diffuseFromFresnel * lambertDiffuse;
    
    // Cook-Torranceモデルを利用した鏡面反射率を計算する
    // Cook-Torranceモデルの鏡面反射率を計算する
    float3 specular = CookTorranceSpecular(-directionalLight.Direction, toEye, normal, smooth) * directionalLight.Color;

    // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
    // スペキュラカラーの強さを鏡面反射率として扱う
    specular *= lerp(float3(1.0f, 1.0f, 1.0f), specularColor, metallic);

    float3 lig = 0;
    // 滑らかさを使って、拡散反射光と鏡面反射光を合成する
    // 滑らかさが高ければ、拡散反射は弱くなる
    lig += diffuse * roughness + specular;
    
      // 環境光による底上げ
    lig += directionalLight.Ambient * albedoColor.rgb;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    outDiffuse = finalColor;
}