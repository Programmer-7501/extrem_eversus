/**
* @file model.h
* @brief 自作モデルファイル
*/

#pragma once

#include"../render_data/vertex_structure.h"
#include"../render_data/cbuffer_data.h"

/**
* @brief 自作モデルを扱うクラス
*/
class Model final
{
private:
	/**
	* @brief メッシュデータ構造体
	*/
	struct MeshData
	{
		std::vector<VERTEX_NORMAL_TEXCOOD> Vertices;
		std::vector<UINT> Indices;
		CBufferData::Material Material;
		std::string TextureName;

		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		ID3D11ShaderResourceView* TextureData;
	};

protected:
	//! ファイル名
	std::string m_FileName;
	//! メッシュリスト
	std::vector<MeshData> m_MeshList;
	//! デバイス
	ID3D11Device* m_Device;
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;
public:
	Model();
	~Model();

	/**
	* @brief 自作モデルの読み込み
	* @param[in] const std::string&(fileName) ファイル名
	* @return bool 読み込みが成功したかどうか
	*/
	bool Load(const std::string& fileName);

	/**
	* @brief 自作モデルの破棄
	*/
	void Unload();

	/**
	* @brief 自作モデルの描画
	*/
	void Draw() const;


	/**
	* @brief 自作モデルの描画
	* @details インスタンシング用
	* @details Matrixのみのインスタンシング用
	*/
	void InstancingDraw(ID3D11Buffer* instancingBuffer, int instancingNum) const;


	/**
	* @brief 自作モデルの描画
	* @details 色付きインスタンシング用
	* @details 色付きのインスタンシング用
	*/
	void InstancingColorDraw(ID3D11Buffer* instancingBuffer, int instancingNum, UINT* stride, UINT* offset) const;

protected:
	/**
	* @brief 自作モデルの更新
	* @return bool 頂点バッファの作成が成功したかどうか
	*/
	bool CreateVertexBuffer();

	/**
	* @brief 自作モデルの更新
	* @return bool インデックスバッファの作成が成功したかどうか
	*/
	bool CreateIndexBuffer();

	/**
	* @brief 自作モデルの更新
	* @return bool テクスチャの読み込みが成功したかどうか
	*/
	bool LoadTexture();
};