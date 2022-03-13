
#include"../../../system/main.h"
#include "grid_component.h"
#include"../../../directx/renderer.h"
#include"../../actor/actor.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../../scene_function/scene_render_manager.h"

namespace GridData
{
	static constexpr float k_Size = 10.0f;
	static constexpr int k_Count = 10;
	static const Conv_XM::Vector4f k_Color(0.0f, 1.0f, 0.2f, 1.0f);
}

GridComponent::GridComponent(class Actor* owner)
	: MeshComponent(owner)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_VertexCount(0)
	, m_VertexBuffer(nullptr)
{
	// �t�H���[�h�����_�����O�ɕύX
	m_RenderingWay = RENDERING_FORWARD;
}

void GridComponent::Draw() const
{
	// null�`�F�b�N
	if (m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_VertexBuffer == nullptr)
	{
		Logger::GetInstance().SetLog("GridComponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false)
	{
		return;
	}

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtx = DirectX::XMMatrixIdentity();
	Renderer::GetInstance().SetWorldMatrix(mtx);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_COLOR);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �`��
	m_DeviceContext->Draw(m_VertexCount, 0);
}

bool GridComponent::LoadData()
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

	// ���b�V���̐���
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init CreateMesh�����s");
		return false;
	}

	return true;
}

bool GridComponent::CreateMesh()
{
	// �f�o�C�X�̎擾
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("GridComponent::CreateMesh device��nullptr");
		return false;
	}

	m_VertexCount = (GridData::k_Count + 1) * 4;

	float length = GridData::k_Size * GridData::k_Count;

	float start = -length * 0.5f;

	float end = length * 0.5f;

	VERTEX_COLOR* vertex = nullptr;
	vertex = DBG_NEW VERTEX_COLOR[m_VertexCount];

	for (int i = 0; i < GridData::k_Count + 1; i++)
	{
		if (i >= m_VertexCount)
		{
			break;
		}

		int n = (GridData::k_Count + 1) * 2;
		vertex[i * 2].Position = Conv_XM::Vector3f(start + GridData::k_Size * i, 0.0f, start);
		vertex[i * 2].Color = GridData::k_Color;

		vertex[i * 2 + 1].Position = Conv_XM::Vector3f(start + GridData::k_Size * i, 0.0f, end);
		vertex[i * 2 + 1].Color = GridData::k_Color;

		vertex[n + i * 2].Position = Conv_XM::Vector3f(start, 0.0f, start + GridData::k_Size * i);
		vertex[n + i * 2].Color = GridData::k_Color;

		vertex[n + i * 2 + 1].Position = Conv_XM::Vector3f(end, 0.0f, start + GridData::k_Size * i);
		vertex[n + i * 2 + 1].Color = GridData::k_Color;
	}

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_COLOR) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData{};
	vertexInitData.pSysMem = vertex;

	HRESULT hr;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		if (vertex)
		{
			delete[] vertex;
			vertex = nullptr;
		}
		Logger::GetInstance().SetLog("GridComponent::CreateMesh CreateBuffer�����s");
		return false;
	}

	if (vertex)
	{
		delete[] vertex;
		vertex = nullptr;
	}

	return true;
}

bool GridComponent::GetDeviceContext()
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

bool GridComponent::GetShader()
{
	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_GRID);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_GRID);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_GRID);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}
