
#include"../../../system/main.h"
#include "cube_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../../directx/resource/texture_manager.h"
#include"../../actor/actor.h"

CubeComponent::CubeComponent(Actor * owner)
	:MeshComponent(owner)
{
	m_IsShadowInput = true;
}

CubeComponent::~CubeComponent()
{
}

void CubeComponent::Draw() const
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//CBufferData::Material material = {};
	//material.Metaric = 0.0f;
	//material.Smoothness = 0.0f;
	//Renderer::GetInstance().SetMaterial(material);

	Conv_XM::Matrix4x4f mat = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);
	//�}�g���N�X�ݒ�
	Renderer::GetInstance().SetWorldMatrix(m_Owner->GetWorldMatrix());

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//�e�N�X�`���ݒ�
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_ShaderResourceView);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer::GetInstance().GetDeviceContext()->DrawIndexed(36, 0, 0);
}

void CubeComponent::ShadowDraw() const
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(NULL, NULL, 0);

	Conv_XM::Matrix4x4f mat = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);
	//�}�g���N�X�ݒ�
	Renderer::GetInstance().SetWorldMatrix(m_Owner->GetWorldMatrix());

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//�e�N�X�`���ݒ�
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_ShaderResourceView);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer::GetInstance().GetDeviceContext()->DrawIndexed(36, 0, 0);
}

bool CubeComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::LoadData GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::LoadData GetShader�����s");
		return false;
	}

	m_ShaderResourceView = TextureManager::GetInstance().Load("asset/texture/purple.png");

	VERTEX_NORMAL_TEXCOOD vertex[24];

	//�O
	{
		vertex[0].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
		vertex[0].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[0].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[1].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
		vertex[1].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[1].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[2].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
		vertex[2].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[2].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[3].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
		vertex[3].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[3].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//�E
	{
		vertex[4].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
		vertex[4].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[4].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[5].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
		vertex[5].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[5].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[6].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
		vertex[6].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[6].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[7].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
		vertex[7].Normal = Conv_XM::Vector3f(1.0f, 0.0f, 0.0f);
		vertex[7].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//���
	{
		vertex[8].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
		vertex[8].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[8].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[9].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
		vertex[9].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[9].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[10].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
		vertex[10].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[10].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[11].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
		vertex[11].Normal = Conv_XM::Vector3f(0.0f, 0.0f, -1.0f);
		vertex[11].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//��
	{
		vertex[12].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
		vertex[12].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[12].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[13].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
		vertex[13].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[13].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[14].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
		vertex[14].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[14].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[15].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
		vertex[15].Normal = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
		vertex[15].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//��
	{
		vertex[16].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
		vertex[16].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[16].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[17].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
		vertex[17].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[17].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[18].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
		vertex[18].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[18].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[19].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
		vertex[19].Normal = Conv_XM::Vector3f(-1.0f, 0.0f, 0.0f);
		vertex[19].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//��
	{
		vertex[20].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
		vertex[20].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[20].TexCoord = Conv_XM::Vector2f(0.0f, 0.0f);

		vertex[21].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
		vertex[21].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[21].TexCoord = Conv_XM::Vector2f(1.0f, 0.0f);

		vertex[22].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
		vertex[22].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[22].TexCoord = Conv_XM::Vector2f(0.0f, 1.0f);

		vertex[23].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
		vertex[23].Normal = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
		vertex[23].TexCoord = Conv_XM::Vector2f(1.0f, 1.0f);
	}

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_NORMAL_TEXCOOD) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, m_VertexBuffer.GetAddressOf());


	WORD index[]{
	 0,  1,  2,     3,  2,  1,
	 4,  5,  6,     7,  6,  5,
	 8,  9, 10,    11, 10,  9,
	12, 13, 14,    15, 14, 13,
	16, 17, 18,    19, 18, 17,
	20, 21, 22,    23, 22, 21,
	};

	WORD indexcount = ARRAYSIZE(index);

	// �C���f�b�N�X�o�b�t�@�Ƃ��č쐬���邽�߂̏��ݒ�
	D3D11_BUFFER_DESC index_buffer_desc;
	index_buffer_desc.ByteWidth = sizeof(UWORD) * indexcount;            // �o�b�t�@�̃T�C�Y(���_��)
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;              // �g�p���@
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;      // �o�b�t�@�̎��(�C���f�b�N�X�o�b�t�@)
	index_buffer_desc.CPUAccessFlags = 0;                       // CPU�A�N�Z�X�ݒ�
	index_buffer_desc.MiscFlags = 0;                            // ���̑��̃I�v�V����
	index_buffer_desc.StructureByteStride = 0;                  // �\���̂ł͂Ȃ��̂�0

	 // �C���f�b�N�X�o�b�t�@�̏����l�w��
	D3D11_SUBRESOURCE_DATA index_init_data;
	// �o�b�t�@�̏����l�ݒ�
	index_init_data.pSysMem = index;
	// �C���f�b�N�X�o�b�t�@�ł͎g�p���Ȃ�
	index_init_data.SysMemPitch = 0;
	index_init_data.SysMemSlicePitch = 0;

	// �C���f�b�N�X�o�b�t�@�쐬
	Renderer::GetInstance().GetDevice()->CreateBuffer(&index_buffer_desc, &index_init_data, m_IndexBuffer.GetAddressOf());

	return true;
}

bool CubeComponent::GetDeviceContext()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_DeviceContext��nullptr");
		return false;
	}

	return true;
}

bool CubeComponent::GetShader()
{
	// �V�F�[�_�[�}�l�[�W���[�̎擾
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_GBUFFERINPUT);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_GBUFFERINPUT);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_GBUFFERINPUT);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}
