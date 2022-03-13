/**
* @file animation_model_manager.h
* @brief ���f���Ǘ�
*/

#pragma once

#include"../../function/singleton.h"
#include"animation_model.h"

/**
* @brief �A�j���[�V�������f���Ǘ��N���X
*/
class AnimationModelManager final : public Singleton<AnimationModelManager>
{
public:
	std::unordered_map<std::string, class AnimationModel*> m_AnimationModelMap;
private:
	friend class Singleton<AnimationModelManager>;

	AnimationModelManager() = default;
	~AnimationModelManager() = default;

public:

	/**
	* @brief ���f���ǂݍ���
	* @param[in] const std::string&(fileName) �t�@�C���l�[��
	* @return AnimationModel* ���f���̃|�C���^
	* @details �t�@�C���l�[�����烂�f����ǂݍ��݃}�b�v�ɓo�^
	*/
	class AnimationModel* Load(const std::string& fileName);

	/**
	* @brief �e�N�X�`���j��
	* @param[in] const std::string&(fileName) �t�@�C���l�[��
	* @details �t�@�C���l�[������e�N�X�`�����}�b�v����j��
	*/
	void Unload(const std::string fileName);

	/**
	* @brief ���f���j��
	* @details �t�@�C���l�[������e�N�X�`�����}�b�v����j��
	*/
	void AllRelease();

	/**
	* @brief ���f���擾
	* @return Model* ���f���̃|�C���^
	*/
	class AnimationModel* GetModel(const std::string fileName) const;
};