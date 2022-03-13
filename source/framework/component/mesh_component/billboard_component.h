/**
 * @file billboard_component.h
 * @brief �r���{�[�h�R���|�[�l���g
 */

#pragma once

#include"mesh_component.h"

class BillBoardComponent : public MeshComponent
{
public:
	struct BillBoardTextureConfig
	{
		Conv_XM::Vector2f TexCut;//�e�N�X�`���؂�����W
		Conv_XM::Vector2f TexCutSize;//�e�N�X�`���؂���T�C�Y
	};

protected:
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

	//! �V�[�������_�[�}�l�[�W���[
	class SceneRenderManager* m_SceneRenderManager;

	//! �|�W�V����
	Conv_XM::Vector3f m_Position;
	//! �T�C�Y
	Conv_XM::Vector2f m_Size;

	//! �e�N�X�`���R���t�B�O
	BillBoardTextureConfig m_BillBoardTextureConfig;

	//! ���̃��b�N
	bool m_ShaftRock[3];

public:
	BillBoardComponent(class Actor* owner);
	virtual ~BillBoardComponent();

	void Draw() const override;

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }

	inline void SetPosition(const Conv_XM::Vector3f& position) { m_Position = position; }
	inline void SetSize(const Conv_XM::Vector2f& size) { m_Size = size; }

	inline void SetBillBoardTextureConfig(const BillBoardTextureConfig& biiBoardTextureConfig) { m_BillBoardTextureConfig = biiBoardTextureConfig; }

	// 0:X 1:Y 2:Z
	inline void SetShaftRock(int index) 
	{
		if (index < 0 || index > 2)
		{
			return;
		}
		m_ShaftRock[index] = true; 
	}
protected:
	/**
	* @brief �f�[�^�ǂݍ���
	* @return bool �����������ǂ���
	*/
	virtual bool LoadData() override;

	/**
	* @brief ���b�V���쐬
	* @return bool �����������ǂ���
	*/
	virtual bool CreateMesh();

	/**
	* @brief �f�o�C�X�R���e�L�X�g�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetDeviceContext();

	/**
	* @brief �V�F�[�_�[�̎擾
	* @return bool �����������ǂ���
	*/
	virtual bool GetShader();
};