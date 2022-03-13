
#include"../../system/main.h"
#include"model.h"
#include"../renderer.h"
#include"../resource/texture_manager.h"
#include"../render_data/srv_data.h"

Model::Model()
	: m_Device(nullptr)
	, m_DeviceContext(nullptr)
{
	// デバイスの取得
	m_Device = Renderer::GetInstance().GetDevice();
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load m_Deviceがnullptr");
	}

	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load m_DeviceContextがnullptr");
	}
}

Model::~Model()
{
}

bool Model::Load(const std::string & fileName)
{
	// デバイスがnull
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load deviceがnullptr");
		return false;
	}

	m_FileName = fileName;

	//ファイル名からバイナリファイルを読み込む
	FILE *fp = fopen(fileName.c_str(), "rb");
	if (fp == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load ファイル読み込み失敗");
		return false;
	}

	//メッシュの数を読み込む
	int meshnum;
	fread(&meshnum, sizeof(int), 1, fp);

	//メッシュの数分回す
	for (int i = 0; i < meshnum; i++)
	{
		//頂点数読み込む
		int vertexnum;
		fread(&vertexnum, sizeof(int), 1, fp);

		//頂点データ読み込む
		std::vector<VERTEX_NORMAL_TEXCOOD> vertex(vertexnum);
		fread(vertex.data(), sizeof(VERTEX_NORMAL_TEXCOOD), vertexnum, fp);

		//インデックス数読み込む
		int indexnum;
		fread(&indexnum, sizeof(int), 1, fp);

		//インデックスデータ読み込む
		std::vector<UINT> index(indexnum);
		fread(index.data(), sizeof(UINT), indexnum, fp);

		struct MatData
		{
			Conv_XM::Vector4f Ambient;
			Conv_XM::Vector4f Diffuse;
		};
		// マテリアルデータを読み込む
		MatData matdata = {};
		fread(&matdata, sizeof(matdata), 1, fp);

		CBufferData::Material material = {};
		material.Ambient = matdata.Ambient;
		material.Diffuse = matdata.Diffuse;

		//テクスチャデータ読み込む
		//文字数読み込み
		int len;
		fread(&len, sizeof(int), 1, fp);

		//文字列読み込み
		char* name = DBG_NEW char[len + 1];
		fread(&name[0], sizeof(char), len, fp);
		name[len] = '\0';

		MeshData mesh;
		mesh.Vertices = vertex;
		mesh.Indices = index;
		mesh.TextureName.reserve(len + 1);
		mesh.TextureName = name;
		mesh.Material = material;
		m_MeshList.emplace_back(mesh);


		delete[] name;
	}

	fclose(fp);

	// 頂点バッファの作成
	if (CreateVertexBuffer() == false)
	{
		Logger::GetInstance().SetLog("Model::Load CreateVertexBufferが失敗");
		return false;
	}

	// インデックスバッファの作成
	if (CreateIndexBuffer() == false)
	{
		Logger::GetInstance().SetLog("Model::Load CreateIndexBufferが失敗");
		return false;
	}
	
	// テクスチャの読み込み
	if (LoadTexture() == false)
	{
		Logger::GetInstance().SetLog("Model::Load LoadTextureが失敗");
		return false;
	}

	return true;
}

void Model::Unload()
{
	for (auto& mesh : m_MeshList)
	{
		// 頂点バッファ解放
		if (mesh.VertexBuffer)
		{
			mesh.VertexBuffer->Release();
			mesh.VertexBuffer = nullptr;
		}
		// インデックスバッファ解放
		if (mesh.IndexBuffer)
		{
			mesh.IndexBuffer->Release();
			mesh.IndexBuffer = nullptr;
		}

		// テクスチャファイル名作成
		int lastpos = (int)m_FileName.find_last_of('/');
		std::string filename = m_FileName.substr(0, lastpos + 1) + mesh.TextureName;
		// テクスチャ破棄
		TextureManager::GetInstance().Unload(filename);
	}
}

