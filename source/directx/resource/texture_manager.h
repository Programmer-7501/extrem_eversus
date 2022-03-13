/**
* @file texture_manager.h
* @brief テクスチャ管理
* @see https://gameworkslab.jp/2020/06/13/%e3%80%90directx11%e3%80%91%e3%83%86%e3%82%af%e3%82%b9%e3%83%81%e3%83%a3%e3%83%bc%e3%82%92%e4%bd%bf%e3%81%a3%e3%81%9f%e6%8f%8f%e7%94%bb/
*/

#pragma once

#include"../../function/singleton.h"

/**
* @brief テクスチャ管理クラス
*/
class TextureManager final : public Singleton<TextureManager>
{
public:
	struct TextureData
	{
		ID3D11ShaderResourceView* pShaderResourceView;
		DXGI_FORMAT Format;
		UINT Width;
		UINT Height;
		UINT LoadCount;
	};
public:
	std::unordered_map<std::string, TextureData> m_TextureMap;
private:
	friend class Singleton<TextureManager>;

	TextureManager() = default;
	~TextureManager() = default;

public:

	/**
	* @brief テクスチャ読み込み
	* @param[in] const std::string&(fileName) ファイルネーム
	* @return ID3D11ShaderResourceView* シェーダーリソースビュー
	* @details ファイルネームからテクスチャを読み込みマップに登録
	*/
	ID3D11ShaderResourceView* Load(const std::string& fileName);

	/**
	* @brief テクスチャ破棄
	* @param[in] const std::string&(fileName) ファイルネーム
	* @details ファイルネームからテクスチャをマップから破棄
	*/
	void Unload(const std::string fileName);

	/**
	* @brief テクスチャ破棄
	* @details ファイルネームからテクスチャをマップから破棄
	*/
	void AllRelease();

	/**
	* @brief シェーダーリソースビュー取得
	* @return ID3D11ShaderResourceView* シェーダーリソースビュー
	*/
	ID3D11ShaderResourceView* GetSRV(const std::string fileName) const;
};