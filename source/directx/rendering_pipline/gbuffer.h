/**
* @file gbuffer_input.h
* @brief GBuffer入力
* @details rendererで使用される
*/

#pragma once

/**
* @brief GBuffer入力クラス
*/
class GBuffer
{
private:
	//! アルベドカラーレンダーターゲットビュー
	ID3D11RenderTargetView* m_AlbedColorRTV;

	//! ノーマルレンダーターゲットビュー
	ID3D11RenderTargetView* m_NormalRTV;

	//! スぺキュラレンダーターゲットビュー
	ID3D11RenderTargetView* m_SpecularRTV;

	//! スぺキュラレンダーターゲットビュー
	ID3D11RenderTargetView* m_depthRTV;

	//! メインデプスステンシル
	ID3D11DepthStencilView* m_MainDSV;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! 初期化カラー
	float m_Color[4];
public:
	GBuffer(
		ID3D11RenderTargetView* albedColorRTV,
		ID3D11RenderTargetView* normalRTV,
		ID3D11RenderTargetView* specularRTV,
		ID3D11RenderTargetView * depthRTV,
		ID3D11DepthStencilView* mainDS,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);

	~GBuffer();

	/**
	* @brief 初期化処理
	* @return bool 成功したかどうか
	*/
	bool Init();

	/**
	* @brief GBuffer入力開始
	*/
	void InputBegin();

	/**
	* @brief GBuffer入力終了
	*/
	void InputEnd();
};