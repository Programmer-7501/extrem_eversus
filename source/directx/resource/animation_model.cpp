
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
	// �f�o�C�X�̎擾
	m_Device = Renderer::GetInstance().GetDevice();
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load m_Device��nullptr");
	}

	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load m_DeviceContext��nullptr");
	}
}

AnimationModel::~AnimationModel()
{
}

bool AnimationModel::Load(const std::string & fileName)
{
	// �f�o�C�X��null
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load device��nullptr");
		return false;
	}

	m_FileName = fileName;

	//�t�@�C��������o�C�i���t�@�C����ǂݍ���
	FILE *fp = fopen(fileName.c_str(), "rb");
	if (fp == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load �t�@�C���ǂݍ��ݎ��s");
		return false;
	}

	//���b�V���̐���ǂݍ���
	int meshnum;
	fread(&meshnum, sizeof(int), 1, fp);

	//���b�V���̐�����
	for (int i = 0; i < meshnum; i++)
	{
		//���_���ǂݍ���
		int vertexnum;
		fread(&vertexnum, sizeof(int), 1, fp);

		//���_�f�[�^�ǂݍ���
		std::vector<ANIMATION_VERTEX_3D> vertex(vertexnum);

		fread(vertex.data(), sizeof(ANIMATION_VERTEX_3D), vertexnum, fp);

		//�C���f�b�N�X���ǂݍ���
		int indexnum;
		fread(&indexnum, sizeof(int), 1, fp);

		//�C���f�b�N�X�f�[�^�ǂݍ���
		std::vector<UINT> index(indexnum);

		fread(index.data(), sizeof(UINT), indexnum, fp);

		//�e�N�X�`���f�[�^�ǂݍ���
		//�������ǂݍ���
		int len;
		fread(&len, sizeof(int), 1, fp);

		//������ǂݍ���
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

	//�A�j���[�V�����f�[�^�ǂݍ���
	int AnimeDataNum;
	fread(&AnimeDataNum, sizeof(int), 1, fp);

	for (int i = 0; i < AnimeDataNum; i++)
	{
		//�A�j���[�V�����f�[�^�̖��O����������
		std::string animedataname;
		//��������������
		int len;
		fread(&len, sizeof(int), 1, fp);

		//���������������
		char* name = DBG_NEW char[len + 1];
		fread(name, sizeof(char), len, fp);
		name[len] = '\0';
		animedataname = name;
		delete[] name;

		//�{�[���f�[�^���擾

		//�{�[���f�[�^����ǂݍ���
		int bonenum;
		fread(&bonenum, sizeof(int), 1, fp);

		//���v����ǂݍ���
		int framenum;
		fread(&framenum, sizeof(int), 1, fp);

		std::vector<FRAMEDATA> framedata(framenum);

		for (int j = 0; j < framenum; j++)
		{
			//�t���[���ǂݍ���
			int frame = 0;
			fread(&frame, sizeof(int), 1, fp);

			framedata[j].Frame = frame;
			//�{�[���ǂݍ���
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

	// ���_�o�b�t�@�̍쐬
	if (CreateVertexBuffer() == false)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load CreateVertexBuffer�����s");
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�̍쐬
	if (CreateIndexBuffer() == false)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load CreateIndexBuffer�����s");
		return false;
	}

	// �e�N�X�`���̓ǂݍ���
	if (LoadTexture() == false)
	{
		Logger::GetInstance().SetLog("AnimationModel::Load LoadTexture�����s");
		return false;
	}

	return true;
}

void AnimationModel::Unload()
{
	for (auto& mesh : m_MeshList)
	{
		// ���_�o�b�t�@���
		if (mesh.VertexBuffer)
		{
			mesh.VertexBuffer->Release();
			mesh.VertexBuffer = nullptr;
		}
		// �C���f�b�N�X�o�b�t�@���
		if (mesh.IndexBuffer)
		{
			mesh.IndexBuffer->Release();
			mesh.IndexBuffer = nullptr;
		}

		// �e�N�X�`���t�@�C�����쐬
		int lastpos = (int)m_FileName.find_last_of('/');
		std::string filename = m_FileName.substr(0, lastpos + 1) + mesh.TextureName;
		// �e�N�X�`���j��
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

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr || mesh.TextureData == nullptr)
		{
			Logger::GetInstance().SetLog("AnimationModel::Draw() nullptr");
			return;
		}

		//�{�[���f�[�^�����
		renderer.SetBones(m_Bones);

		// ���_�o�b�t�@�̐ݒ�
		m_DeviceContext->IASetVertexBuffers(0, 1, &mesh.VertexBuffer, &strides, &offsets);

		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		m_DeviceContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// �e�N�X�`���ݒ�
		m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &mesh.TextureData);

		// �`��
		m_DeviceContext->DrawIndexed((UINT)mesh.Indices.size(), 0, 0);
	}
}

