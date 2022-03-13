/**
* @file obb_debug_view_component.h
* @brief OBBデバッグビュー
*/

#pragma once

#include"../mesh_component.h"

/**
* @brief OBBデバッグビューを扱うコンポーネント
*/
class OBBDebugViewComponent : public MeshComponent
{
private:
	//! 表示するかどうかのフラグ
	static bool m_IsUseFlag;

public:
	static void SetIsUseFlag(bool flag);
private:
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! 頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! インデックス数
	UINT m_IndexNum;

	//! ポジション
	Conv_XM::Vector3f m_Position;
	//! スケール
	Conv_XM::Vector3f m_Scale;
	//! ローテーション
	Conv_XM::Vector4f m_Quaternion;

public:
	OBBDebugViewComponent(class Actor* owner);
	~OBBDebugViewComponent() {}

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const;

	inline void SetTransform(const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& scale, const Conv_XM::Vector4f& quaternion)
	{
		m_Position = position;
		m_Scale = scale;
		m_Quaternion = quaternion;
	}

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
