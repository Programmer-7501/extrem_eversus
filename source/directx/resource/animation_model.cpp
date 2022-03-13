
#include"../../system/main.h"
#include "animation_model.h"
#include"../renderer.h"
#include"../resource/texture_manager.h"
#include"../render_data/srv_data.h"

AnimationModel::AnimationModel()
	: m_Device(nullptr)
	, m_DeviceContext(nullptr)
	, m_CurrentFrame(0)
	, m_IsIdentity(false)
{
	// デバイスの取得
	m_Device = Renderer::GetInstance().GetDevice();
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load m_Deviceがnullptr");
	}

	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load m_DeviceContextがnullptr");
	}
}

AnimationModel::~AnimationModel()
{
}

bool AnimationModel::Load(const std::string & fileName)
{
	// デバイスがnull
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load deviceがnullptr");
		return false;
	}

	m_FileName = fileName;

	//ファイル名からバイナリファイルを読み込む
	FILE *fp = fopen(fileName.c_str(), "rb");
	if (fp == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load ファイル読み込み失敗");
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
		std::vector<ANIMATION_VERTEX_3D> vertex(vertexnum);

		fread(vertex.data(), sizeof(ANIMATION_VERTEX_3D), vertexnum, fp);

		//インデックス数読み込む
		int indexnum;
		fread(&indexnum, sizeof(int), 1, fp);

		//インデックスデータ読み込む
		std::vector<UINT> index(indexnum);

		fread(index.data(), sizeof(UINT), indexnum, fp);

		//テクスチャデータ読み込む
		//文字数読み込み
		int len;
		fread(&len, sizeof(int), 1, fp);

		//文字列読み込み
		char* name = DBG_NEW char[len + 1];
		fread(&name[0], sizeof(char), len, fp);
		name[len] = '\0';

		AnimationMeshData mesh;
		mesh.Vertices = vertex;
		mesh.Indices = index;
		mesh.TextureName = name;
		m_MeshList.emplace_back(mesh);

		delete[] name;
	}

	//アニメーションデータ読み込み
	int AnimeDataNum;
	fread(&AnimeDataNum, sizeof(int), 1, fp);

	for (int i = 0; i < AnimeDataNum; i++)
	{
		//アニメーションデータの名前を書き込む
		std::string animedataname;
		//文字数書き込む
		int len;
		fread(&len, sizeof(int), 1, fp);

		//文字列を書き込む
		char* name = DBG_NEW char[len + 1];
		fread(name, sizeof(char), len, fp);
		name[len] = '\0';
		animedataname = name;
		delete[] name;

		//ボーンデータを取得

		//ボーンデータ数を読み込む
		int bonenum;
		fread(&bonenum, sizeof(int), 1, fp);

		//合計数を読み込む
		int framenum;
		fread(&framenum, sizeof(int), 1, fp);

		std::vector<FRAMEDATA> framedata(framenum);

		for (int j = 0; j < framenum; j++)
		{
			//フレーム読み込み
			int frame = 0;
			fread(&frame, sizeof(int), 1, fp);

			framedata[j].Frame = frame;
			//ボーン読み込み
			for (int k = 0; k < bonenum; k++)
			{
				Conv_XM::Matrix4x4f bone;
				fread(&bone, sizeof(Conv_XM::Matrix4x4f), 1, fp);
				framedata[j].Bones.emplace_back(bone);
			}
		}
		m_Animation_Datas.emplace(animedataname, framedata);
	}

	fclose(fp);

	// 頂点バッファの作成
	if (CreateVertexBuffer() == false)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load CreateVertexBufferが失敗");
		return false;
	}

	// インデックスバッファの作成
	if (CreateIndexBuffer() == false)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load CreateIndexBufferが失敗");
		return false;
	}

	// テクスチャの読み込み
	if (LoadTexture() == false)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load LoadTextureが失敗");
		return false;
	}

	return true;
}

void AnimationModel::Unload()
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

	m_CurrentAnimeData = m_Animation_Datas[0];
}

void AnimationModel::Update()
{

}

