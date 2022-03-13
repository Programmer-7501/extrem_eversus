
#include"../../system/main.h"
#include"model_manager.h"

Model * ModelManager::Load(const std::string & fileName)
{
	Model* Result = nullptr;

	// 読み込み済みか確認するため連想配列から探す
	auto iter = m_ModelMap.find(fileName);

	if (iter == m_ModelMap.end())
	{
		Result = new Model();

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
		m_ModelMap.emplace(fileName, Result);

		return Result;
	}
	else
	{
		// 検索結果を返す
		return iter->second;
	}
		
}

void ModelManager::Unload(const std::string fileName)
{
	// 読み込み済みか確認するため連想配列から探す
	auto&& iter = m_ModelMap.find(fileName);

	// 登録されていない
	if (iter == m_ModelMap.end())
	{
		Logger::GetInstance().SetLog("ModelManager::Unload 指定されたファイルがMapに登録されていない");
		return;
	}

	// nullptrだったら
	if (iter->second == nullptr)
	{
		Logger::GetInstance().SetLog("ModelManager::Unload modelクラスがnullptr");

		// マップから削除
		m_ModelMap.erase(iter);

		return;
	}

	iter->second->Unload();
	delete iter->second;
	iter->second = nullptr;

	// マップから削除
	m_ModelMap.erase(iter);
}

void ModelManager::AllRelease()
{
	for (auto&& iter : m_ModelMap)
	{
		// シェーダーリソースビュー解放
		if (iter.second)
		{
			iter.second->Unload();
			delete iter.second;
			iter.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("ModelManager::AllRelease シェーダーリソースビューがnullptr");
		}
	}
	m_ModelMap.clear();
}

Model * ModelManager::GetModel(const std::string fileName) const
{
	// Mapに登録されているか探す
	auto&& iter = m_ModelMap.find(fileName);

	// 登録されていたら
	if (iter != m_ModelMap.end())
	{
		// モデルクラスを渡すを渡す
		return iter->second;
	}

	Logger::GetInstance().SetLog("ModelManager::GetModel 指定されたファイルがMapに登録されていない");
	return false;
}
