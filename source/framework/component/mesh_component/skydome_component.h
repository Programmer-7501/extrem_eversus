/**
* @file skydome_component.h
* @brief スカイドームファイル
*/

#pragma once

#include"mesh_component.h"

/**
* @brief 自作モデルのスカイドームを扱うコンポーネント
*/
class SkyDomeomponent : public MeshComponent
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

public:
	SkyDomeomponent(class Actor* owner);
	~SkyDomeomponent() {}

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const override;

	// セッター
	inline void SetModel(class Model* model) { m_Model = model; }
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