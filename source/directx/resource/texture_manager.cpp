
#include"../../system/main.h"
#include"texture_manager.h"
#include"../renderer.h"
#include"../../function/useful_string.h"



ID3D11ShaderResourceView* TextureManager::Load(const std::string & fileName)
{
	ID3D11ShaderResourceView* nullResult = nullptr;

	// 読み込み済みか確認するため連想配列から探す
	auto iter = m_TextureMap.find(fileName);

	if (iter == m_TextureMap.end())
	{
		// デバイスの取得
		ID3D11Device* device = Renderer::GetInstance().GetDevice();
		if (device == nullptr)
		{
			Logger::GetInstance().SetLog("TextureManager::Load deviceがnullptr");
			return nullResult;
		}

		// ファイル拡張子
		std::string expand;
		// ファイル拡張子取得
		ExtractFileExpand(fileName, expand);

		// WCHARに変換
		WCHAR path[256];
		size_t len = 0;
		mbstowcs_s(&len, path, 256, fileName.c_str(), _TRUNCATE);

		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		HRESULT hr;
		// DDSファイルの読み込み
		if (expand == "dds")
		{
			hr = DirectX::LoadFromDDSFile(path, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metadata, image);
		}
		else if (expand == "tga")// TGAファイルの読み込み
		{
			hr = DirectX::LoadFromTGAFile(path, &metadata, image);
		}
		else// WICファイル(bmp, jpg, png等)の読み込み
		{
			hr = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, image);
		}

		// 読み込みが出来たか判定
		if (FAILED(hr))
		{
			Logger::GetInstance().SetLog("TextureManager::Load ファイルロード失敗");
			return nullResult;
		}

		// データ構造体
		TextureData data = {};

		// 画像からシェーダリソースViewの作成
		hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, &data.pShaderResourceView);

		if (FAILED(hr)) 
		{
			Logger::GetInstance().SetLog("TextureManager::Load シェーダーリソースビュー作成失敗");
			return nullResult;
		}

		data.Format = metadata.format;
		data.Width = static_cast<UINT>(metadata.width);
		data.Height = static_cast<UINT>(metadata.height);
		data.LoadCount = 1;

		// Mapに登録
		m_TextureMap.emplace(fileName, data);

		return data.pShaderResourceView;
	}
	else
	{
		//ロードカウントを増やす
		iter->second.LoadCount++;
		return iter->second.pShaderResourceView;
	}
}

void TextureManager::Unload(const std::string fileName)
{
	// Mapに登録されているか探す
	auto&& iter = m_TextureMap.find(fileName);

	// 登録されていたら
	if (iter != m_TextureMap.end())
	{
		//複数ロードしてたらカウント減らしてリターン
		if (iter->second.LoadCount > 1) 
		{
			iter->second.LoadCount--;
			return;
		}

		// シェーダーリソースビュー解放
		if (iter->second.pShaderResourceView)
		{
			iter->second.pShaderResourceView->Release();
		}
		else
		{
			Logger::GetInstance().SetLog("TextureManager::Unload シェーダーリソースビューがnullptr");
		}
		// マップから削除
		m_TextureMap.erase(iter);
	}
	else
	{
		Logger::GetInstance().SetLog("TextureManager::Unload 指定されたファイルがMapに登録されていない");
	}
}

void TextureManager::AllRelease()
{
	for (auto&& iter : m_TextureMap)
	{
		// シェーダーリソースビュー解放
		if (iter.second.pShaderResourceView)
		{
			iter.second.pShaderResourceView->Release();
		}
		else
		{
			Logger::GetInstance().SetLog("TextureManager::AllRelease シェーダーリソースビューがnullptr");
		}
	}
	m_TextureMap.clear();
}

ID3D11ShaderResourceView * TextureManager::GetSRV(const std::string fileName) const
{
	// Mapに登録されているか探す
	auto&& iter = m_TextureMap.find(fileName);

	// 登録されていたら
	if (iter != m_TextureMap.end())
	{
		// シェーダーリソースビューを渡す
		return iter->second.pShaderResourceView;
	}

	Logger::GetInstance().SetLog("TextureManager::GetSRV 指定されたファイルがMapに登録されていない");
	return false;
}
