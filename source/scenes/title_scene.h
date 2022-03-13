/**
 * @file title_scene.h
 * @brief タイトルシーン
 */

#pragma once

#include"../framework/scene/scene.h"
#include"../directx/render_data/cbuffer_data.h"

class TitleScene : public Scene
{
private:
	//! シーンチェンジ予約フラグ
	bool m_IsSceneChange;

	//! フェード
	class Fade* m_Fade;

	//! パーティクルエミッター
	CBufferData::ParticleEmitter m_Emitter001;
	CBufferData::ParticleEmitter m_Emitter002;

	//! ビームサーベル
	class BeamSaber* beamSaber001;
	class BeamSaber* beamSaber002;

	//! アニメーションモデルコンポーネント
	class AnimationModelComponent* animeModelComp001;
	class AnimationModelComponent* animeModelComp002;

	//! フレームカウント
	int m_Frame;

	//! 直接光
	class DirectionalLightComponent* directionalLightComponent;

	CBufferData::DirectionalLight directionalLightData;

	//! カメラ
	class QuaternionCameraComponent* quaternionCameraComponent;
public:
	TitleScene();
	~TitleScene();

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
