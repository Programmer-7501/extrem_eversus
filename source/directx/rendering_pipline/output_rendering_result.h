/**
* @file output_rendering_result.h
* @brief �����_�����O���ʕ\��
*/

#pragma once

/**
* @brief �����_�����O���ʕ\���N���X
*/
class OutPutRenderingResult
{
private:
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! �����_�����O���ʃV�F�[�_�[���\�[�X�r���[
	ID3D11ShaderResourceView* m_RenderingResultSRV;

	//! �������J���[
	float m_Color[4];


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
public:
	OutPutRenderingResult(
		ID3D11ShaderResourceView* renderingResultSRV,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);
	~OutPutRenderingResult();


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