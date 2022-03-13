/**
* @file lockon_cursol_billboard_component.h
* @brief ���b�N�I���J�[�\���r���{�[�h�R���|�[�l���g
*/

#pragma once

#include"mesh_component.h"

/**
* @brief ���b�N�I���J�[�\���r���{�[�h�R���|�[�l���g
*/
class LockonCursolBillboardComponent : public MeshComponent
{
public:
	enum LockOnState
	{
		LOCKONSTATE_RED,
		LOCKONSTATE_GREEN,
		LOCKONSTATE_YELLOW,
		LOCKONSTATE_MAX,
	};
private:
	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;

	//! �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! GBuffer���͗p���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;

	//! �e�N�X�`��
	ID3D11ShaderResourceView* m_ShaderResourceView[LOCKONSTATE_MAX];

	//! �|�W�V����
	Conv_XM::Vector3f m_Position;
	//! �T�C�Y
	Conv_XM::Vector2f m_Size;

	//! �V�[�������_�[�}�l�[�W���[
	class SceneRenderManager* m_SceneRenderManager;

	//! ���b�N�I�����
	LockOnState m_LockOnState;
public:
	LockonCursolBillboardComponent(class Actor* owner);
	~LockonCursolBillboardComponent() {}

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const override;

	inline void SetSRV(ID3D11ShaderResourceView* srv, LockOnState lockOnState) { m_ShaderResourceView[lockOnState] = srv; }
	inline void SetPosition(const Conv_XM::Vector3f& position) { m_Position = position; }
	inline void SetSize(const Conv_XM::Vector2f& size) { m_Size = size; }
	inline void SetLockOnState(LockOnState lockOnState) { m_LockOnState = lockOnState; }
	inline LockOnState GetLockOnState() const { return m_LockOnState; }
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