void Model::Draw() const
{
	UINT strides = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offsets = 0;

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr)
		{
			Logger::GetInstance().SetLog("Model::Draw() nullptr");
			return;
		}

		// 頂点バッファの設定
		m_DeviceContext->IASetVertexBuffers(0, 1, &mesh.VertexBuffer, &strides, &offsets);

		// インデックスバッファの設定
		m_DeviceContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// テクスチャ設定
		m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &mesh.TextureData);

		// 描画
		m_DeviceContext->DrawIndexed((UINT)mesh.Indices.size(), 0, 0);
	}
}

void Model::InstancingDraw(ID3D11Buffer * instancingBuffer, int instancingNum) const
{
	UINT stride[2] = { sizeof(VERTEX_NORMAL_TEXCOOD), sizeof(Conv_XM::Matrix4x4f) };
	UINT offset[2] = { 0, 0 };

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr)
		{
			Logger::GetInstance().SetLog("Model::Draw() nullptr");
			return;
		}

		// 頂点バッファの設定
		ID3D11Buffer* pBuf[2] = { mesh.VertexBuffer, instancingBuffer };
		m_DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);

		// インデックスバッファの設定
		m_DeviceContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// テクスチャ設定
		m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &mesh.TextureData);

		// 描画
		m_DeviceContext->DrawIndexedInstanced((UINT)mesh.Indices.size(), (UINT)instancingNum, 0, 0, 0);
	}
}

void Model::InstancingColorDraw(ID3D11Buffer * instancingBuffer, int instancingNum, UINT * stride, UINT * offset) const
{
	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr)
		{
			Logger::GetInstance().SetLog("Model::Draw() nullptr");
			return;
		}

		// 頂点バッファの設定
		ID3D11Buffer* pBuf[2] = { mesh.VertexBuffer, instancingBuffer };
		m_DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);

		// インデックスバッファの設定
		m_DeviceContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// テクスチャ設定
		m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &mesh.TextureData);

		// 描画
		m_DeviceContext->DrawIndexedInstanced((UINT)mesh.Indices.size(), (UINT)instancingNum, 0, 0, 0);
	}
}

bool Model::CreateVertexBuffer()
{
	for (auto& mesh : m_MeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(VERTEX_NORMAL_TEXCOOD) * (UINT)mesh.Vertices.size();
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.Vertices[0];
		sub_resource.SysMemPitch = 0;
		sub_resource.SysMemSlicePitch = 0;
		
		HRESULT hr;
		ID3D11Buffer* vertexBuffer = nullptr;

		hr = m_Device->CreateBuffer(&buffer_desc, &sub_resource, &vertexBuffer);
		if (FAILED(hr))
		{
			Logger::GetInstance().SetLog("Model::CreateVertexBuffer CreateBufferが失敗");
			return false;
		}

		// メッシュリストに頂点バッファを登録
		mesh.VertexBuffer = vertexBuffer;
	}

	return true;
}

bool Model::CreateIndexBuffer()
{
	for (auto& mesh : m_MeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)sizeof(UINT) * (UINT)mesh.Indices.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;		
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.Indices[0];
		sub_resource.SysMemPitch = 0;	
		sub_resource.SysMemSlicePitch = 0;

		HRESULT hr;
		ID3D11Buffer* indexBuffer = nullptr;

		hr = m_Device->CreateBuffer(&buffer_desc, &sub_resource, &indexBuffer);
		if (FAILED(hr))
		{
			Logger::GetInstance().SetLog("Model::CreateIndexBuffer CreateBufferが失敗");
			return false;
		}

		// メッシュリストにインデックスバッファを登録
		mesh.IndexBuffer = indexBuffer;
	}

	return true;
}

bool Model::LoadTexture()
{
	for (auto& mesh : m_MeshList)
	{
		if (mesh.TextureName.size() > 0)
		{
			// テクスチャファイル名作成
			int lastpos = (int)m_FileName.find_last_of('/');
			std::string filename = m_FileName.substr(0, lastpos + 1) + mesh.TextureName;

			// テクスチャ読み込み
			mesh.TextureData = TextureManager::GetInstance().Load(filename);

			// nullチェック
			if (mesh.TextureData == nullptr)
			{
				Logger::GetInstance().SetLog("Model::LoadTexture TextureDataがnullptr");
				return false;
			}
		}
		else
		{
			ID3D11ShaderResourceView* srv = nullptr;
			mesh.TextureData = srv;
		}
	}
	return true;
}
