/**
* @file model_component.h
* @brief ���샂�f���t�@�C��
*/

#pragma once

#include"mesh_component.h"

/**
* @brief ���샂�f���������R���|�[�l���g
*/
class AnimationModelComponent : public MeshComponent
{
private:
	//! ���샂�f���N���X
	class AnimationModel* m_AnimationModel;

	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;

	//! ���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;

	//! ���݂̃A�j���[�V�����̖��O
	std::string m_CurrentAnimeName;

	//! �I�t�Z�b�g�T�C�Y
	Conv_XM::Vector3f m_OffsetScale;

	//! �I�t�Z�b�g���[�e�[�V����
	Conv_XM::Vector4f m_Quaternion;

	//!�t���[���J�E���g
	int m_CurrentFrame;

	//! �ő�J�E���g
	int m_MaxCount;

	//! ��~�t���O
	bool m_StopFlag;

	//! �Đ����x�p�ςݏグ��J�E���g
	float m_Counter;

	//! �J�E���g�𑝂₷���x
	float m_CounterSpeed;

	//! �u�����h���[�h
	bool m_IsBlendMode;

	//! �u�����h����
	int m_MaxBlendCount;

	//! �u�����h���Ԍv���^�C�}�[
	int m_BlendCount;

	//! �u�����h����A�j���[�V�����̖��O
	std::string m_BlendAnime001Name;

	//! �u�����h����A�j���[�V�����̃J�E���g
	int m_BlendAnime001Count;

	//! �u�����h����A�j���[�V�����̍ő�J�E���g
	int m_BlendAnime001MaxCount;

	//! �u�����h����A�j���[�V�����̖��O
	std::string m_BlendAnime002Name;

	//! �u�����h����A�j���[�V�����̃J�E���g
	int m_BlendAnime002Count;

	//! �u�����h����A�j���[�V�����̍ő�J�E���g
	int m_BlendAnime002MaxCount;
public:
	AnimationModelComponent(class Actor* owner);
	~AnimationModelComponent() {}

	/**
	* @brief �X�V����
	*/
	virtual void Update() override;

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const override;

	/**
	* @brief �e�`�揈��
	*/
	virtual void ShadowDraw() const override;


	/**
	* @brief ���݂̃A�j���[�V�����ƃu�����h(�u�����h�I����͓��͂����A�j���[�V����
	* @param[in] const std::string&(blendAnimationName) �u�����h�A�j���[�V����(����ɐݒ肷��A�j���[�V����)
	* @param[in] float(time) �u�����h����
	*/
	void SetCurrentAnimationBlendToNextAnimation(const std::string& blendAnimationName, int time);

	/**
	* @brief �g�p�p�[�e�B�N���̖��O�̃Z�b�g
	* @param[in] const std::string&(blendAnimationName001) �u�����h�A�j���[�V����
	* @param[in] const std::string&(blendAnimationName002) �u�����h�A�j���[�V����(����ɐݒ肷��A�j���[�V����)
	* @param[in] float(time) �u�����h����
	*/
	void SetBlendAnimation(const std::string& blendAnimationName001, const std::string& blendAnimationName002, int time);

	// �Z�b�^�[
	void SetAnimationModel(class AnimationModel* animationModel);

	void SetAnimation(const std::string& name);

	void ResetAnimation(const std::string& name);

	inline void SetOffsetScale(const Conv_XM::Vector3f scale) { m_OffsetScale = scale; }

	inline void SetOffsetQuaternion(const Conv_XM::Vector4f quaternion) { m_Quaternion = quaternion; }

	inline void SetCounetSpeed(float counterSpeed) { m_CounterSpeed = counterSpeed; }

	inline void SetCurrentFrame(int currentFrame) { m_CurrentFrame = currentFrame; }

	void SetIsIdentity(bool flag);

	/**
	* @brief �A�j���[�V������~
	* @param[in] bool(flag) �~�߂邩�ǂ���
	* @param[in] int(time) ��~����
	*/
	void StopModel(bool flag, int time = 0);
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