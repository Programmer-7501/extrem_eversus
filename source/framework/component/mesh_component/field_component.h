#pragma once

#include"mesh_component.h"

class FieldComponent : public MeshComponent
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;
	//! ���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;
	//! �e�N�X�`��
	ID3D11ShaderResourceView* m_ShaderResourceView;
public:
	FieldComponent(class Actor* owner);
	~FieldComponent();

	void Draw() const override;

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }
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