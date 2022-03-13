#pragma once

#include"mesh_component.h"

class FieldComponent : public MeshComponent
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;
	//! 頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;
	//! テクスチャ
	ID3D11ShaderResourceView* m_ShaderResourceView;
public:
	FieldComponent(class Actor* owner);
	~FieldComponent();

	void Draw() const override;

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }
private:
	/**
	* @brief データ読み込み
	* @return bool 成功したかどうか
	*/
	bool LoadData() override;

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