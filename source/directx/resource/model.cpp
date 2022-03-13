
#include"../../system/main.h"
#include"model.h"
#include"../renderer.h"
#include"../resource/texture_manager.h"
#include"../render_data/srv_data.h"

Model::Model()
	: m_Device(nullptr)
	, m_DeviceContext(nullptr)
{
	// �f�o�C�X�̎擾
	m_Device = Renderer::GetInstance().GetDevice();
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load m_Device��nullptr");
	}

	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load m_DeviceContext��nullptr");
	}
}

Model::~Model()
{
}

bool Model::Load(const std::string & fileName)
{
	// �f�o�C�X��null
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load device��nullptr");
		return false;
	}

	m_FileName = fileName;

	//�t�@�C��������o�C�i���t�@�C����ǂݍ���
	FILE *fp = fopen(fileName.c_str(), "rb");
	if (fp == nullptr)
	{
		Logger::GetInstance().SetLog("Model::Load �t�@�C���ǂݍ��ݎ��s");
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
		std::vector<VERTEX_NORMAL_TEXCOOD> vertex(vertexnum);
		fread(vertex.data(), sizeof(VERTEX_NORMAL_TEXCOOD), vertexnum, fp);

		//�C���f�b�N�X���ǂݍ���
		int indexnum;
		fread(&indexnum, sizeof(int), 1, fp);

		//�C���f�b�N�X�f�[�^�ǂݍ���
		std::vector<UINT> index(indexnum);
		fread(index.data(), sizeof(UINT), indexnum, fp);

		struct MatData
		{
			Conv_XM::Vector4f Ambient;
			Conv_XM::Vector4f Diffuse;
		};
		// �}�e���A���f�[�^��ǂݍ���
		MatData matdata = {};
		fread(&matdata, sizeof(matdata), 1, fp);

		CBufferData::Material material = {};
		material.Ambient = matdata.Ambient;
		material.Diffuse = matdata.Diffuse;

		//�e�N�X�`���f�[�^�ǂݍ���
		//�������ǂݍ���
		int len;
		fread(&len, sizeof(int), 1, fp);

		//������ǂݍ���
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

	// ���_�o�b�t�@�̍쐬
	if (CreateVertexBuffer() == false)
	{
		Logger::GetInstance().SetLog("Model::Load CreateVertexBuffer�����s");
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�̍쐬
	if (CreateIndexBuffer() == false)
	{
		Logger::GetInstance().SetLog("Model::Load CreateIndexBuffer�����s");
		return false;
	}
	
	// �e�N�X�`���̓ǂݍ���
	if (LoadTexture() == false)
	{
		Logger::GetInstance().SetLog("Model::Load LoadTexture�����s");
		return false;
	}

	return true;
}

void Model::Unload()
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
}

void Model::Draw() const
{
	UINT strides = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offsets = 0;

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr)
		{
			Logger::GetInstance().SetLog("Model::Draw() nullptr");
			return;
		}

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

void Model::InstancingDraw(ID3D11Buffer * instancingBuffer, int instancingNum) const
{
	UINT stride[2] = { sizeof(VERTEX_NORMAL_TEXCOOD), sizeof(Conv_XM::Matrix4x4f) };
	UINT offset[2] = { 0, 0 };

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr)
		{
			Logger::GetInstance().SetLog("Model::Draw() nullptr");
			return;
		}

		// ���_�o�b�t�@�̐ݒ�
		ID3D11Buffer* pBuf[2] = { mesh.VertexBuffer, instancingBuffer };
		m_DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);

		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		m_DeviceContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// �e�N�X�`���ݒ�
		m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &mesh.TextureData);

		// �`��
		m_DeviceContext->DrawIndexedInstanced((UINT)mesh.Indices.size(), (UINT)instancingNum, 0, 0, 0);
	}
}

void Model::InstancingColorDraw(ID3D11Buffer * instancingBuffer, int instancingNum, UINT * stride, UINT * offset) const
{
	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_MeshList)
	{
		if (mesh.VertexBuffer == nullptr || mesh.IndexBuffer == nullptr)
		{
			Logger::GetInstance().SetLog("Model::Draw() nullptr");
			return;
		}

		// ���_�o�b�t�@�̐ݒ�
		ID3D11Buffer* pBuf[2] = { mesh.VertexBuffer, instancingBuffer };
		m_DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);

		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		m_DeviceContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// �e�N�X�`���ݒ�
		m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &mesh.TextureData);

		// �`��
		m_DeviceContext->DrawIndexedInstanced((UINT)mesh.Indices.size(), (UINT)instancingNum, 0, 0, 0);
	}
}

bool Model::CreateVertexBuffer()
{
	for (auto& mesh : m_MeshList)
	{
		//���_�o�b�t�@�쐬
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
			Logger::GetInstance().SetLog("Model::CreateVertexBuffer CreateBuffer�����s");
			return false;
		}

		// ���b�V�����X�g�ɒ��_�o�b�t�@��o�^
		mesh.VertexBuffer = vertexBuffer;
	}

	return true;
}

bool Model::CreateIndexBuffer()
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
			Logger::GetInstance().SetLog("Model::CreateIndexBuffer CreateBuffer�����s");
			return false;
		}

		// ���b�V�����X�g�ɃC���f�b�N�X�o�b�t�@��o�^
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
			// �e�N�X�`���t�@�C�����쐬
			int lastpos = (int)m_FileName.find_last_of('/');
			std::string filename = m_FileName.substr(0, lastpos + 1) + mesh.TextureName;

			// �e�N�X�`���ǂݍ���
			mesh.TextureData = TextureManager::GetInstance().Load(filename);

			// null�`�F�b�N
			if (mesh.TextureData == nullptr)
			{
				Logger::GetInstance().SetLog("Model::LoadTexture TextureData��nullptr");
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
