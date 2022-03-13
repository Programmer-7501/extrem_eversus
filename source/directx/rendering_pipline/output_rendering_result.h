/**
* @file output_rendering_result.h
* @brief レンダリング結果表示
*/

#pragma once

/**
* @brief レンダリング結果表示クラス
*/
class OutPutRenderingResult
{
private:
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! レンダリング結果シェーダーリソースビュー
	ID3D11ShaderResourceView* m_RenderingResultSRV;

	//! 初期化カラー
	float m_Color[4];


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
public:
	OutPutRenderingResult(
		ID3D11ShaderResourceView* renderingResultSRV,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);
	~OutPutRenderingResult();


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