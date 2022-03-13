
#include"../../system/main.h"
#include"model_manager.h"

Model * ModelManager::Load(const std::string & fileName)
{
	Model* Result = nullptr;

	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_ModelMap.find(fileName);

	if (iter == m_ModelMap.end())
	{
		Result = new Model();

		// �ǂݍ��ݎ��s
		if (Result->Load(fileName) == false)
		{
			Logger::GetInstance().SetLog("ModelManager::Load ���f�����[�h���s");

			if (Result)
			{
				delete Result;
				Result = nullptr;
			}

			return Result;
		}

	
		// �o�^����
		m_ModelMap.emplace(fileName, Result);

		return Result;
	}
	else
	{
		// �������ʂ�Ԃ�
		return iter->second;
	}
		
}

void ModelManager::Unload(const std::string fileName)
{
	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto&& iter = m_ModelMap.find(fileName);

	// �o�^����Ă��Ȃ�
	if (iter == m_ModelMap.end())
	{
		Logger::GetInstance().SetLog("ModelManager::Unload �w�肳�ꂽ�t�@�C����Map�ɓo�^����Ă��Ȃ�");
		return;
	}

	// nullptr��������
	if (iter->second == nullptr)
	{
		Logger::GetInstance().SetLog("ModelManager::Unload model�N���X��nullptr");

		// �}�b�v����폜
		m_ModelMap.erase(iter);

		return;
	}

	iter->second->Unload();
	delete iter->second;
	iter->second = nullptr;

	// �}�b�v����폜
	m_ModelMap.erase(iter);
}

void ModelManager::AllRelease()
{
	for (auto&& iter : m_ModelMap)
	{
		// �V�F�[�_�[���\�[�X�r���[���
		if (iter.second)
		{
			iter.second->Unload();
			delete iter.second;
			iter.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("ModelManager::AllRelease �V�F�[�_�[���\�[�X�r���[��nullptr");
		}
	}
	m_ModelMap.clear();
}

Model * ModelManager::GetModel(const std::string fileName) const
{
	// Map�ɓo�^����Ă��邩�T��
	auto&& iter = m_ModelMap.find(fileName);

	// �o�^����Ă�����
	if (iter != m_ModelMap.end())
	{
		// ���f���N���X��n����n��
		return iter->second;
	}

	Logger::GetInstance().SetLog("ModelManager::GetModel �w�肳�ꂽ�t�@�C����Map�ɓo�^����Ă��Ȃ�");
	return false;
}
