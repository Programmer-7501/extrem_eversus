/**
* @file forward_rendering.h
* @brief �t�H���[�h�����_�����O
*/

#pragma once

/**
* @brief �t�H���[�h�����_�����O���s�N���X
*/
class ForwardRendering
{
private:
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! ���C�������_�[�^�[�Q�b�g�r���[
	ID3D11RenderTargetView* m_MainRTV;

	//! ���C���f�v�X�X�e���V��
	ID3D11DepthStencilView* m_MainDSV;

	//! �������J���[
	float m_Color[4];
public:
	ForwardRendering(
		ID3D11RenderTargetView* mainRTV,
		ID3D11DepthStencilView* mainDSV,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);
	~ForwardRendering();

	/**
	* @brief ����������
	* @return bool �����������ǂ���
	*/
	bool Init();

	/**
	* @brief �t�H���[�h�����_�����O�J�n
	*/
	void Begin() const;

	/**
	* @brief �t�H���[�h�����_�����O�I��
	*/
	void End() const;
};