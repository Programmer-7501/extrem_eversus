/**
* @file model_component.h
* @brief ���샂�f���t�@�C��
*/

#pragma once

#include"mesh_component.h"

/**
* @brief ���샂�f���������R���|�[�l���g
*/
class ModelComponent : public MeshComponent
{
private:
	//! ���샂�f���N���X
	class Model* m_Model;

	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! GBuffer���͗p���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;

	//! �I�t�Z�b�g�T�C�Y
	Conv_XM::Vector3f m_OffsetScale;

	//! �I�t�Z�b�g���[�e�[�V����
	Conv_XM::Vector4f m_OffsetQuaternion;

public:
	ModelComponent(class Actor* owner);
	~ModelComponent() {}

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const override;

	/**
	* @brief �e�`�揈��
	*/
	virtual void ShadowDraw() const override;

	// �Z�b�^�[
	inline void SetModel(class Model* model) { m_Model = model; }

	inline void SetOffsetScale(const Conv_XM::Vector3f scale) { m_OffsetScale = scale; }

	inline void SetOffsetQuaternion(const Conv_XM::Vector4f quaternion) { m_OffsetQuaternion = quaternion; }

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