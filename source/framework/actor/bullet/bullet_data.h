/**
 * @file bullet_data.h
 * @brief �e�̃f�[�^
 */

#pragma once

namespace BeamRifleData
{
	//! �f�t�H���g�̑��x
	static constexpr float k_DefaultSpeed = 0.75f;

	//! �f�t�H���g�̏I���t���[��
	static constexpr int k_DefaultFinishFrame = 300;

	//! �e�̐F
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.8f, 1.0f);
	//! 1�t���[�����ɔ���������p�[�e�B�N����
	static constexpr int k_DefaultSpawnNum = 2;
	//! ���C�t
	static constexpr float k_DefaultLife = 5.0f;
	//! �p�[�e�B�N���T�C�Y
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(1.0f, 1.0f);
	//! �g�p�G�~�b�^�[�^�C�v
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_SPHERE;

	//! �ő��]��
	static constexpr float k_DefaultMaxRotate = 0.25f;
}

namespace BazookaData
{
	//! ���f���t�@�C����
	static const char* k_ModelFileName = "asset/model/bazooka/bazooka.bin";

	//! �f�t�H���g�̑��x
	static constexpr float k_DefaultSpeed = 0.75f;

	//! ���f���̃I�t�Z�b�g�X�P�[��
	static const Conv_XM::Vector3f k_OffsetModelScale = Conv_XM::Vector3f(0.2f, 0.2f, 0.2f);
}