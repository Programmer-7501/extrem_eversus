/**
 * @file demo_scene.h
 * @brief テストシーン
 */

#pragma once

#include"../framework/scene/scene.h"
#include"../directx/render_data/cbuffer_data.h"

class DemoScene : public Scene
{
private:
	//! マテリアル変更用モデル変数
	CBufferData::Material m_Material;
	class ModelComponent* m_SphereModel;
	float m_Metaric;
	float m_Smoothness;

	//! 直接光変更用変数
	class DirectionalLightComponent* m_DirectionalLight;
	CBufferData::DirectionalLight m_DirectionalLightData;
	float m_Ambient[3];
	float m_Color[3];
	float m_Direction[3];

	//! パーティクルデータ
	int m_MaxParticleNum;

	//! パーティクルエミッター
	bool m_ParticleUse;
	CBufferData::ParticleEmitter m_Emitter001;
	CBufferData::ParticleEmitter m_Emitter002;
	CBufferData::ParticleEmitter m_Emitter003;

	//! フェード用
	class Fade* m_Fade;

	//! シーンチェンジ予約フラグ
	bool m_IsSceneChange;
public:
	DemoScene();
	~DemoScene();

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
