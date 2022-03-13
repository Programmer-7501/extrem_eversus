/**
* @file deferred_rendering.h
* @brief ディファードレンダリング
*/

#pragma once

/**
* @brief ディファードレンダリング実行クラス
*/
class DeferredRendering
{
private:
	//! アルベドカラーシェーダーリソースビュー
	ID3D11ShaderResourceView* m_AlbedColorSRV;
	//! ノーマルシェーダーリソースビュー
	ID3D11ShaderResourceView* m_NormalSRV;
	//! スぺキュラシェーダーリソースビュー
	ID3D11ShaderResourceView* m_SpecularSRV;
	//! デプスステンシルシェーダーリソースビュー
	ID3D11ShaderResourceView* m_DepthStancilSRV;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! 頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! 初期化カラー
	float m_Color[4];
public:
	DeferredRendering(
		ID3D11ShaderResourceView* albedColorSRV,
		ID3D11ShaderResourceView* normalSRV,
		ID3D11ShaderResourceView* specularSRV,
		ID3D11ShaderResourceView* depthstencilSRV,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);
	~DeferredRendering();


	/**
	* @brief 初期化処理
	* @return bool 成功したかどうか
	*/
	bool Init();


	/**
	* @brief ディファードレンダリング実行
	*/
	void Draw() const;

private:

	/**
	* @brief シェーダーの取得
	* @return bool 成功したかどうか
	*/
	bool GetShader();

	/**
	* @brief ポリゴン情報作成
	* @return bool 成功したかどうか
	*/
	bool CreatePolygon();
};