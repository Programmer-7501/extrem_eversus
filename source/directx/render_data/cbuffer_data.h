/**
* @file cbuffer_data.h
* @brief �R���X�^���g�o�b�t�@�̏��W��
*/

#pragma once

#include"../../shader/shader_data.h"

/**
* @namespace CBufferData
* @brief �R���X�^���g�o�b�t�@�̏��
*/
namespace CBufferData
{
	enum Index
	{
		//! ���[���h�s��̃C���f�b�N�X�ԍ�
		WORLDMATRIX_INDEX,
		//! �`��f�[�^�̃C���f�b�N�X�ԍ�
		DRAWDATA_INDEX,
		//! �p�[�e�B�N���G�~�b�^�[�z��̃C���f�b�N�X�ԍ�
		PARTICLEEMITTERARRAY_INDEX,
		//! ���S�p�[�e�B�N�����̃C���f�b�N�X�ԍ�
		DEADPARTICLENUMBER_INDEX,
		//! �f�B���N�V���i�����C�g�̃C���f�b�N�X�ԍ�
		DIRECTIONALLIGHT_INDEX,
		//! �|�C���g���C�g�̃C���f�b�N�X�ԍ�
		POINTLIGHT_INDEX,
		//! �}�e���A���f�[�^�̃C���f�b�N�X�ԍ�
		MATERIAL_INDEX,
		//! ���C�g�J�����̃C���f�b�N�X�ԍ�
		LIGHTVIEWPROJECTION_INDEX,
		//! �g�p�p�[�e�B�N���G�~�b�^�[�̃C���f�b�N�X�ԍ�
		USEPARTICLEEMITTER_INDEX,
		//! �{�[���f�[�^�̃C���f�b�N�X�ԍ�
		BONE_INDEX,
		//! �C���f�b�N�X��
		MAX_INDEX,
	};


	struct DrawData
	{
		Conv_XM::Matrix4x4f View;
		Conv_XM::Matrix4x4f Projection;
		Conv_XM::Matrix4x4f CameraViewProjectionInverse;
		Conv_XM::Vector3f CameraPosition;
		float RandomNumber;
	};

	struct BONES
	{
		Conv_XM::Matrix4x4f bones[MAX_BONE_NUM];
	};

	enum EmitterType
	{
		//! �X�t�B�A�^�ɔ���
		ET_SPHERE,
		//! ���N�g�^�ɔ���
		ET_RECT,
		//! �f���p
		ET_DEMO,
		ET_MAX,
	};

	/**
	* @brief �G�~�b�^�[�f�[�^�\����
	*/
	struct ParticleEmitter
	{
		//! �G�~�b�^�[�̃|�W�V����
		Conv_XM::Vector3f EmitterPosition;
		//! ���C�t�ɂ���ăT�C�Y�����炷���ǂ���
		UINT SizeOverLifeTime;

		//! �p�[�e�B�N���̑��x
		Conv_XM::Vector3f ParticleVelocity;
		float pad2;

		//! �p�[�e�B�N���̐����|�W�V�����̂΂��
		Conv_XM::Vector3f PositionVariance;
		float pad3;

		//! �J���[
		Conv_XM::Vector4f Color;

		//! ���̃t���[���ɂ�����p�[�e�B�N���̍ő吶����
		int MaxSpawnParticlesThisFrame;
		//! ���̃t���[���̐�������
		float ParticleLifeSpan;
		//! �g�p���邩�ۂ�
		int IsUse;
		//! ���g�̃C���f�b�N�X�ԍ�
		UINT MyIndex;

		//! ���x
		float Speed;
		//! �G�~�b�^�[�̎��
		UINT EmitterType;
		//! �p�[�e�B�N���̃T�C�Y
		Conv_XM::Vector2f Size;

		// ���x�̂΂��
		Conv_XM::Vector3f VelocityVariance;
		// �d��
		float Gravity;
	};

	/**
	* @brief �f�B���N�V���i�����C�g�f�[�^�\����
	*/
	struct DirectionalLight
	{
		//! �f�B���N�V���i�����C�g�̊p�x
		Conv_XM::Vector3f Direction;
		float pad0;
		//! �f�B���N�V���i�����C�g�̐F
		Conv_XM::Vector3f Color;
		float pad1;
		//! �A���r�G���g���C�g
		Conv_XM::Vector3f Ambient;
		float pad2;
	};

	/**
	* @brief �|�C���g���C�g�f�[�^�\����
	*/
	struct PointLight
	{
		//! �|�C���g���C�g�̃|�W�V����
		Conv_XM::Vector3f Position;
		float pad;
		//! �|�C���g���C�g�̐F
		Conv_XM::Vector3f Color;
		//! �|�C���g���C�g�̗L���͈�
		float Range;
	};

	//!	�}�e���A���f�[�^
	struct Material
	{
		Conv_XM::Vector4f Ambient;
		Conv_XM::Vector4f Diffuse;
		float Metaric;
		float Smoothness;
		Conv_XM::Vector2f pad;
	};

	struct ShadowData
	{
		Conv_XM::Matrix4x4f LightViewProjection;
		Conv_XM::Matrix4x4f ShadowBiasMatrix;
	};
}