int AnimationModel::SetBlendMatrix(const std::string & animation001Name, int frame001, const std::string & animation002Name, int frame002, float t)
{
	std::vector<FRAMEDATA> animation001Data;
	std::vector<Conv_XM::Matrix4x4f> animation001Bones;

	//�A�j���[�V�����f�[�^��T��
	auto animation001Iter = m_Animation_Datas.find(animation001Name);

	//�A�j���[�V�����f�[�^�����݂���Ȃ�
	if (animation001Iter != m_Animation_Datas.end())
	{
		//���݂̃A�j���[�V�����f�[�^�ɃZ�b�g����
		animation001Data = animation001Iter->second;
	}
	else
	{
		return 0;
	}

	bool frameinside001 = false;//�t���[���J�E���g�������ĂȂ������f
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

	//�t���[���J�E���g�������Ă���
	if (frameinside001 == false)
	{
		//�t���[���J�E���g��������
		m_CurrentFrame = 1;

		for (int j = 0; j < animation001Data[0].Bones.size(); j++)
		{
			animation001Bones.emplace_back(animation001Data[0].Bones[j]);
		}
	}

	std::vector<FRAMEDATA> animation002Data;
	std::vector<Conv_XM::Matrix4x4f> animation002Bones;

	//�A�j���[�V�����f�[�^��T��
	auto animation002Iter = m_Animation_Datas.find(animation002Name);

	//�A�j���[�V�����f�[�^�����݂���Ȃ�
	if (animation002Iter != m_Animation_Datas.end())
	{
		//���݂̃A�j���[�V�����f�[�^�ɃZ�b�g����
		animation002Data = animation002Iter->second;
	}
	else
	{
		return 0;
	}

	bool frameinside002 = false;//�t���[���J�E���g�������ĂȂ������f
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

	//�t���[���J�E���g�������Ă���
	if (frameinside002 == false)
	{
		//�t���[���J�E���g��������
		m_CurrentFrame = 1;


		for (int j = 0; j < animation002Data[0].Bones.size(); j++)
		{
			animation002Bones.emplace_back(animation002Data[0].Bones[j]);
		}
	}

	// ���g����ɂ���
	m_CurrentBones.clear();

	// ���`��Ԃ���
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

	//�A�j���[�V�����f�[�^��T��
	auto iter = m_Animation_Datas.find(animationName);

	//�A�j���[�V�����f�[�^�����݂���Ȃ�
	if (iter != m_Animation_Datas.end())
	{
		//���݂̃A�j���[�V�����f�[�^�ɃZ�b�g����
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
		//���_�o�b�t�@�쐬
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
			Logger::GetInstance().SetLog("AnimationModel::CreateVertexBuffer CreateBuffer�����s");
			return false;
		}

		// ���b�V�����X�g�ɒ��_�o�b�t�@��o�^
		mesh.VertexBuffer = vertexBuffer;
	}

	return true;
}

bool AnimationModel::CreateIndexBuffer()
{
	for (auto& mesh : m_MeshList)
	{
		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = (UINT)sizeof(UINT) * (UINT)mesh.Indices.size();	// �o�b�t�@�̃T�C�Y
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
			Logger::GetInstance().SetLog("AnimationModel::CreateIndexBuffer CreateBuffer�����s");
			return false;
		}

		// ���b�V�����X�g�ɃC���f�b�N�X�o�b�t�@��o�^
		mesh.IndexBuffer = indexBuffer;
	}

	return true;
}

bool AnimationModel::LoadTexture()
{
	for (auto& mesh : m_MeshList)
	{
		// �e�N�X�`���t�@�C�����쐬
		int lastpos = (int)m_FileName.find_last_of('/');
		std::string filename = m_FileName.substr(0, lastpos + 1) + mesh.TextureName;

		// �e�N�X�`���ǂݍ���
		TextureManager::GetInstance().Load(filename);
		mesh.TextureData = TextureManager::GetInstance().GetSRV(filename);

		// null�`�F�b�N
		if (mesh.TextureData == nullptr)
		{
			Logger::GetInstance().SetLog("AnimationModel::LoadTexture TextureData��nullptr");
			return false;
		}
	}
	return true;
}

int AnimationModel::SetMatrix(const std::string& animationName, int frame)
{
	//�A�j���[�V�����f�[�^��T��
	auto iter = m_Animation_Datas.find(animationName);

	//�A�j���[�V�����f�[�^�����݂���Ȃ�
	if (iter != m_Animation_Datas.end())
	{
		//���݂̃A�j���[�V�����f�[�^�ɃZ�b�g����
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

	//���݂̃A�j���[�V�����f�[�^�������Ȃ烊�^�[��
	if (m_CurrentAnimeData.empty() == true)
	{
		return 0;
	}

	m_CurrentBones.clear();


	bool frameinside = false;//�t���[���J�E���g�������ĂȂ������f
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

	//�t���[���J�E���g�������Ă���
	if (frameinside == false)
	{
		//�t���[���J�E���g��������
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
