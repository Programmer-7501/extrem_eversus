/**
* @file animation_model.h
* @brief ����A�j�����f���t�@�C��
*/

#pragma once

#include"../render_data/vertex_structure.h"
#include"../render_data/cbuffer_data.h"

/**
* @brief ����A�j�����f���������N���X
*/
class AnimationModel final
{
private:
	/**
	* @brief ���b�V���f�[�^�\����
	*/
	struct AnimationMeshData
	{
		std::vector<ANIMATION_VERTEX_3D> Vertices;
		std::vector<UINT> Indices;
		std::string TextureName;

		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		ID3D11ShaderResourceView* TextureData;
	};

	struct FRAMEDATA
	{
		int Frame;
		std::vector<Conv_XM::Matrix4x4f> Bones;
	};
protected:
	//! �t�@�C����
	std::string m_FileName;
	//! ���b�V�����X�g
	std::vector<AnimationMeshData> m_MeshList;
	//! �f�o�C�X
	ID3D11Device* m_Device;
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! �A�j���[�V�����f�[�^
	std::unordered_map<std::string, std::vector<FRAMEDATA>> m_Animation_Datas;

	//! ���݂̃A�j���[�V�����f�[�^
	std::vector<FRAMEDATA> m_CurrentAnimeData;

	//! �t�@�C������ǂݍ��񂾌��݂̃{�[���f�[�^
	std::vector<Conv_XM::Matrix4x4f> m_CurrentBones;

	//! ����̃t���[���Ŏg�p�����{�[���f�[�^
	CBufferData::BONES m_Bones;

	//!�t���[���J�E���g
	int m_CurrentFrame;

	//! �������Ȃ�
	bool m_IsIdentity;

public:
	AnimationModel();
	virtual ~AnimationModel();

	/**
	* @brief ���샂�f���̓ǂݍ���
	* @param[in] const std::string&(fileName) �t�@�C����
	* @return bool �ǂݍ��݂������������ǂ���
	*/
	virtual bool Load(const std::string& fileName);

	/**
	* @brief ���샂�f���̔j��
	*/
	virtual void Unload();

	/**
	* @brief ���샂�f���̍X�V
	*/
	virtual void Update();

	/**
	* @brief ���샂�f���̕`��
	*/
	virtual void Draw() const;

	/**
	* @brief �s��̃Z�b�g
	* @param[in] int(frame) �A�j���[�V�����t���[��
	* @return int ���݂̃A�j���[�V�����t���[��
	*/
	int SetMatrix(const std::string& animationName, int frame);

	/**
	* @brief �s��̃Z�b�g(�u�����h��)
	* @param[in] const std::string&(animation001Name) �A�j���[�V�����l�[��
	* @param[in] int(frame001) �A�j���[�V�����t���[��
	* @param[in] const std::string&(animation001Name) �A�j���[�V�����l�[��
	* @param[in] int(frame002) �A�j���[�V�����t���[��
	* @param[in] float(t) ��ԌW��(0 �` 1)
	* @return int ���݂̃A�j���[�V�����t���[��
	*/
	int SetBlendMatrix(const std::string& animation001Name, int frame001, const std::string& animation002Name, int frame002, float t);

	///**
	//* @brief �s��̃Z�b�g(���݂̃A�j���[�V�����ƃu�����h��)
	//* @param[in] const std::string&(animation001Name) �A�j���[�V�����l�[��
	//* @param[in] int(frame001) �A�j���[�V�����t���[��
	//* @param[in] float(t) ��ԌW��(0 �` 1)
	//* @return int ���݂̃A�j���[�V�����t���[��
	//*/
	//int SetBlendMatrix(const std::string& blendanimationName, int frame, float t);


	int GetMaxFrame(const std::string& animationName);

	inline void SetIsIdentity(bool flag) { m_IsIdentity = flag; }

protected:
	/**
	* @brief ���샂�f���̍X�V
	* @return bool ���_�o�b�t�@�̍쐬�������������ǂ���
	*/
	bool CreateVertexBuffer();

	/**
	* @brief ���샂�f���̍X�V
	* @return bool �C���f�b�N�X�o�b�t�@�̍쐬�������������ǂ���
	*/
	bool CreateIndexBuffer();

	/**
	* @brief ���샂�f���̍X�V
	* @return bool �e�N�X�`���̓ǂݍ��݂������������ǂ���
	*/
	bool LoadTexture();
};