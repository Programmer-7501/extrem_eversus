/**
* @file particle_manager.h
* @brief �p�[�e�B�N���Ǘ�
*/

#pragma once

#include"../function/singleton.h"
#include"../directx/render_data/cbuffer_data.h"
#include"../shader/shader_data.h"

namespace ParticleManagerData
{
	//! �r�[���p�[�e�B�N���̖��O
	static const char* k_BeamParticleName = "BeamParticle";

	//! �r�[���p�[�e�B�N���̍ő吔
	static constexpr int k_MaxBeamRifleParticleNum = 16384;
}

/**
* @brief �p�[�e�B�N���Ǘ��N���X
*/
class ParticleManager final : public Singleton<ParticleManager>
{
private:
	//! �p�[�e�B�N���}�b�v
	std::unordered_map<std::string, class Particle*> m_ParticleMap;

	//! �G�~�b�^�[�\����
	CBufferData::ParticleEmitter m_Emitters[EMITTER_MAXNUM] = {};

	//! �G�~�b�^�[�C���f�b�N�X�J�E���g
	int m_EmitterIndexCount = 0;

	//! �G�~�b�^�[�z��̃R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_EmitterArrayConstantBuffer = nullptr;

	//! ���S�p�[�e�B�N�������̓R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> DeadParticleNumConstantBuffer = nullptr;

	//! �g�p�G�~�b�^�[�R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_EmitterConstantBuffer = nullptr;


	//! �����p�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RandomTexture;
	//! �����p�e�N�X�`��SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RandomTextureSRV;

	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext = nullptr;

	//! �G�~�b�^�[�z����X�V����K�v�����邩�ǂ���
	bool m_EmitterArrayIsUpdate = true;


	//! �G�~�b�^�[�R���s���[�g�V�F�[�_�[
	ID3D11ComputeShader* m_EmitterComputeShader[CBufferData::ET_MAX] = {};


	// �f�o�b�O�p�̃o�b�t�@
	ID3D11Buffer* m_DebugCounterBuffer = nullptr;

	// �f�o�b�O�p�t���O
	bool m_IsDebugCountView = false;

private:
	friend class Singleton<ParticleManager>;

	ParticleManager() = default;
	~ParticleManager() = default;

public:

	/**
	* @brief ����������
	*/
	void Init();

	/**
	* @brief �I������
	*/
	void Uninit();

	/**
	* @brief ���Z�b�g����
	*/
	void Reset();


	/**
	* @brief �p�[�e�B�N���̔���
	* @param[in] const std::string&(particleName) �p�[�e�B�N���l�[��
	* @param[in] int(emitterIndex) �p�[�e�B�N���G�~�b�^�[�C���f�b�N�X�ԍ�
	*/
	void SpawnParticle(const std::string& particleName, int emitterIndex);


	/**
	* @brief �X�V����
	*/
	void Update();


	/**
	* @brief �`�揈��
	*/
	void Draw() const;

	/**
	* @brief �G�~�b�^�[�z��̃R���X�^���g�o�b�t�@�̍X�V
	* @details 
	*/
	void UpdateParticleEmitterArrayConstantBuffer();

	/**
	* @brief �p�[�e�B�N���̍쐬�A�o�^
	* @param[in] const std::string&(particleName) �p�[�e�B�N���l�[��
	* @param[in] int(maxParticleNum) �p�[�e�B�N���̍ő吔
	* @param[in] ID3D11ShaderResourceView*(srv) �p�[�e�B�N���Ɏg�p����e�N�X�`��
	*/
	void CreateParticle(const std::string& particleName,int maxParticleNum, ID3D11ShaderResourceView* srv, bool invisibleAddFlag = true);

	/**
	* @brief �G�~�b�^�[�̃R���X�^���g�o�b�t�@�̍X�V
	* @param[in] int(emitterIndex) �p�[�e�B�N���G�~�b�^�[�C���f�b�N�X�ԍ�
	* @param[in] const CBufferData::ParticleEmitter&(particleEmitter) �p�[�e�B�N���G�~�b�^�[�\����
	*/
	void UpdateParticleEmitterConstantBuffer(int emitterIndex, const CBufferData::ParticleEmitter& particleEmitter);

	/**
	* @brief �p�[�e�B�N���G�~�b�^�[�o�^
	* @param[in] const CBufferData::ParticleEmitter&(particleEmitter) �p�[�e�B�N���G�~�b�^�[
	* @return �o�^���ꂽ�C���f�b�N�X�ԍ�(�G���[�Ȃ�-1)
	* @details �������������Ɏg�p����
	*/
	inline int RegisterParticleEmitter(const CBufferData::ParticleEmitter& particleEmitter)
	{
		if (m_EmitterIndexCount >= EMITTER_MAXNUM)
		{
			Logger::GetInstance().SetLog("ParticleManager::SetParticleEmitter �ő吔�𒴂����G�~�b�^�[�̓o�^");
			return -1;
		}

		// �G�~�b�^�[��o�^
		m_Emitters[m_EmitterIndexCount] = particleEmitter;
		m_Emitters[m_EmitterIndexCount].MyIndex = m_EmitterIndexCount;
		m_EmitterIndexCount++;

		return (m_EmitterIndexCount - 1);
	}


	inline void SetDebugCountView(bool flag) { m_IsDebugCountView = flag; }

private:

	/**
	* @brief �G�~�b�^�[�̃R���X�^���g�o�b�t�@�̐ݒ�
	* @param[in] int(emitterIndex) �p�[�e�B�N���G�~�b�^�[�C���f�b�N�X�ԍ�
	* @return int �p�[�e�B�N��������
	*/
	int SetParticleEmitterConstantBuffer(int emitterIndex);

	/**
	* @brief UAV�̃J�E���g�ǂݍ���
	* @param[in] ID3D11UnorderedAccessView*(uav) �ǂݍ���UAV
	* @return int �J�E���g��
	*/
	int ReadCounter(ID3D11UnorderedAccessView* uav);
};