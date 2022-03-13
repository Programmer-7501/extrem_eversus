/**
* @file mesh_component.h
* @brief ���샂�f���t�@�C��
*/

#pragma once

#include"../component.h"
#include"../../../directx/render_data/cbuffer_data.h"

/**
* @brief 3D���f���������R���|�[�l���g
*/
class MeshComponent : public Component
{
public:
	//! �����_�����O���@
	enum RenderingWay
	{
		//! �f�B�t�@�[�h�����_�����O(�f�t�H���g)
		RENDERING_DEFERRED,
		//! �t�H���[�h�����_�����O
		RENDERING_FORWARD,
	};
protected:
	//! �����邩�ǂ���
	bool m_Visible;

	//! �����_�����O���@
	RenderingWay m_RenderingWay;

	//! �e���͂��邩�ǂ���
	bool m_IsShadowInput;

	//! �}�e���A��
	CBufferData::Material m_Material;
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();


	/**
	* @brief ����������
	* @details ����ȍ~�̏�����������LoadData�ɂ����邽�߃I�[�o�[���C�h�s�ɂ���
	*/
	void Init() final override;

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const = 0;

	/**
	* @brief �e�`�揈��
	*/
	virtual void ShadowDraw() const;

	// �Z�b�^�[�A�Q�b�^�[
	inline void SetVisible(bool flag) { m_Visible = flag; }
	inline bool GetVisible() const { return m_Visible; }

	inline void SetRenderingWay(RenderingWay renderingWay) { m_RenderingWay = renderingWay; }
	inline RenderingWay GetRenderingWay() const { return m_RenderingWay; }

	inline void SetIsShadowInput(bool flag) { m_IsShadowInput = flag; }
	inline bool GetIsShadowInput() const { return m_IsShadowInput; }

	inline void SetMaterial(const CBufferData::Material& material) { m_Material = material; }
private:
	/**
	* @brief �V�[�������_�[�}�l�[�W���[�̓o�^
	*/
	bool RegisterSceneRenderManager();

	/**
	* @brief �f�[�^�ǂݍ���
	*/
	virtual bool LoadData();
};