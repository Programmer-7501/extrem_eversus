/**
* @file texture_manager.h
* @brief �e�N�X�`���Ǘ�
* @see https://gameworkslab.jp/2020/06/13/%e3%80%90directx11%e3%80%91%e3%83%86%e3%82%af%e3%82%b9%e3%83%81%e3%83%a3%e3%83%bc%e3%82%92%e4%bd%bf%e3%81%a3%e3%81%9f%e6%8f%8f%e7%94%bb/
*/

#pragma once

#include"../../function/singleton.h"

/**
* @brief �e�N�X�`���Ǘ��N���X
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
	* @brief �e�N�X�`���ǂݍ���
	* @param[in] const std::string&(fileName) �t�@�C���l�[��
	* @return ID3D11ShaderResourceView* �V�F�[�_�[���\�[�X�r���[
	* @details �t�@�C���l�[������e�N�X�`����ǂݍ��݃}�b�v�ɓo�^
	*/
	ID3D11ShaderResourceView* Load(const std::string& fileName);

	/**
	* @brief �e�N�X�`���j��
	* @param[in] const std::string&(fileName) �t�@�C���l�[��
	* @details �t�@�C���l�[������e�N�X�`�����}�b�v����j��
	*/
	void Unload(const std::string fileName);

	/**
	* @brief �e�N�X�`���j��
	* @details �t�@�C���l�[������e�N�X�`�����}�b�v����j��
	*/
	void AllRelease();

	/**
	* @brief �V�F�[�_�[���\�[�X�r���[�擾
	* @return ID3D11ShaderResourceView* �V�F�[�_�[���\�[�X�r���[
	*/
	ID3D11ShaderResourceView* GetSRV(const std::string fileName) const;
};