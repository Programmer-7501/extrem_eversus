
#include"../../system/main.h"
#include "animation_model_manager.h"

AnimationModel * AnimationModelManager::Load(const std::string & fileName)
{
	AnimationModel* Result = nullptr;

	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_AnimationModelMap.find(fileName);

	if (iter == m_AnimationModelMap.end())
	{
		Result = new AnimationModel();

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
		m_AnimationModelMap.emplace(fileName, Result);

		return Result;
	}
	else
	{
		// �������ʂ�Ԃ�
		return iter->second;
	}

}

void AnimationModelManager::Unload(const std::string fileName)
{
	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_AnimationModelMap.find(fileName);

	// �o�^����Ă��Ȃ�
	if (iter == m_AnimationModelMap.end())
	{
		return;
	}

	// nullptr��������
	if (iter->second == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelManager::Unload model�N���X��nullptr");

		// �}�b�v����폜
		m_AnimationModelMap.erase(iter);

		return;
	}

	delete iter->second;
	iter->second = nullptr;

	// �}�b�v����폜
	m_AnimationModelMap.erase(iter);
}

void AnimationModelManager::AllRelease()
{
	for (auto&& iter : m_AnimationModelMap)
	{
		// �V�F�[�_�[���\�[�X�r���[���
		if (iter.second)
		{
			delete iter.second;
			iter.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("AnimationModelManager::AllRelease �V�F�[�_�[���\�[�X�r���[��nullptr");
		}
	}
	m_AnimationModelMap.clear();
}

AnimationModel * AnimationModelManager::GetModel(const std::string fileName) const
{
	// Map�ɓo�^����Ă��邩�T��
	auto&& iter = m_AnimationModelMap.find(fileName);

	// �o�^����Ă�����
	if (iter != m_AnimationModelMap.end())
	{
		// ���f���N���X��n����n��
		return iter->second;
	}

	Logger::GetInstance().SetLog("AnimationModelManager::GetModel �w�肳�ꂽ�t�@�C����Map�ɓo�^����Ă��Ȃ�");
	return false;
}
