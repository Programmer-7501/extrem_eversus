/**
 * @file scene_render_manager.h
 * @brief シーンの描画管理
 */

#pragma once

 /**
 * @brief シーンの描画管理クラス
 */
class SceneRenderManager
{
private:
	//! スプライトコンポーネント
	std::vector<class SpriteComponent*> m_SpriteComponents;

	//! 影入力するメッシュコンポーネント
	std::vector<class MeshComponent*> m_ShadowInputMeshComponents;

	//! ディファードレンダリングするメッシュコンポーネント
	std::vector<class MeshComponent*> m_DeferredRenderingMeshComponents;

	//! フォワードレンダリングするメッシュコンポーネント
	std::vector<class MeshComponent*> m_ForwardRenderingMeshComponents;

	//! カメラコンポーネント
	class CameraComponent* m_MainCameraComponent;

	//! ライトカメラコンポーネント
	class CameraComponent* m_LightCameraComponent;

	//! 時間計測クラス
	class TimeMeasurement* m_TimeMeasure;
public:
	SceneRenderManager();
	~SceneRenderManager();

	/**
	* @brief 初期化処理
	*/
	void Init();

	/**
	* @brief 終了処理
	*/
	void Uninit();

	/**
	* @brief 描画処理
	*/
	void Draw() const;

	/**
	* @brief スプライトコンポーネント追加関数
	* @param[in] class SpriteComponent*(spriteComponent) スプライトコンポーネント
	*/
	void AddSpriteComponent(class SpriteComponent* spriteComponent);

	/**
	* @brief メッシュコンポーネント追加関数
	* @param[in] class MeshComponent*(meshComponent) メッシュコンポーネント
	*/
	void AddMeshComponent(class MeshComponent* meshComponent);

	/**
	* @brief メインカメラコンポーネントの登録
	* @param[in] class CameraComponent*(cameraComponent) カメラコンポーネント
	*/
	inline void SetMainCameraComponent(class CameraComponent* mainCameraComponent) { m_MainCameraComponent = mainCameraComponent; }

	/**
	* @brief ライトカメラコンポーネントの登録
	* @param[in] class CameraComponent*(cameraComponent) カメラコンポーネント
	*/
	inline void SetLightCameraComponent(class CameraComponent* lightCameraComponent) { m_LightCameraComponent = lightCameraComponent; }

	/**
	* @brief カメラコンポーネントの取得
	* @return class CameraComponent* カメラコンポーネントの取得
	*/
	inline class CameraComponent* GetMainCameraComponent() const { return m_MainCameraComponent; }

private:

	/**
	* @brief 3D描画
	*/
	void Draw3D() const;

	/**
	* @brief 2D描画
	*/
	void Draw2D() const;

	/**
	* @brief シャドウマップ入力
	*/
	void InputShadow() const;

	/**
	* @brief GBuffer入力
	*/
	void InputGBuffer() const;

	/**
	* @brief フォワードレンダリング実行
	*/
	void FowardRenderingExcute() const;
};