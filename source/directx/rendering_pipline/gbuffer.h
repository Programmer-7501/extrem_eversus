/**
* @file gbuffer_input.h
* @brief GBuffer����
* @details renderer�Ŏg�p�����
*/

#pragma once

/**
* @brief GBuffer���̓N���X
*/
class GBuffer
{
private:
	//! �A���x�h�J���[�����_�[�^�[�Q�b�g�r���[
	ID3D11RenderTargetView* m_AlbedColorRTV;

	//! �m�[�}�������_�[�^�[�Q�b�g�r���[
	ID3D11RenderTargetView* m_NormalRTV;

	//! �X�؃L���������_�[�^�[�Q�b�g�r���[
	ID3D11RenderTargetView* m_SpecularRTV;

	//! �X�؃L���������_�[�^�[�Q�b�g�r���[
	ID3D11RenderTargetView* m_depthRTV;

	//! ���C���f�v�X�X�e���V��
	ID3D11DepthStencilView* m_MainDSV;

	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! �������J���[
	float m_Color[4];
public:
	GBuffer(
		ID3D11RenderTargetView* albedColorRTV,
		ID3D11RenderTargetView* normalRTV,
		ID3D11RenderTargetView* specularRTV,
		ID3D11RenderTargetView * depthRTV,
		ID3D11DepthStencilView* mainDS,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);

	~GBuffer();

	/**
	* @brief ����������
	* @return bool �����������ǂ���
	*/
	bool Init();

	/**
	* @brief GBuffer���͊J�n
	*/
	void InputBegin();

	/**
	* @brief GBuffer���͏I��
	*/
	void InputEnd();
};