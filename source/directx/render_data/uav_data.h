/**
* @file uav_data.h
* @brief �A���I�[�_�[�A�N�Z�X�r���[�̏��W��
*/

#pragma once

/**
* @namespace SRVData
* @brief �A���I�[�_�[�A�N�Z�X�r���[�̏��
*/
namespace UAVData
{
	// uav�C���f�b�N�X�f�[�^
	enum Index
	{
		//! �p�[�e�B�N���̃C���X�^���X���C���f�b�N�X
		PARTICLEDRAWARGS_INDEX,
		//! �p�[�e�B�N�����C���f�b�N�X
		PARTICLEDATA_INDEX,
		//! ���S�p�[�e�B�N���C���f�b�N�X�̏��C���f�b�N�X
		DEADPARTICLE_INDEX,
		//! �����p�[�e�B�N���C���f�b�N�X�̏��C���f�b�N�X
		ALLIVEPARTICLE_INDEX,
	};
}
