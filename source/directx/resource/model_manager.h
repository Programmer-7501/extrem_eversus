/**
* @file model_manager.h
* @brief ���f���Ǘ�
*/

#pragma once

#include"../../function/singleton.h"
#include"model.h"

/**
* @brief ���f���Ǘ��N���X
*/
class ModelManager final : public Singleton<ModelManager>
{
public:
	std::unordered_map<std::string, class Model*> m_ModelMap;
private:
	friend class Singleton<ModelManager>;

	ModelManager() = default;
	~ModelManager() = default;

public:

	/**
	* @brief ���f���ǂݍ���
	* @param[in] const std::string&(fileName) �t�@�C���l�[��
	* @return Model* ���f���̃|�C���^
	* @details �t�@�C���l�[�����烂�f����ǂݍ��݃}�b�v�ɓo�^
	*/
	class Model* Load(const std::string& fileName);

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
	class Model* GetModel(const std::string fileName) const;
};