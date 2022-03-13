/**
* @file animation_model_manager.h
* @brief モデル管理
*/

#pragma once

#include"../../function/singleton.h"
#include"animation_model.h"

/**
* @brief アニメーションモデル管理クラス
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
	* @brief モデル読み込み
	* @param[in] const std::string&(fileName) ファイルネーム
	* @return AnimationModel* モデルのポインタ
	* @details ファイルネームからモデルを読み込みマップに登録
	*/
	class AnimationModel* Load(const std::string& fileName);

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
	class AnimationModel* GetModel(const std::string fileName) const;
};