/**
* @file srv_data.h
* @brief �V�F�[�_�[���\�[�X�r���[�̏��W��
*/

#pragma once

/**
* @namespace SRVData
* @brief �V�F�[�_�[���\�[�X�r���[�̏��
*/
namespace SRVData
{
	//! �f�t�H���g�̃V�F�[�_�[���\�[�X�r���[�C���f�b�N�X
	static constexpr int k_DefaultShaderResourceViewIndex = 0;

	static constexpr int k_ParticleDataShaderResourceViewIndex = 1;
	static constexpr int k_AlliveParticleIndexShaderResourceViewIndex = 2;

	//! �����e�N�X�`���p�̃C���f�b�N�X
	static constexpr int k_RandomTextureShaderResourceViewIndex = 3;

	//! ���b�V���t�B�[���h�p�e�N�X�`��
	static constexpr int k_MeshField001ShaderResourceViewIndex = 4;
	//! ���b�V���t�B�[���h�p�e�N�X�`��
	static constexpr int k_MeshField002ShaderResourceViewIndex = 5;


	//! �V���h�E�}�b�v�p�V�F�[�_�[���\�[�X�r���[�C���f�b�N�X
	static constexpr int k_ShadowMapShaderResourceViewIndex = 10;
}
