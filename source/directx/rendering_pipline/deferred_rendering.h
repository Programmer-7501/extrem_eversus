/**
* @file deferred_rendering.h
* @brief �f�B�t�@�[�h�����_�����O
*/

#pragma once

/**
* @brief �f�B�t�@�[�h�����_�����O���s�N���X
*/
class DeferredRendering
{
private:
	//! �A���x�h�J���[�V�F�[�_�[���\�[�X�r���[
	ID3D11ShaderResourceView* m_AlbedColorSRV;
	//! �m�[�}���V�F�[�_�[���\�[�X�r���[
	ID3D11ShaderResourceView* m_NormalSRV;
	//! �X�؃L�����V�F�[�_�[���\�[�X�r���[
	ID3D11ShaderResourceView* m_SpecularSRV;
	//! �f�v�X�X�e���V���V�F�[�_�[���\�[�X�r���[
	ID3D11ShaderResourceView* m_DepthStancilSRV;

	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! ���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;

	//! �������J���[
	float m_Color[4];
public:
	DeferredRendering(
		ID3D11ShaderResourceView* albedColorSRV,
		ID3D11ShaderResourceView* normalSRV,
		ID3D11ShaderResourceView* specularSRV,
		ID3D11ShaderResourceView* depthstencilSRV,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);
	~DeferredRendering();


	/**
	* @brief ����������
	* @return bool �����������ǂ���
	*/
	bool Init();


	/**
	* @brief �f�B�t�@�[�h�����_�����O���s
	*/
	void Draw() const;

private:

	/**
	* @brief �V�F�[�_�[�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetShader();

	/**
	* @brief �|���S�����쐬
	* @return bool �����������ǂ���
	*/
	bool CreatePolygon();
};