/**
* @file model_manager.h
* @brief モデル管理
*/

#pragma once

#include"../../function/singleton.h"
#include"model.h"

/**
* @brief モデル管理クラス
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
	* @brief モデル読み込み
	* @param[in] const std::string&(fileName) ファイルネーム
	* @return Model* モデルのポインタ
	* @details ファイルネームからモデルを読み込みマップに登録
	*/
	class Model* Load(const std::string& fileName);

	/**
	* @brief テクスチャ破棄
	* @param[in] const std::string&(fileName) ファイルネーム
	* @details ファイルネームからテクスチャをマップから破棄
	*/
	void Unload(const std::string fileName);

	/**
	* @brief モデル破棄
	* @details ファイルネームからテクスチャをマップから破棄
	*/
	void AllRelease();

	/**
	* @brief モデル取得
	* @return Model* モデルのポインタ
	*/
	class Model* GetModel(const std::string fileName) const;
};