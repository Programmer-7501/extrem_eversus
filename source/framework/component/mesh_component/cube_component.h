#pragma once

#include"mesh_component.h"

class CubeComponent : public MeshComponent
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
	CubeComponent(class Actor* owner);
	~CubeComponent();

	void Draw()const override;

	/**
	* @brief �e�`�揈��
	*/
	virtual void ShadowDraw() const;

private:
	/**
	* @brief �f�[�^�ǂݍ���
	* @return bool �����������ǂ���
	*/
	virtual bool LoadData();

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