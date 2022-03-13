/**
* @file instancing_model_component.h
* @brief ���샂�f�����C���X�^���V���O�`�悷��t�@�C��
*/

#pragma once

#include"mesh_component.h"

/**
* @brief ���샂�f�����C���X�^���V���O�`�悷��R���|�[�l���g
*/
class InstancingModelComponent : public MeshComponent
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

	//! �ő吔
	int m_MaxInstanceNum;

	//! �C���X�^���V���O�}�g���N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceMatrixBuffer;

	//! �C���X�^���V���O�}�g���N�X�f�[�^
	std::vector<Conv_XM::Matrix4x4f> m_InstanceMatrixData;
public:
	InstancingModelComponent(class Actor* owner);
	~InstancingModelComponent() {}

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const override;

	/**
	* @brief �e�`�揈��
	*/
	virtual void ShadowDraw() const override;

	/**
	* @brief �C���X�^���V���O�}�g���N�X�f�[�^�Z�b�g
	* @details ����������O�Ɏg�p����
	*/
	void SetInstanceMatrixData(const std::vector<Conv_XM::Matrix4x4f>& instanceMatrixData);

	// �Z�b�^�[
	inline void SetModel(class Model* model) { m_Model = model; }

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