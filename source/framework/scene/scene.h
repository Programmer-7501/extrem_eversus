/**
 * @file scene.h
 * @brief �V�[��
 */

#pragma once

#include"../actor_manager/actor_manager.h"

 /**
 * @brief �V�[���N���X
 */
class Scene
{
protected:
	//! �A�N�^�[�W����
	std::vector<class ActorManager*> m_ActorManager;

	//! �V�[�������_�[�}�l�[�W���[
	class SceneRenderManager* m_SceneRenderManager;

	//! �R���W�����}�l�[�W���[
	class SceneCollisionManager* m_SceneCollisionManager;
public:
	Scene();
	~Scene();

	/**
	* @brief ����������
	*/
	void Init();

	/**
	* @brief �I������
	*/
	void Uninit();

	/**
	* @brief �X�V����
	*/
	void Update();

	/**
	* @brief �`�揈��
	*/
	void Draw() const;

	/**
	* @brief �A�N�^�[�}�l�[�W���[�����֐�
	* @return T �쐬�����A�N�^�[�}�l�[�W���[
	*/
	template<typename T>
	T* CreateActorManager()
	{
		// �A�N�^�[�}�l�[�W���[�𐶐�
		T* actorManager = new T;
		// null�`�F�b�N
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::CreateObjectManager actorManager��nullptr");
			return nullptr;
		}
		// �A�N�^�[�}�l�[�W���[��ɓo�^
		m_ActorManager.emplace_back(actorManager);

		// �A�N�^�[�}�l�[�W���[��Ԃ�
		return actorManager;
	}

	// �Q�b�^�[
	inline class SceneRenderManager* GetSceneRenderManager() const { return m_SceneRenderManager; }
	inline class SceneCollisionManager* GetSceneCollisionManager() const { return m_SceneCollisionManager; }
private:

	/**
	* @brief �V�[���œǂݍ��ރf�[�^����
	*/
	virtual void LoadData();

	/**
	* @brief �V�[���Ǝ��̔j��
	*/
	virtual void UnloadData();

	/**
	* @brief �V�[���ŏ������O�ɏ�����͂��鏈��
	*/
	virtual void SetData();

	/**
	* @brief �V�[���Ǝ��̍X�V����
	*/
	virtual void UpdateScene();

	/**
	* @brief �V�[���ŗp������@�\�̐���
	*/
	virtual void CreateSceneFunction();

	/**
	* @brief �V�[���ŗp����ꂽ�@�\�̍폜
	*/
	virtual void UnloadSceneFunction();
};