/**
 * @file scene.h
 * @brief シーン
 */

#pragma once

#include"../actor_manager/actor_manager.h"

 /**
 * @brief シーンクラス
 */
class Scene
{
protected:
	//! アクター集合体
	std::vector<class ActorManager*> m_ActorManager;

	//! シーンレンダーマネージャー
	class SceneRenderManager* m_SceneRenderManager;

	//! コリジョンマネージャー
	class SceneCollisionManager* m_SceneCollisionManager;
public:
	Scene();
	~Scene();

	/**
	* @brief 初期化処理
	*/
	void Init();

	/**
	* @brief 終了処理
	*/
	void Uninit();

	/**
	* @brief 更新処理
	*/
	void Update();

	/**
	* @brief 描画処理
	*/
	void Draw() const;

	/**
	* @brief アクターマネージャー生成関数
	* @return T 作成したアクターマネージャー
	*/
	template<typename T>
	T* CreateActorManager()
	{
		// アクターマネージャーを生成
		T* actorManager = new T;
		// nullチェック
		if (actorManager == nullptr)
		{
			Logger::GetInstance().SetLog("Scene::CreateObjectManager actorManagerがnullptr");
			return nullptr;
		}
		// アクターマネージャー列に登録
		m_ActorManager.emplace_back(actorManager);

		// アクターマネージャーを返す
		return actorManager;
	}

	// ゲッター
	inline class SceneRenderManager* GetSceneRenderManager() const { return m_SceneRenderManager; }
	inline class SceneCollisionManager* GetSceneCollisionManager() const { return m_SceneCollisionManager; }
private:

	/**
	* @brief シーンで読み込むデータ処理
	*/
	virtual void LoadData();

	/**
	* @brief シーン独自の破棄
	*/
	virtual void UnloadData();

	/**
	* @brief シーンで初期化前に情報を入力する処理
	*/
	virtual void SetData();

	/**
	* @brief シーン独自の更新処理
	*/
	virtual void UpdateScene();

	/**
	* @brief シーンで用いられる機能の生成
	*/
	virtual void CreateSceneFunction();

	/**
	* @brief シーンで用いられた機能の削除
	*/
	virtual void UnloadSceneFunction();
};