void AnimationModel::Draw() const
{
	Renderer& renderer = Renderer::GetInstance();

	UINT strides = sizeof(ANIMATION_VERTEX_3D);
	UINT offsets = 0;

	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr || mesh.TextureData == nullptr)
		{
			Logger::GetInstance().SetLog("AnimationModel::Draw() nullptr");
			return;
		}

		//ボーンデータを入力
		renderer.SetBones(m_Bones);

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

int AnimationModel::SetBlendMatrix(const std::string & animation001Name, int frame001, const std::string & animation002Name, int frame002, float t)
{
	std::vector<FRAMEDATA> animation001Data;
	std::vector<Conv_XM::Matrix4x4f> animation001Bones;

	//アニメーションデータを探す
	auto animation001Iter = m_Animation_Datas.find(animation001Name);

	//アニメーションデータが存在するなら
	if (animation001Iter != m_Animation_Datas.end())
	{
		//現在のアニメーションデータにセットする
		animation001Data = animation001Iter->second;
	}
	else
	{
		return 0;
	}

	bool frameinside001 = false;//フレームカウントが超えてないか判断
	for (int i = 0; i < (int)animation001Data.size() - 1; i++)
	{
		if (animation001Data[i + 1].Frame > frame001)
		{
			frameinside001 = true;

			for (int j = 0; j < animation001Data[i].Bones.size(); j++)
			{
				animation001Bones.emplace_back(animation001Data[i].Bones[j]);
			}

			break;
		}
	}

	//フレームカウントが超えてたら
	if (frameinside001 == false)
	{
		//フレームカウントを初期化
		m_CurrentFrame = 1;

		for (int j = 0; j < animation001Data[0].Bones.size(); j++)
		{
			animation001Bones.emplace_back(animation001Data[0].Bones[j]);
		}
	}

	std::vector<FRAMEDATA> animation002Data;
	std::vector<Conv_XM::Matrix4x4f> animation002Bones;

	//アニメーションデータを探す
	auto animation002Iter = m_Animation_Datas.find(animation002Name);

	//アニメーションデータが存在するなら
	if (animation002Iter != m_Animation_Datas.end())
	{
		//現在のアニメーションデータにセットする
		animation002Data = animation002Iter->second;
	}
	else
	{
		return 0;
	}

	bool frameinside002 = false;//フレームカウントが超えてないか判断
	for (int i = 0; i < (int)animation002Data.size() - 1; i++)
	{
		if (animation002Data[i + 1].Frame > frame002)
		{
			frameinside002 = true;

			for (int j = 0; j < animation002Data[i].Bones.size(); j++)
			{
				animation002Bones.emplace_back(animation002Data[i].Bones[j]);
			}

			break;
		}
	}

	//フレームカウントが超えてたら
	if (frameinside002 == false)
	{
		//フレームカウントを初期化
		m_CurrentFrame = 1;


		for (int j = 0; j < animation002Data[0].Bones.size(); j++)
		{
			animation002Bones.emplace_back(animation002Data[0].Bones[j]);
		}
	}

	// 中身を空にする
	m_CurrentBones.clear();

	// 線形補間する
	for (int i = 0; i < static_cast<int>(animation001Bones.size()); i++)
	{
		Conv_XM::Matrix4x4f result;
		Conv_XM::Matrix4x4f::Linear(&result, animation001Bones[i], animation002Bones[i], t);
		m_CurrentBones.emplace_back(result);
	}

	int size = static_cast<int>(m_CurrentBones.size());
	for (int i = size; i < MAX_BONE_NUM; i++)
	{
		m_CurrentBones.emplace_back(DirectX::XMMatrixIdentity());
	}

	for (int i = 0; i < MAX_BONE_NUM; i++)
	{
		m_Bones.bones[i] = m_CurrentBones[i];
	}
	return m_CurrentFrame;
}

