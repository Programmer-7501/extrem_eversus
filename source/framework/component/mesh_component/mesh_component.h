/**
* @file mesh_component.h
* @brief 自作モデルファイル
*/

#pragma once

#include"../component.h"
#include"../../../directx/render_data/cbuffer_data.h"

/**
* @brief 3Dモデルを扱うコンポーネント
*/
class MeshComponent : public Component
{
public:
	//! レンダリング方法
	enum RenderingWay
	{
		//! ディファードレンダリング(デフォルト)
		RENDERING_DEFERRED,
		//! フォワードレンダリング
		RENDERING_FORWARD,
	};
protected:
	//! 見えるかどうか
	bool m_Visible;

	//! レンダリング方法
	RenderingWay m_RenderingWay;

	//! 影入力するかどうか
	bool m_IsShadowInput;

	//! マテリアル
	CBufferData::Material m_Material;
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();


	/**
	* @brief 初期化処理
	* @details これ以降の初期化処理はLoadDataにさせるためオーバーライド不可にする
	*/
	void Init() final override;

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const = 0;

	/**
	* @brief 影描画処理
	*/
	virtual void ShadowDraw() const;

	// セッター、ゲッター
	inline void SetVisible(bool flag) { m_Visible = flag; }
	inline bool GetVisible() const { return m_Visible; }

	inline void SetRenderingWay(RenderingWay renderingWay) { m_RenderingWay = renderingWay; }
	inline RenderingWay GetRenderingWay() const { return m_RenderingWay; }

	inline void SetIsShadowInput(bool flag) { m_IsShadowInput = flag; }
	inline bool GetIsShadowInput() const { return m_IsShadowInput; }

	inline void SetMaterial(const CBufferData::Material& material) { m_Material = material; }
private:
	/**
	* @brief シーンレンダーマネージャーの登録
	*/
	bool RegisterSceneRenderManager();

	/**
	* @brief データ読み込み
	*/
	virtual bool LoadData();
};