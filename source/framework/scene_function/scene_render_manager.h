/**
 * @file scene_render_manager.h
 * @brief �V�[���̕`��Ǘ�
 */

#pragma once

 /**
 * @brief �V�[���̕`��Ǘ��N���X
 */
class SceneRenderManager
{
private:
	//! �X�v���C�g�R���|�[�l���g
	std::vector<class SpriteComponent*> m_SpriteComponents;

	//! �e���͂��郁�b�V���R���|�[�l���g
	std::vector<class MeshComponent*> m_ShadowInputMeshComponents;

	//! �f�B�t�@�[�h�����_�����O���郁�b�V���R���|�[�l���g
	std::vector<class MeshComponent*> m_DeferredRenderingMeshComponents;

	//! �t�H���[�h�����_�����O���郁�b�V���R���|�[�l���g
	std::vector<class MeshComponent*> m_ForwardRenderingMeshComponents;

	//! �J�����R���|�[�l���g
	class CameraComponent* m_MainCameraComponent;

	//! ���C�g�J�����R���|�[�l���g
	class CameraComponent* m_LightCameraComponent;

	//! ���Ԍv���N���X
	class TimeMeasurement* m_TimeMeasure;
public:
	SceneRenderManager();
	~SceneRenderManager();

	/**
	* @brief ����������
	*/
	void Init();

	/**
	* @brief �I������
	*/
	void Uninit();

	/**
	* @brief �`�揈��
	*/
	void Draw() const;

	/**
	* @brief �X�v���C�g�R���|�[�l���g�ǉ��֐�
	* @param[in] class SpriteComponent*(spriteComponent) �X�v���C�g�R���|�[�l���g
	*/
	void AddSpriteComponent(class SpriteComponent* spriteComponent);

	/**
	* @brief ���b�V���R���|�[�l���g�ǉ��֐�
	* @param[in] class MeshComponent*(meshComponent) ���b�V���R���|�[�l���g
	*/
	void AddMeshComponent(class MeshComponent* meshComponent);

	/**
	* @brief ���C���J�����R���|�[�l���g�̓o�^
	* @param[in] class CameraComponent*(cameraComponent) �J�����R���|�[�l���g
	*/
	inline void SetMainCameraComponent(class CameraComponent* mainCameraComponent) { m_MainCameraComponent = mainCameraComponent; }

	/**
	* @brief ���C�g�J�����R���|�[�l���g�̓o�^
	* @param[in] class CameraComponent*(cameraComponent) �J�����R���|�[�l���g
	*/
	inline void SetLightCameraComponent(class CameraComponent* lightCameraComponent) { m_LightCameraComponent = lightCameraComponent; }

	/**
	* @brief �J�����R���|�[�l���g�̎擾
	* @return class CameraComponent* �J�����R���|�[�l���g�̎擾
	*/
	inline class CameraComponent* GetMainCameraComponent() const { return m_MainCameraComponent; }

private:

	/**
	* @brief 3D�`��
	*/
	void Draw3D() const;

	/**
	* @brief 2D�`��
	*/
	void Draw2D() const;

	/**
	* @brief �V���h�E�}�b�v����
	*/
	void InputShadow() const;

	/**
	* @brief GBuffer����
	*/
	void InputGBuffer() const;

	/**
	* @brief �t�H���[�h�����_�����O���s
	*/
	void FowardRenderingExcute() const;
};