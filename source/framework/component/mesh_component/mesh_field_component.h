#pragma once

#include"mesh_component.h"
#include"../../../directx/render_data/vertex_structure.h"

class MeshFieldComponent : public MeshComponent
{
public:
	//! 芝テクスチャ
	static ID3D11ShaderResourceView* m_Grass_ShaderResourceView;
	//! 岩テクスチャ
	static ID3D11ShaderResourceView* m_Rock_ShaderResourceView;
	//! ボーダーテクスチャ
	static ID3D11ShaderResourceView* m_Border_ShaderResourceView;

	/**
	* @brief テクスチャ読み込み
	*/
	static void LoadTexture();

	/**
	* @brief テクスチャ破棄
	*/
	static void UnloadTexture();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND* m_Vertex;
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;
	//! 頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! Xの頂点数
	int m_X_Num;
	//! Zの頂点数
	int m_Z_Num;
	//! Xの頂点間隔
	float m_X_Length;
	//! Zの頂点間隔
	float m_Z_Length;

	//! 高さマップ
	std::vector<std::vector<float>> m_HeightMap;
	//! テクスチャブレンド
	std::vector<std::vector<float>> m_TextureBlend1;
	//! テクスチャブレンド
	std::vector<std::vector<float>> m_TextureBlend2;
	//! テクスチャブレンド
	std::vector<std::vector<float>> m_TextureBlend3;
	//! テクスチャブレンド
	std::vector<std::vector<float>> m_TextureBlend4;

	//! タイトルかどうか
	bool m_IsTitle;
public:
	MeshFieldComponent(class Actor* owner);
	~MeshFieldComponent();

	void Draw() const override;

	float GetHeight(const Conv_XM::Vector3f& position);

	/**
	* @brief 高さファイル読み込み
	* @param[in] const std::string&(fileName) ファイル名
	*/
	void LoadHeightMapFile(const std::string& fileName);

	/**
	* @brief テクスチャブレンドファイル読み込み
	* @param[in] const std::string&(fileName) ファイル名
	* @param[in] int(blendNo) ブレンドテクスチャナンバー
	*/
	void LoadTextureBlendFile(const std::string& fileName, int blendNo);

	/**
	* @brief メッシュフィールドのパラメーター設定
	* @param[in] int(x_num) Xの頂点数
	* @param[in] int(z_num) Zの頂点数
	* @param[in] float(x_length) Xの頂点間隔
	* @param[in] float(z_length) Zの頂点間隔
	*/
	inline void SetParameter(int x_num, int z_num, float x_length, float z_length)
	{
		m_X_Num = x_num;
		m_Z_Num = z_num;
		m_X_Length = x_length;
		m_Z_Length = z_length;
	}

	inline void SetIsTitle(bool flag) { m_IsTitle = flag; }
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