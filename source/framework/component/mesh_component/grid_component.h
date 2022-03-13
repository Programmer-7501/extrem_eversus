/**
* @file grid_component.h
* @brief �O���b�h
*/

#pragma once

#include"mesh_component.h"

/**
* @brief �O���b�h�������R���|�[�l���g
*/
class GridComponent : public MeshComponent
{
private:
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! ���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;

	//! ���_��
	int m_VertexCount;

	//! ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
public:
	GridComponent(class Actor* owner);
	~GridComponent() {}

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const;

private:
	/**
	* @brief �f�[�^�ǂݍ���
	* @return bool �����������ǂ���
	*/
	virtual bool LoadData();

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
