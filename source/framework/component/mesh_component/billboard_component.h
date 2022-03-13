/**
 * @file billboard_component.h
 * @brief ビルボードコンポーネント
 */

#pragma once

#include"mesh_component.h"

class BillBoardComponent : public MeshComponent
{
public:
	struct BillBoardTextureConfig
	{
		Conv_XM::Vector2f TexCut;//テクスチャ切り取り座標
		Conv_XM::Vector2f TexCutSize;//テクスチャ切り取りサイズ
	};

protected:
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

	//! シーンレンダーマネージャー
	class SceneRenderManager* m_SceneRenderManager;

	//! ポジション
	Conv_XM::Vector3f m_Position;
	//! サイズ
	Conv_XM::Vector2f m_Size;

	//! テクスチャコンフィグ
	BillBoardTextureConfig m_BillBoardTextureConfig;

	//! 軸のロック
	bool m_ShaftRock[3];

public:
	BillBoardComponent(class Actor* owner);
	virtual ~BillBoardComponent();

	void Draw() const override;

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }

	inline void SetPosition(const Conv_XM::Vector3f& position) { m_Position = position; }
	inline void SetSize(const Conv_XM::Vector2f& size) { m_Size = size; }

	inline void SetBillBoardTextureConfig(const BillBoardTextureConfig& biiBoardTextureConfig) { m_BillBoardTextureConfig = biiBoardTextureConfig; }

	// 0:X 1:Y 2:Z
	inline void SetShaftRock(int index) 
	{
		if (index < 0 || index > 2)
		{
			return;
		}
		m_ShaftRock[index] = true; 
	}
protected:
	/**
	* @brief データ読み込み
	* @return bool 成功したかどうか
	*/
	virtual bool LoadData() override;

	/**
	* @brief メッシュ作成
	* @return bool 成功したかどうか
	*/
	virtual bool CreateMesh();

	/**
	* @brief デバイスコンテキストの取得
	* @return bool 成功したかどうか
	*/
	bool GetDeviceContext();

	/**
	* @brief シェーダーの取得
	* @return bool 成功したかどうか
	*/
	virtual bool GetShader();
};