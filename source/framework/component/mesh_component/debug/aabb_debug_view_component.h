/**
* @file obb_debug_view_component.h
* @brief OBB�f�o�b�O�r���[
*/

#pragma once

#include"../mesh_component.h"

/**
* @brief �O���b�h�������R���|�[�l���g
*/
class AABBDebugViewComponent : public MeshComponent
{
private:
	//! �\�����邩�ǂ����̃t���O
	static bool m_IsUseFlag;

public:
	static void SetIsUseFlag(bool flag);
private:
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! ���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;

	//! ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! �C���f�b�N�X��
	UINT m_IndexNum;

	//! �|�W�V����
	Conv_XM::Vector3f m_Position;
	//! �X�P�[��
	Conv_XM::Vector3f m_Scale;

public:
	AABBDebugViewComponent(class Actor* owner);
	~AABBDebugViewComponent() {}

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const;

	inline void SetTransform(const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& scale)
	{
		m_Position = position;
		m_Scale = scale;
	}

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
