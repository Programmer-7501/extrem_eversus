/**
 * @file scene_manager.h
 * @brief シーン管理機能
 */

#pragma once

#include"../../function/singleton.h"
#include"scene.h"

 /**
 * @brief シーン管理クラス
 */
class SceneManager : public Singleton<SceneManager>
{
public:
	enum SceneName
	{
		//! タイトルシーン
		SN_TITLE_SCENE,
		//! モード選択シーン
		SN_MODESELECT_SCENE,
		//! ゲームシーン
		SN_GAME_SCENE,
		//! デモシーン
		SN_DEMO_SCENE,
		//! ゲームオーバーシーン
		SN_GAME_OVER_SCENE,
		//! ゲームクリアシーン
		SN_GAME_CLEAR_SCENE,
		//! AIテストシーン
		SN_AI_TEST_SCENE,
		//! ノード作成シーン
		SN_MAKE_NODE_SCENE,
	};
private:
	//! 現在使用しているシーン
	class Scene* m_CurrentScene = nullptr;

	//! フェードクラス
	class Fade* m_Fade = nullptr;

	//! 現在のシーン名
	SceneName m_CurrentSceneName;
	//! 切り替え先のシーン名
	SceneName m_NextSceneName;
private:
	friend class Singleton<SceneManager>;

	SceneManager() = default;
	~SceneManager() = default;
public:
	/**
	* @brief 初期化処理
	* @return bool 初期化が成功したか否か
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
	void Draw();

	/**
	* @brief シーン切り替え
	*/
	void SceneExChange();


	/**
	* @brief フェードイン
	* @param[in] int(fadeTime) フェード時間
	*/
	void FadeInStart(int fadeTime = 60);

	/**
	* @brief フェードアウト
	* @param[in] int(fadeTime) フェード時間
	*/
	void FadeOutStart(int fadeTime = 60);

	// セッター、ゲッター
	inline class Scene* GetCurrentScene() const { return m_CurrentScene; }
	inline class Fade* GetFade() const { return m_Fade; }

	inline void SceneChange(SceneName name) { m_NextSceneName = name; }

};
