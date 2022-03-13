
#include"../../../system/main.h"
#include "field_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../actor/actor.h"

FieldComponent::FieldComponent(Actor * owner)
	:MeshComponent(owner)
{
}

FieldComponent::~FieldComponent()
{
}

void FieldComponent::Draw() const
{
	// �����Ȃ��Ȃ烊�^�[��
	if (m_Visible == false)
	{
		return;
	}

	// null�`�F�b�N
	if (m_Owner == nullptr || m_DeviceContext == nullptr || m_VertexBuffer == nullptr || m_IndexBuffer == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_InputLayout == nullptr || m_ShaderResourceView == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false)
	{
		return;
	}

	Renderer& renderer = Renderer::GetInstance();

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	Conv_XM::Vector3f position = m_Owner->GetPosition();
	Conv_XM::Vector3f scale = m_Owner->GetScale();
	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)mtxTranslate;
	renderer.SetWorldMatrix(mtxWorld);

	// �}�e���A���Z�b�g
	renderer.SetMaterial(m_Material);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_NORMAL_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �C���f�b�N�X�o�b�t�@��ݒ�
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//�e�N�X�`���ݒ�
	m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &m_ShaderResourceView);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�`��
	m_DeviceContext->DrawIndexed(6, 0, 0);
}

bool FieldComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetShader�����s");
		return false;
	}

	// ���b�V���쐬
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init CreateMesh�����s");
		return false;
	}

	return true;
}

bool FieldComponent::CreateMesh()
{
	// �f�o�C�X�̎擾
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// null�`�F�b�N
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init device��nullptr");
		return false;
	}

	HRESULT hr;

	// ���_�ݒ�
	VERTEX_NORMAL_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f, 0.0f, 0.5f},{0.0f,1.0f,0.0f}, {0.0f,0.0f} };
	vertex[1] = { { 0.5f, 0.0f, 0.5f},{0.0f,1.0f,0.0f}, {1.0f,0.0f} };
	vertex[2] = { {-0.5f, 0.0f, -0.5f}, {0.0f,1.0f,0.0f},{0.0f,1.0f} };
	vertex[3] = { { 0.5f, 0.0f, -0.5f}, {0.0f,1.0f,0.0f},{1.0f,1.0f} };


	// ���_�o�b�t�@����
	// ������������
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_NORMAL_TEXCOOD) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = vertex;

	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init CreateBuffer���s");
		return false;
	}

	// �l�p�`�̃C���f�b�N�X���`
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// �o�b�t�@���쐬
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	// �C���f�b�N�X���̒ǉ�
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = index;

	// �C���f�b�N�X�o�b�t�@���쐬
	hr = device->CreateBuffer(&indexBufferDesc, &indexInitData, m_IndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("SpriteComponent::Init CreateBuffer���s");
		return false;
	}

	return true;
}

bool FieldComponent::GetDeviceContext()
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

bool FieldComponent::GetShader()
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
