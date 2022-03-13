/**
* @file shadowinput.h
* @brief �e����
*/

#pragma once

/**
* @brief �e���̓N���X
*/
class ShadowInput
{
private:
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! �V���h�E�C���v�b�g�f�v�X�X�e���V��
	ID3D11DepthStencilView* m_ShadowInputDSV;

public:
	ShadowInput(ID3D11DepthStencilView* shadowInputDSV);
	~ShadowInput();

	/**
	* @brief ����������
	* @return bool �����������ǂ���
	*/
	bool Init();

	/**
	* @brief �e���͊J�n
	*/
	void Begin();

	/**
	* @brief �e���͏I��
	*/
	void End();
};
