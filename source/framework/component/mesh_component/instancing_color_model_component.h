/**
* @file instancing_color_model_component.h
* @brief ���샂�f�����C���X�^���V���O�`�悷��t�@�C��
* @details �r���ŃC���X�^���V���O�f�[�^�ύX�\
* @details �e�`��͔�Ή�
*/

#pragma once

#include"mesh_component.h"

/**
* @brief ���샂�f�����C���X�^���V���O�`�悷��R���|�[�l���g
* @details �r���ŃC���X�^���V���O�f�[�^�ύX�\
*/
class InstancingColorModelComponent : public MeshComponent
{
public:
	struct InstanceColorData
	{
		Conv_XM::Matrix4x4f Matrix;
		Conv_XM::Vector4f Color;
	};
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
	std::vector<InstanceColorData> m_InstanceColorData;

	//! �X�V�t���O
	bool m_IsUpdate;
public:
	InstancingColorModelComponent(class Actor* owner);
	~InstancingColorModelComponent() {}

	/**
	* @brief �X�V����
	*/
	void Update()override;

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const override;

	/**
	* @brief �C���X�^���V���O�ő吔�Z�b�g
	* @details ����������O�Ɏg�p����
	*/
	inline void SetMaxInstancingNum(int maxInstancingNum) { m_MaxInstanceNum = maxInstancingNum; }

	/**
	* @brief �C���X�^���V���O�f�[�^�Z�b�g
	*/
	inline void SetInstanceData(const std::vector<InstanceColorData>& instanceData) 
	{
		m_IsUpdate = true;
		m_InstanceColorData = instanceData; 
	}


	// �Z�b�^�[
	inline void SetModel(class Model* model) { m_Model = model; }

private:

	/**
	* @brief �C���X�^���V���O�f�[�^�쐬
	* @return bool �����������ǂ���
	*/
	virtual bool CreateInstancingData();

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