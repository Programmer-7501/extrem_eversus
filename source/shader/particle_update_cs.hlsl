
#include "common.hlsl"

RWStructuredBuffer<uint> DrawArgs : register(u0);

RWStructuredBuffer<ParticleData> ParticleDatas : register(u1);

AppendStructuredBuffer<uint> DeadParticleIndex : register(u2);

RWStructuredBuffer<uint> AlliveParticleIndex : register(u3);

float4x4 MatrixIdentity()
{
    float4x4 mat;
    
    mat[0][0] = 1.0f;
    mat[0][1] = 0.0f;
    mat[0][2] = 0.0f;
    mat[0][3] = 0.0f;
    
    mat[1][0] = 0.0f;
    mat[1][1] = 1.0f;
    mat[1][2] = 0.0f;
    mat[1][3] = 0.0f;
    
    mat[2][0] = 0.0f;
    mat[2][1] = 0.0f;
    mat[2][2] = 1.0f;
    mat[2][3] = 0.0f;
    
    mat[3][0] = 0.0f;
    mat[3][1] = 0.0f;
    mat[3][2] = 0.0f;
    mat[3][3] = 1.0f;

    return mat;
}

float4x4 inverse(float4x4 m)
{
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

float4x4 MatrixTranslation(float3 v)
{
    float4x4 mat = MatrixIdentity();
    float x = v.x, y = v.y, z = v.z;
    mat[3][0] = x;
    mat[3][1] = y;
    mat[3][2] = z;
    return mat;
}

float4x4 MatrixScaling(float3 v)
{
    float4x4 mat = MatrixIdentity();
    float x = v.x, y = v.y, z = v.z;
    mat[0][0] = x;
    mat[1][1] = y;
    mat[2][2] = z;
    return mat;
}


[numthreads(PARTICLE_UPDATE_THREDNUM, 1, 1)]
void main(in CSInput IN)
{
    // 最初に描画引数の初期化を行う
    if (IN.Dispatch.x == 0)
    {
        DrawArgs[0] = 6;
        DrawArgs[1] = 0;
        DrawArgs[2] = 0;
        DrawArgs[3] = 0;
        DrawArgs[4] = 0;
    }
    
    // パーティクル配列からデータを取得
    ParticleData particleData = ParticleDatas[IN.Dispatch.x];

    // ライフがゼロ以上か判断
    if (particleData.Life > 0.0f)
    {
        // エミッターが使用を辞めたら終える
        if (particleEmitterData[particleData.EmitterIndex].IsUse <= 0)
        {
            DeadParticleIndex.Append(IN.Dispatch.x);
            particleData.Life = -1.0f;
        }
        
        // ライフを減らす
        particleData.Life--;

        // ライフがゼロ以下なら終了
        if (particleData.Life <= 0.0f)
        {
            DeadParticleIndex.Append(IN.Dispatch.x);
            particleData.Life = -1.0f;
        }
        else
        {
            if (particleEmitterData[particleData.EmitterIndex].SizeOverLifeTime > 0)
            {
                particleData.Size = lerp(float2(0.0f, 0.0f), particleEmitterData[particleData.EmitterIndex].Size, particleData.Life / particleEmitterData[particleData.EmitterIndex].ParticleLifeSpan);
            }
            
            particleData.Velocity.y -= particleEmitterData[particleData.EmitterIndex].Gravity;
            
            // 速度を加える
            particleData.Position += particleData.Velocity;
            
            float4x4 mtxT = MatrixTranslation(particleData.Position);
            float3 scale = float3(particleData.Size.x, particleData.Size.y, 1.0f);
            float4x4 mtxS = MatrixScaling(scale);
            
            float4x4 invView = drawData.View;
            invView[3][0] = 0.0f;
            invView[3][1] = 0.0f;
            invView[3][2] = 0.0f;
            invView[3][3] = 1.0f;
            invView = inverse(invView);

            float4x4 mtxSV = mul(mtxS, invView);
            particleData.WorldMatrix = mul(mtxSV, mtxT);

            
            uint index = AlliveParticleIndex.IncrementCounter();
            AlliveParticleIndex[index] = IN.Dispatch.x;
            uint dstIdx = 0;
            InterlockedAdd(DrawArgs[1], 1, dstIdx);
        }
    }
    
    ParticleDatas[IN.Dispatch.x] = particleData;
}