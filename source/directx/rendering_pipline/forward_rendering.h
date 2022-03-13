/**
* @file forward_rendering.h
* @brief フォワードレンダリング
*/

#pragma once

/**
* @brief フォワードレンダリング実行クラス
*/
class ForwardRendering
{
private:
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! メインレンダーターゲットビュー
	ID3D11RenderTargetView* m_MainRTV;

	//! メインデプスステンシル
	ID3D11DepthStencilView* m_MainDSV;

	//! 初期化カラー
	float m_Color[4];
public:
	ForwardRendering(
		ID3D11RenderTargetView* mainRTV,
		ID3D11DepthStencilView* mainDSV,
		float clearColor_R,
		float clearColor_B,
		float clearColor_G,
		float clearColor_A
	);
	~ForwardRendering();

	/**
	* @brief 初期化処理
	* @return bool 成功したかどうか
	*/
	bool Init();

	/**
	* @brief フォワードレンダリング開始
	*/
	void Begin() const;

	/**
	* @brief フォワードレンダリング終了
	*/
	void End() const;
};