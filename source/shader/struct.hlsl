
#include"shader_data.h"

struct DrawData
{
    float4x4 View;
    float4x4 Projection;
    float4x4 CameraViewProjectionInverse;
    float3 CameraPosition;
    float RandomNumber;
};

/**
* @brief �p�[�e�B�N���f�[�^�\����
*/
struct ParticleData
{
	// ���W
    float3 Position;
	// ���x
    float3 Velocity;
	// ���C�t
    float Life;
	// �T�C�Y
    float2 Size;
	// ���[���h�}�g���N�X
    float4x4 WorldMatrix;
	// �J���[
    float4 Color;
    //! �G�~�b�^�[�C���f�b�N�X
    int EmitterIndex;
};

/**
* @brief �p�[�e�B�N���G�~�b�^�[�\����
*/
struct ParticleEmitterData
{
    // �G�~�b�^�[�|�W�V����
    float3 EmitterPosition;
    // ���C�t�ɂ���ăT�C�Y�����������邩�ǂ���
    uint SizeOverLifeTime;
    
    // �p�[�e�B�N�����x
    float3 ParticleVelocity;
    float pad2;
    
    // �|�W�V�����̂΂��
    float3 PositionVariance;
    float pad3;
    
    // �F
    float4 Color;
    
    //�@���̃t���[���ɔ���������p�[�e�B�N����
    int MaxSpawnParticlesThisFrame;
    //�@�p�[�e�B�N���̃��C�t�^�C��
    float ParticleLifeSpan;
    // �g�p���邩�ǂ���
    int IsUse;
    // ���g�̃C���f�b�N�X�ԍ�
    uint MyIndex;
    
    //�@���x
    float Speed;
    // �G�~�b�^�[�̎��
    uint EmitterType;
    // �p�[�e�B�N���̃T�C�Y
    float2 Size;
    
    // ���x�̂΂��
    float3 VelocityVariance;
    // �d��
    float Gravity;
};

/**
* @brief �f�B���N�V���i�����C�g�f�[�^�\����
*/
struct DirectionalLightData
{
	//! �f�B���N�V���i�����C�g�̊p�x
    float3 Direction;
    float pad0;
	//! �f�B���N�V���i�����C�g�̐F
    float3 Color;
    float pad1;
	//! �A���r�G���g���C�g
    float3 Ambient;
    float pad2;
};


/**
* @brief �|�C���g���C�g�f�[�^�\����
*/
struct PointLightData
{
	//! �|�C���g���C�g�̃|�W�V����
    float3 Position;
    float pad;
	//! �|�C���g���C�g�̐F
    float3 Color;
	//! �|�C���g���C�g�̗L���͈�
    float Range;
};

/**
* @brief �}�e���A���\����
*/
struct MaterialData
{
    float4 Ambient;
    float4 Diffuse;
    float Metaric;
    float Smoothness;
    float2 pad;
};

struct ShadowData
{
    float4x4 LightViewProjection;
    float4x4 ShadowBiasMatrix;
};