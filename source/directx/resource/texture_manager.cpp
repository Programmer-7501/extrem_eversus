
#include"../../system/main.h"
#include"texture_manager.h"
#include"../renderer.h"
#include"../../function/useful_string.h"



ID3D11ShaderResourceView* TextureManager::Load(const std::string & fileName)
{
	ID3D11ShaderResourceView* nullResult = nullptr;

	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_TextureMap.find(fileName);

	if (iter == m_TextureMap.end())
	{
		// �f�o�C�X�̎擾
		ID3D11Device* device = Renderer::GetInstance().GetDevice();
		if (device == nullptr)
		{
			Logger::GetInstance().SetLog("TextureManager::Load device��nullptr");
			return nullResult;
		}

		// �t�@�C���g���q
		std::string expand;
		// �t�@�C���g���q�擾
		ExtractFileExpand(fileName, expand);

		// WCHAR�ɕϊ�
		WCHAR path[256];
		size_t len = 0;
		mbstowcs_s(&len, path, 256, fileName.c_str(), _TRUNCATE);

		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		HRESULT hr;
		// DDS�t�@�C���̓ǂݍ���
		if (expand == "dds")
		{
			hr = DirectX::LoadFromDDSFile(path, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metadata, image);
		}
		else if (expand == "tga")// TGA�t�@�C���̓ǂݍ���
		{
			hr = DirectX::LoadFromTGAFile(path, &metadata, image);
		}
		else// WIC�t�@�C��(bmp, jpg, png��)�̓ǂݍ���
		{
			hr = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, image);
		}

		// �ǂݍ��݂��o����������
		if (FAILED(hr))
		{
			Logger::GetInstance().SetLog("TextureManager::Load �t�@�C�����[�h���s");
			return nullResult;
		}

		// �f�[�^�\����
		TextureData data = {};

		// �摜����V�F�[�_���\�[�XView�̍쐬
		hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, &data.pShaderResourceView);

		if (FAILED(hr)) 
		{
			Logger::GetInstance().SetLog("TextureManager::Load �V�F�[�_�[���\�[�X�r���[�쐬���s");
			return nullResult;
		}

		data.Format = metadata.format;
		data.Width = static_cast<UINT>(metadata.width);
		data.Height = static_cast<UINT>(metadata.height);
		data.LoadCount = 1;

		// Map�ɓo�^
		m_TextureMap.emplace(fileName, data);

		return data.pShaderResourceView;
	}
	else
	{
		//���[�h�J�E���g�𑝂₷
		iter->second.LoadCount++;
		return iter->second.pShaderResourceView;
	}
}

void TextureManager::Unload(const std::string fileName)
{
	// Map�ɓo�^����Ă��邩�T��
	auto&& iter = m_TextureMap.find(fileName);

	// �o�^����Ă�����
	if (iter != m_TextureMap.end())
	{
		//�������[�h���Ă���J�E���g���炵�ă��^�[��
		if (iter->second.LoadCount > 1) 
		{
			iter->second.LoadCount--;
			return;
		}

		// �V�F�[�_�[���\�[�X�r���[���
		if (iter->second.pShaderResourceView)
		{
			iter->second.pShaderResourceView->Release();
		}
		else
		{
			Logger::GetInstance().SetLog("TextureManager::Unload �V�F�[�_�[���\�[�X�r���[��nullptr");
		}
		// �}�b�v����폜
		m_TextureMap.erase(iter);
	}
	else
	{
		Logger::GetInstance().SetLog("TextureManager::Unload �w�肳�ꂽ�t�@�C����Map�ɓo�^����Ă��Ȃ�");
	}
}

void TextureManager::AllRelease()
{
	for (auto&& iter : m_TextureMap)
	{
		// �V�F�[�_�[���\�[�X�r���[���
		if (iter.second.pShaderResourceView)
		{
			iter.second.pShaderResourceView->Release();
		}
		else
		{
			Logger::GetInstance().SetLog("TextureManager::AllRelease �V�F�[�_�[���\�[�X�r���[��nullptr");
		}
	}
	m_TextureMap.clear();
}

ID3D11ShaderResourceView * TextureManager::GetSRV(const std::string fileName) const
{
	// Map�ɓo�^����Ă��邩�T��
	auto&& iter = m_TextureMap.find(fileName);

	// �o�^����Ă�����
	if (iter != m_TextureMap.end())
	{
		// �V�F�[�_�[���\�[�X�r���[��n��
		return iter->second.pShaderResourceView;
	}

	Logger::GetInstance().SetLog("TextureManager::GetSRV �w�肳�ꂽ�t�@�C����Map�ɓo�^����Ă��Ȃ�");
	return false;
}
