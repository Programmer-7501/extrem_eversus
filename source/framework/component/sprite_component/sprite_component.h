/**
 * @file sprite_component.h
 * @brief スプライトコンポーネント
 */

#pragma once

#include"../component.h"

 /**
 * @brief スプライトコンポーネント
 * @details 設定を途中で変更するつもりのないスプライトに使用
 */
class SpriteComponent : public Component
{
public:
	struct SpriteTextureConfig
	{
		Conv_XM::Vector2f TexCut;//テクスチャ切り取り座標
		Conv_XM::Vector2f TexCutSize;//テクスチャ切り取りサイズ
	};

	enum DrawNumber
	{
		DM_DEFAULT = 100,
	};
protected:
	//! 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! 描画順番
	int m_DrawOrder;

	//! 描画するかどうか
	bool m_Visible;

	//! テクスチャ情報
	SpriteTextureConfig m_SpriteTextureConfig;

	//! スプライトのポジション
	Conv_XM::Vector2f m_Position;
	//! スプライトのサイズ
	Conv_XM::Vector2f m_Size;

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
	SpriteComponent(class Actor* owner);
	virtual ~SpriteComponent()override;

	/**
	* @brief 初期化処理
	*/
	virtual void Init()override;

	/**
	* @brief 終了処理
	*/
	virtual void Uninit()override;

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const;

	// セッター、ゲッター

	inline void SetVisible(bool flag) { m_Visible = flag; }

	inline void SetPosition(const Conv_XM::Vector2f& position) { m_Position = position; }
	inline void SetSize(const Conv_XM::Vector2f& size) { m_Size = size; }

	inline int GetDrawOrder() const { return m_DrawOrder; }

	// ここから先はActorInitまでに設定しないと無効

	inline void SetDrawOrder(int order) { m_DrawOrder = order; }

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }

	inline void SetSpriteTextureConfig(const SpriteTextureConfig& spriteTextureConfig) { m_SpriteTextureConfig = spriteTextureConfig; }

protected:
	/**
	* @brief デバイスコンテキストの取得
	* @return bool 成功したかどうか
	*/
	bool GetDeviceContext();

private:


	/**
	* @brief シェーダーの取得
	* @return bool 成功したかどうか
	*/
	virtual bool GetShader();
};