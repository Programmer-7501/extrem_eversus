/**
 * @file demo_scene.h
 * @brief �e�X�g�V�[��
 */

#pragma once

#include"../framework/scene/scene.h"
#include"../directx/render_data/cbuffer_data.h"

class DemoScene : public Scene
{
private:
	//! �}�e���A���ύX�p���f���ϐ�
	CBufferData::Material m_Material;
	class ModelComponent* m_SphereModel;
	float m_Metaric;
	float m_Smoothness;

	//! ���ڌ��ύX�p�ϐ�
	class DirectionalLightComponent* m_DirectionalLight;
	CBufferData::DirectionalLight m_DirectionalLightData;
	float m_Ambient[3];
	float m_Color[3];
	float m_Direction[3];

	//! �p�[�e�B�N���f�[�^
	int m_MaxParticleNum;

	//! �p�[�e�B�N���G�~�b�^�[
	bool m_ParticleUse;
	CBufferData::ParticleEmitter m_Emitter001;
	CBufferData::ParticleEmitter m_Emitter002;
	CBufferData::ParticleEmitter m_Emitter003;

	//! �t�F�[�h�p
	class Fade* m_Fade;

	//! �V�[���`�F���W�\��t���O
	bool m_IsSceneChange;
public:
	DemoScene();
	~DemoScene();

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
