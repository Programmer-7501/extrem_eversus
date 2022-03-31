/**
 * @file particle_component.h
 * @brief �p�[�e�B�N���R���|�[�l���g
 */

#pragma once

#include"../directx/render_data/cbuffer_data.h"

 /**
 * @brief �p�[�e�B�N���R���|�[�l���g
 */
class Particle
{
private:
	/**
	* @brief �p�[�e�B�N���f�[�^�\����
	*/
	struct ParticleData
	{
		//! ���W
		Conv_XM::Vector3f Position;
		//! ���x
		Conv_XM::Vector3f Velocity;
		//! ���C�t
		float Life;
		//! �T�C�Y
		Conv_XM::Vector2f Size;
		//! ���[���h�}�g���N�X
		Conv_XM::Matrix4x4f WorldMatrix;
		//! �J���[
		Conv_XM::Vector4f Color;
		//! �G�~�b�^�[�C���f�b�N�X
		int EmitterIndex;
	};
private:
	//! �C���X�^���X���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawArgs;
	//! ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! �C���X�^���X���UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_DrawArgsUAV;

	//! �p�[�e�B�N�����o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ParticleBuffer;

	//! �p�[�e�B�N�������o��UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_ParticleUAV;

	//! �p�[�e�B�N�����o��SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ParticleSRV;

	//! ���S�p�[�e�B�N���C���f�b�N�X
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DeadParticleIndexBuffer;

	//! ���S�p�[�e�B�N�����o��UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_DeadParticleIndexAppendConsumeUAV;

	//! �g�p�p�[�e�B�N���C���f�b�N�X
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_AlliveParticleIndexBuffer;

	//! �g�p�p�[�e�B�N�����o��UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_AlliveParticleIndexUAV;

	//! �g�p�p�[�e�B�N���o��SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AlliveParticleIndexSRV;

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

	//! �������R���s���[�g�V�F�[�_�[
	ID3D11ComputeShader* m_InitComputeShader;

	//! �X�V�R���s���[�g�V�F�[�_�[
	ID3D11ComputeShader* m_UpdateComputeShader;

	//! �V�[�������_�[�}�l�[�W���[
	class SceneRenderManager* m_SceneRenderManager;

	//! �p�[�e�B�N���̍ő吔(256�̔{���̂�)
	int m_MaxParticleNum;

	//! �X�V�f�B�X�p�b�`��
	int m_Dispatch;

	//! ���Z�����t���O
	bool m_BlendStateInvisibleAddFlag;
public:

	/**
	* @brief �R���X�g���N�^
	* @param[in] int(maxParticleNum) �p�[�e�B�N���̍ő吔
	* @details �p�[�e�B�N���̍ő吔��256�̔{���̂�
	*/
	Particle(int maxParticleNum);
	~Particle();

	/**
	* @brief ����������
	*/
	bool Init();

	/**
	* @brief �X�V����
	*/
	void Update();

	/**
	* @brief �`�揈��
	*/
	void Draw() const;

	/**
	* @brief �G�~�b�^�[�N��
	* @param[in] int(spawnNum) ����������p�[�e�B�N����
	* @details �p�[�e�B�N���}�l�[�W���[�ŃG�~�b�^�[�Ǝ��S�p�[�e�B�N�������R���X�^���g�o�b�t�@�ɃZ�b�g���Ă����K�v������
	*/
	void Emitter(int spawnNum);

	/**
	* @brief �e�N�X�`���ݒ�
	* @param[in] ID3D11ShaderResourceView*(srv) �e�N�X�`���̃V�F�[�_�[���\�[�X�r���[
	*/
	inline void SetTexture(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }

	/**
	* @brief ���S�p�[�e�B�N���C���f�b�N�XUAV���擾
	* @return ID3D11UnorderedAccessView* ���S�p�[�e�B�N���C���f�b�N�XUAV
	*/
	inline ID3D11UnorderedAccessView* GetDeadParticleIndexAppendConsumeUAV() const { return m_DeadParticleIndexAppendConsumeUAV.Get();}

	/**
	* @brief �����p�[�e�B�N���C���f�b�N�XUAV���擾
	* @return ID3D11UnorderedAccessView* �����p�[�e�B�N���C���f�b�N�XUAV
	*/
	inline ID3D11UnorderedAccessView* GetAlliveParticleIndexAppendConsumeUAV() const { return m_AlliveParticleIndexUAV.Get(); }

	/**
	* @brief �p�[�e�B�N���̍ő吔���擾
	* @return int �p�[�e�B�N���̍ő吔
	*/
	inline int GetMaxParticleNumber() const { return m_MaxParticleNum; }

	/**
	* @brief ���Z�����t���O�Z�b�g
	* @param[in] bool(flag) ���Z�����t���O
	*/
	inline void SetBlendStateInvisibleAddFlag(bool flag) { m_BlendStateInvisibleAddFlag = flag; }
private:

	/**
	* @brief �p�[�e�B�N���f�[�^������
	* @return bool �����������ǂ���
	*/
	bool InitParticleData();

	/**
	* @brief �p�[�e�B�N���f�[�^�X�V
	*/
	void UpdateParticle() const;

	/**
	* @brief �f�o�C�X�R���e�L�X�g�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetDeviceContext();

	/**
	* @brief �V�F�[�_�[�̎擾
	*/
	bool GetShader();
};