/**
* @file model.h
* @brief ���샂�f���t�@�C��
*/

#pragma once

#include"../render_data/vertex_structure.h"
#include"../render_data/cbuffer_data.h"

/**
* @brief ���샂�f���������N���X
*/
class Model final
{
private:
	/**
	* @brief ���b�V���f�[�^�\����
	*/
	struct MeshData
	{
		std::vector<VERTEX_NORMAL_TEXCOOD> Vertices;
		std::vector<UINT> Indices;
		CBufferData::Material Material;
		std::string TextureName;

		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		ID3D11ShaderResourceView* TextureData;
	};

protected:
	//! �t�@�C����
	std::string m_FileName;
	//! ���b�V�����X�g
	std::vector<MeshData> m_MeshList;
	//! �f�o�C�X
	ID3D11Device* m_Device;
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;
public:
	Model();
	~Model();

	/**
	* @brief ���샂�f���̓ǂݍ���
	* @param[in] const std::string&(fileName) �t�@�C����
	* @return bool �ǂݍ��݂������������ǂ���
	*/
	bool Load(const std::string& fileName);

	/**
	* @brief ���샂�f���̔j��
	*/
	void Unload();

	/**
	* @brief ���샂�f���̕`��
	*/
	void Draw() const;


	/**
	* @brief ���샂�f���̕`��
	* @details �C���X�^���V���O�p
	* @details Matrix�݂̂̃C���X�^���V���O�p
	*/
	void InstancingDraw(ID3D11Buffer* instancingBuffer, int instancingNum) const;


	/**
	* @brief ���샂�f���̕`��
	* @details �F�t���C���X�^���V���O�p
	* @details �F�t���̃C���X�^���V���O�p
	*/
	void InstancingColorDraw(ID3D11Buffer* instancingBuffer, int instancingNum, UINT* stride, UINT* offset) const;

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