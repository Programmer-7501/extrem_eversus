#pragma once

#include"mesh_component.h"
#include"../../../directx/render_data/vertex_structure.h"

class MeshFieldComponent : public MeshComponent
{
public:
	//! �Ńe�N�X�`��
	static ID3D11ShaderResourceView* m_Grass_ShaderResourceView;
	//! ��e�N�X�`��
	static ID3D11ShaderResourceView* m_Rock_ShaderResourceView;
	//! �{�[�_�[�e�N�X�`��
	static ID3D11ShaderResourceView* m_Border_ShaderResourceView;

	/**
	* @brief �e�N�X�`���ǂݍ���
	*/
	static void LoadTexture();

	/**
	* @brief �e�N�X�`���j��
	*/
	static void UnloadTexture();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND* m_Vertex;
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;
	//! ���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;

	//! X�̒��_��
	int m_X_Num;
	//! Z�̒��_��
	int m_Z_Num;
	//! X�̒��_�Ԋu
	float m_X_Length;
	//! Z�̒��_�Ԋu
	float m_Z_Length;

	//! �����}�b�v
	std::vector<std::vector<float>> m_HeightMap;
	//! �e�N�X�`���u�����h
	std::vector<std::vector<float>> m_TextureBlend1;
	//! �e�N�X�`���u�����h
	std::vector<std::vector<float>> m_TextureBlend2;
	//! �e�N�X�`���u�����h
	std::vector<std::vector<float>> m_TextureBlend3;
	//! �e�N�X�`���u�����h
	std::vector<std::vector<float>> m_TextureBlend4;

	//! �^�C�g�����ǂ���
	bool m_IsTitle;
public:
	MeshFieldComponent(class Actor* owner);
	~MeshFieldComponent();

	void Draw() const override;

	float GetHeight(const Conv_XM::Vector3f& position);

	/**
	* @brief �����t�@�C���ǂݍ���
	* @param[in] const std::string&(fileName) �t�@�C����
	*/
	void LoadHeightMapFile(const std::string& fileName);

	/**
	* @brief �e�N�X�`���u�����h�t�@�C���ǂݍ���
	* @param[in] const std::string&(fileName) �t�@�C����
	* @param[in] int(blendNo) �u�����h�e�N�X�`���i���o�[
	*/
	void LoadTextureBlendFile(const std::string& fileName, int blendNo);

	/**
	* @brief ���b�V���t�B�[���h�̃p�����[�^�[�ݒ�
	* @param[in] int(x_num) X�̒��_��
	* @param[in] int(z_num) Z�̒��_��
	* @param[in] float(x_length) X�̒��_�Ԋu
	* @param[in] float(z_length) Z�̒��_�Ԋu
	*/
	inline void SetParameter(int x_num, int z_num, float x_length, float z_length)
	{
		m_X_Num = x_num;
		m_Z_Num = z_num;
		m_X_Length = x_length;
		m_Z_Length = z_length;
	}

	inline void SetIsTitle(bool flag) { m_IsTitle = flag; }
private:
	/**
	* @brief �f�[�^�ǂݍ���
	* @return bool �����������ǂ���
	*/
	bool LoadData() override;

	/**
	* @brief ���b�V���쐬
	* @return bool �����������ǂ���
	*/
	bool CreateMesh();

	/**
	* @brief �f�o�C�X�R���e�L�X�g�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetDeviceContext();

	/**
	* @brief �V�F�[�_�[�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetShader();
};