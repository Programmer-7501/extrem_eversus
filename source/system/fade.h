#pragma once

class Fade
{
public:
	enum FadeMode
	{
		FADE_NONE,
		FADE_IN,
		FADE_OUT,
	};

private:
	//! アクティブフラグ
	bool m_Active;
	//! 現在のフレーム
	int m_CurrentFrame;
	//! フェード時間
	int m_FadeTime;
	//! アルファ値
	float m_Arufa;
	FadeMode m_FadeState;

	//! 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! インデックスバッファ
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

	//! 色
	Conv_XM::Vector3f m_Color;
public:
	Fade();
	~Fade();

	/**
	* @brief 初期化処理
	*/
	void Init();

	/**
	* @brief 更新処理
	*/
	void Update();

	/**
	* @brief 描画処理
	*/
	void Draw();

	bool GetActive() { return m_Active; }
	void SetFade(FadeMode state, int fadetime);

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }
};