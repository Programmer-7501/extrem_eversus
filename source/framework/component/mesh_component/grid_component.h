/**
* @file grid_component.h
* @brief グリッド
*/

#pragma once

#include"mesh_component.h"

/**
* @brief グリッドを扱うコンポーネント
*/
class GridComponent : public MeshComponent
{
private:
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! 頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! 頂点数
	int m_VertexCount;

	//! 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
public:
	GridComponent(class Actor* owner);
	~GridComponent() {}

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const;

private:
	/**
	* @brief データ読み込み
	* @return bool 成功したかどうか
	*/
	virtual bool LoadData();

	/**
	* @brief メッシュ作成
	* @return bool 成功したかどうか
	*/
	bool CreateMesh();

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
