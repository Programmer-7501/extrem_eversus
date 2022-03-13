
#include"../../../../system/main.h"
#include "obb_debug_view_component.h"
#include"../../../../directx/renderer.h"
#include"../../../actor/actor.h"
#include"../../../../directx/resource/shader_manager.h"
#include"../../../scene/scene.h"
#include"../../../scene/scene_manager.h"
#include"../../../scene_function/scene_render_manager.h"

bool OBBDebugViewComponent::m_IsUseFlag = true;

void OBBDebugViewComponent::SetIsUseFlag(bool flag)
{
	m_IsUseFlag = flag;
}

OBBDebugViewComponent::OBBDebugViewComponent(Actor * owner)
	: MeshComponent(owner)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_VertexBuffer(nullptr)
	, m_IndexNum(0)
	, m_Position(Conv_XM::Vector3f(0.0f,0.0f,0.0f))
	, m_Scale(Conv_XM::Vector3f(1.0f, 1.0f, 1.0f))
	, m_Quaternion(DirectX::XMQuaternionIdentity())
{
	// �t�H���[�h�����_�����O�ɕύX
	m_RenderingWay = RENDERING_FORWARD;
}

void OBBDebugViewComponent::Draw() const
{
	// null�`�F�b�N
	if (m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_VertexBuffer == nullptr)
	{
		Logger::GetInstance().SetLog("OBBDebugViewComponent::Draw nullptr");
		return;
	}
	if (m_Owner->GetActive() == false || m_IsUseFlag == false || m_Visible == false)
	{
		return;
	}

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxT, mtxR, mtxS, mtxW;
	mtxT = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	mtxR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);
	mtxS = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	mtxW = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;
	Renderer::GetInstance().SetWorldMatrix(mtxW);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_COLOR);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �`��
	m_DeviceContext->DrawIndexed(m_IndexNum, 0, 0);
}

bool OBBDebugViewComponent::LoadData()
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

bool OBBDebugViewComponent::CreateMesh()
{
	Renderer& renderer = Renderer::GetInstance();
	ID3D11Device* device = renderer.GetDevice();

	//���_���
	VERTEX_COLOR vertex[8];

	vertex[0].Position = Conv_XM::Vector3f(-0.5f, 0.5f, -0.5f);
	vertex[0].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[1].Position = Conv_XM::Vector3f(0.5f, 0.5f, -0.5f);
	vertex[1].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[2].Position = Conv_XM::Vector3f(-0.5f, -0.5f, -0.5f);
	vertex[2].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[3].Position = Conv_XM::Vector3f(0.5f, -0.5f, -0.5f);
	vertex[3].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[4].Position = Conv_XM::Vector3f(-0.5f, 0.5f, 0.5f);
	vertex[4].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[5].Position = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
	vertex[5].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[6].Position = Conv_XM::Vector3f(-0.5f, -0.5f, 0.5f);
	vertex[6].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertex[7].Position = Conv_XM::Vector3f(0.5f, -0.5f, 0.5f);
	vertex[7].Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);


	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_COLOR) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	device->CreateBuffer(&bd, &sd, m_VertexBuffer.GetAddressOf());


	WORD index[]{
		0,	1,
		1,	3,
		3,	2,
		2,	0,
		0,	4,
		4,	5,
		5,	1,
		3,	7,
		7,	5,
		2,	6,
		6,	4,
		6,	7
	};

	m_IndexNum = ARRAYSIZE(index);

	// �C���f�b�N�X�o�b�t�@�Ƃ��č쐬���邽�߂̏��ݒ�
	D3D11_BUFFER_DESC index_buffer_desc;
	index_buffer_desc.ByteWidth = sizeof(WORD) * m_IndexNum;
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	// �C���f�b�N�X�o�b�t�@�̏����l�w��
	D3D11_SUBRESOURCE_DATA index_init_data;
	// �o�b�t�@�̏����l�ݒ�
	index_init_data.pSysMem = index;
	// �C���f�b�N�X�o�b�t�@�ł͎g�p���Ȃ�
	index_init_data.SysMemPitch = 0;
	index_init_data.SysMemSlicePitch = 0;

	// �C���f�b�N�X�o�b�t�@�쐬
	device->CreateBuffer(&index_buffer_desc, &index_init_data, m_IndexBuffer.GetAddressOf());

	return true;
}

bool OBBDebugViewComponent::GetDeviceContext()
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

bool OBBDebugViewComponent::GetShader()
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
