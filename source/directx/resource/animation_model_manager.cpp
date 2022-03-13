
#include"../../system/main.h"
#include "animation_model_manager.h"

AnimationModel * AnimationModelManager::Load(const std::string & fileName)
{
	AnimationModel* Result = nullptr;

	// 読み込み済みか確認するため連想配列から探す
	auto iter = m_AnimationModelMap.find(fileName);

	if (iter == m_AnimationModelMap.end())
	{
		Result = new AnimationModel();

		// 読み込み失敗
		if (Result->Load(fileName) == false)
		{
			Logger::GetInstance().SetLog("ModelManager::Load モデルロード失敗");

			if (Result)
			{
				delete Result;
				Result = nullptr;
			}

			return Result;
		}


		// 登録する
		m_AnimationModelMap.emplace(fileName, Result);

		return Result;
	}
	else
	{
		// 検索結果を返す
		return iter->second;
	}

}

void AnimationModelManager::Unload(const std::string fileName)
{
	// 読み込み済みか確認するため連想配列から探す
	auto iter = m_AnimationModelMap.find(fileName);

	// 登録されていない
	if (iter == m_AnimationModelMap.end())
	{
		return;
	}

	// nullptrだったら
	if (iter->second == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelManager::Unload modelクラスがnullptr");

		// マップから削除
		m_AnimationModelMap.erase(iter);

		return;
	}

	delete iter->second;
	iter->second = nullptr;

	// マップから削除
	m_AnimationModelMap.erase(iter);
}

void AnimationModelManager::AllRelease()
{
	for (auto&& iter : m_AnimationModelMap)
	{
		// シェーダーリソースビュー解放
		if (iter.second)
		{
			delete iter.second;
			iter.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("AnimationModelManager::AllRelease シェーダーリソースビューがnullptr");
		}
	}
	m_AnimationModelMap.clear();
}

AnimationModel * AnimationModelManager::GetModel(const std::string fileName) const
{
	// Mapに登録されているか探す
	auto&& iter = m_AnimationModelMap.find(fileName);

	// 登録されていたら
	if (iter != m_AnimationModelMap.end())
	{
		// モデルクラスを渡すを渡す
		return iter->second;
	}

	Logger::GetInstance().SetLog("AnimationModelManager::GetModel 指定されたファイルがMapに登録されていない");
	return false;
}
