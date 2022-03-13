/**
 * @file mode_select_scene.h
 * @brief モード選択シーン
 */

#pragma once

#include"../framework/scene/scene.h"


 /**
 * @namespace ModeSelectSceneData
 * @brief モード選択シーン情報
 */
namespace ModeSelectSceneData
{
	//! オーディオデータ
	//! BGM
	static const char* BGM = "asset/sound/BGM/ModeSelectBGM.wav";
}

class ModeSelectScene : public Scene
{
private:
	//! シーンチェンジ予約フラグ
	bool m_IsSceneChange;

	//! フェード
	class Fade* m_Fade;

	//! デバックコリジョンビューを表示するかどうか
	bool m_IsUseDebugCollisionViewFlag;

	//! セレクト
	class ModeSelectSceneUI* m_ModeSelectSceneUI;
public:
	ModeSelectScene();
	~ModeSelectScene();

private:
	/**
	* @brief シーンで読み込むデータ処理
	*/
	void LoadData() override;

	/**
	* @brief シーン独自の破棄
	*/
	void UnloadData() override;

	/**
	* @brief シーンで初期化前に情報を入力する処理
	*/
	void SetData()override;

	/**
	* @brief シーン独自の更新処理
	*/
	void UpdateScene() override;

	/**
	* @brief シーンで用いられる機能の生成
	*/
	void CreateSceneFunction() override;

	/**
	* @brief シーンで用いられた機能の削除
	*/
	void UnloadSceneFunction() override;
};