int AnimationModel::GetMaxFrame(const std::string & animationName)
{
	int maxFrame = 0;

	//アニメーションデータを探す
	auto iter = m_Animation_Datas.find(animationName);

	//アニメーションデータが存在するなら
	if (iter != m_Animation_Datas.end())
	{
		//現在のアニメーションデータにセットする
		maxFrame = iter->second[iter->second.size() - 1].Frame;
	}
	else
	{
		maxFrame = 0;
	}

	return maxFrame;
}

bool AnimationModel::CreateVertexBuffer()
{
	for (auto& mesh : m_MeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(ANIMATION_VERTEX_3D) * (UINT)mesh.Vertices.size();
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
			Logger::GetInstance().SetLog("AnimationModel::CreateVertexBuffer CreateBufferが失敗");
			return false;
		}

		// メッシュリストに頂点バッファを登録
		mesh.VertexBuffer = vertexBuffer;
	}

	return true;
}

bool AnimationModel::CreateIndexBuffer()
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
			Logger::GetInstance().SetLog("AnimationModel::CreateIndexBuffer CreateBufferが失敗");
			return false;
		}

		// メッシュリストにインデックスバッファを登録
		mesh.IndexBuffer = indexBuffer;
	}

	return true;
}

bool AnimationModel::LoadTexture()
{
	for (auto& mesh : m_MeshList)
	{
		// テクスチャファイル名作成
		int lastpos = (int)m_FileName.find_last_of('/');
		std::string filename = m_FileName.substr(0, lastpos + 1) + mesh.TextureName;

		// テクスチャ読み込み
		TextureManager::GetInstance().Load(filename);
		mesh.TextureData = TextureManager::GetInstance().GetSRV(filename);

		// nullチェック
		if (mesh.TextureData == nullptr)
		{
			Logger::GetInstance().SetLog("AnimationModel::LoadTexture TextureDataがnullptr");
			return false;
		}
	}
	return true;
}

int AnimationModel::SetMatrix(const std::string& animationName, int frame)
{
	//アニメーションデータを探す
	auto iter = m_Animation_Datas.find(animationName);

	//アニメーションデータが存在するなら
	if (iter != m_Animation_Datas.end())
	{
		//現在のアニメーションデータにセットする
		m_CurrentAnimeData = iter->second;
	}
	else
	{
		return 0;
	}

	m_CurrentFrame = frame;

	if (m_IsIdentity == true)
	{
		m_CurrentBones.clear();
		for (int i = 0; i < MAX_BONE_NUM; i++)
		{
			m_CurrentBones.emplace_back(DirectX::XMMatrixIdentity());
		}

		for (int i = 0; i < MAX_BONE_NUM; i++)
		{
			m_Bones.bones[i] = m_CurrentBones[i];
		}
		m_CurrentFrame = 0;

		return m_CurrentFrame;
	}

	//現在のアニメーションデータが無いならリターン
	if (m_CurrentAnimeData.empty() == true)
	{
		return 0;
	}

	m_CurrentBones.clear();


	bool frameinside = false;//フレームカウントが超えてないか判断
	for (int i = 0; i < (int)m_CurrentAnimeData.size() - 1; i++)
	{
		if (m_CurrentAnimeData[i + 1].Frame > m_CurrentFrame)
		{
			frameinside = true;

			for (int j = 0; j < m_CurrentAnimeData[i].Bones.size(); j++)
			{
				m_CurrentBones.emplace_back(m_CurrentAnimeData[i].Bones[j]);
			}

			break;
		}
	}

	//フレームカウントが超えてたら
	if (frameinside == false)
	{
		//フレームカウントを初期化
		m_CurrentFrame = 0;

		for (int j = 0; j < m_CurrentAnimeData[0].Bones.size(); j++)
		{
			m_CurrentBones.emplace_back(m_CurrentAnimeData[0].Bones[j]);
		}
	}

	int size = static_cast<int>(m_CurrentBones.size());
	for (int i = size; i < MAX_BONE_NUM; i++)
	{
		m_CurrentBones.emplace_back(DirectX::XMMatrixIdentity());
	}

	for (int i = 0; i < MAX_BONE_NUM; i++)
	{
		m_Bones.bones[i] = m_CurrentBones[i];
	}

	return m_CurrentFrame;
}
