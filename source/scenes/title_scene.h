/**
 * @file title_scene.h
 * @brief �^�C�g���V�[��
 */

#pragma once

#include"../framework/scene/scene.h"
#include"../directx/render_data/cbuffer_data.h"

class TitleScene : public Scene
{
private:
	//! �V�[���`�F���W�\��t���O
	bool m_IsSceneChange;

	//! �t�F�[�h
	class Fade* m_Fade;

	//! �p�[�e�B�N���G�~�b�^�[
	CBufferData::ParticleEmitter m_Emitter001;
	CBufferData::ParticleEmitter m_Emitter002;

	//! �r�[���T�[�x��
	class BeamSaber* beamSaber001;
	class BeamSaber* beamSaber002;

	//! �A�j���[�V�������f���R���|�[�l���g
	class AnimationModelComponent* animeModelComp001;
	class AnimationModelComponent* animeModelComp002;

	//! �t���[���J�E���g
	int m_Frame;

	//! ���ڌ�
	class DirectionalLightComponent* directionalLightComponent;

	CBufferData::DirectionalLight directionalLightData;

	//! �J����
	class QuaternionCameraComponent* quaternionCameraComponent;
public:
	TitleScene();
	~TitleScene();

private:
	/**
	* @brief �V�[���œǂݍ��ރf�[�^����
	*/
	void LoadData() override;

	/**
	* @brief �V�[���Ǝ��̔j��
	*/
	void UnloadData() override;

	/**
	* @brief �V�[���ŏ������O�ɏ�����͂��鏈��
	*/
	void SetData()override;

	/**
	* @brief �V�[���Ǝ��̍X�V����
	*/
	void UpdateScene() override;

	/**
	* @brief �V�[���ŗp������@�\�̐���
	*/
	void CreateSceneFunction() override;

	/**
	* @brief �V�[���ŗp����ꂽ�@�\�̍폜
	*/
	void UnloadSceneFunction() override;
};
