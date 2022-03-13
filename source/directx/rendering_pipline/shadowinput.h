/**
* @file shadowinput.h
* @brief 影入力
*/

#pragma once

/**
* @brief 影入力クラス
*/
class ShadowInput
{
private:
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! シャドウインプットデプスステンシル
	ID3D11DepthStencilView* m_ShadowInputDSV;

public:
	ShadowInput(ID3D11DepthStencilView* shadowInputDSV);
	~ShadowInput();

	/**
	* @brief 初期化処理
	* @return bool 成功したかどうか
	*/
	bool Init();

	/**
	* @brief 影入力開始
	*/
	void Begin();

	/**
	* @brief 影入力終了
	*/
	void End();
};
