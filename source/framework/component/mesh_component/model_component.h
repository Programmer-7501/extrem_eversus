/**
* @file model_component.h
* @brief 自作モデルファイル
*/

#pragma once

#include"mesh_component.h"

/**
* @brief 自作モデルを扱うコンポーネント
*/
class ModelComponent : public MeshComponent
{
private:
	//! 自作モデルクラス
	class Model* m_Model;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! GBuffer入力用頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! オフセットサイズ
	Conv_XM::Vector3f m_OffsetScale;

	//! オフセットローテーション
	Conv_XM::Vector4f m_OffsetQuaternion;

public:
	ModelComponent(class Actor* owner);
	~ModelComponent() {}

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const override;

	/**
	* @brief 影描画処理
	*/
	virtual void ShadowDraw() const override;

	// セッター
	inline void SetModel(class Model* model) { m_Model = model; }

	inline void SetOffsetScale(const Conv_XM::Vector3f scale) { m_OffsetScale = scale; }

	inline void SetOffsetQuaternion(const Conv_XM::Vector4f quaternion) { m_OffsetQuaternion = quaternion; }

private:
	/**
	* @brief データ読み込み
	* @return bool 成功したかどうか
	*/
	virtual bool LoadData();

	/**
	* @brief デバイスコンテキストの取得
	* @return bool 成功したかどうか
	*/
	bool GetDeviceContext();

	/**
	* @brief シェーダーの取得
	* @return bool 成功したかどうか
	*/
	bool GetShader();
};