
#include"../../../system/main.h"
#include "changeable_billboard_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../actor/actor.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../component/cameracomponent/camera_component.h"

ChangeableBillBoardComponent::ChangeableBillBoardComponent(Actor * owner)
	:BillBoardComponent(owner)
	, m_Color(1.0f,1.0f,1.0f,1.0f)
	, m_IsUpdate(true)
{
}

ChangeableBillBoardComponent::~ChangeableBillBoardComponent()
{
}

void ChangeableBillBoardComponent::Update()
{	
	// �X�V�t���O�������Ă���Ȃ�
	if (m_IsUpdate == true)
	{
		// ���_�X�V
		UpdateVetexData();
	}
}

void ChangeableBillBoardComponent::Draw() const
{
	// �����Ȃ��Ȃ烊�^�[��
	if (m_Visible == false)
	{
		return;
	}

	// null�`�F�b�N
	if (m_Owner == nullptr || m_DeviceContext == nullptr || m_VertexBuffer == nullptr
		|| m_IndexBuffer == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr
		|| m_InputLayout == nullptr || m_ShaderResourceView == nullptr || m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Draw nullptr");
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


	// �J�����R���|�[�l���g�擾
	CameraComponent* camera = m_SceneRenderManager->GetMainCameraComponent();
	// �r���[�}�g���N�X�擾
	Conv_XM::Matrix4x4f matView = camera->GetViewMatrix();

	//�r���[�s��̋t�s��łȂ������s�ړ�������0
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	matView._44 = 1.0f;

	DirectX::XMVECTOR determinant;
	Conv_XM::Matrix4x4f invView = XMMatrixInverse(&determinant, matView);

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)invView * (DirectX::XMMATRIX)mtxTranslate;
	renderer.SetWorldMatrix(mtxWorld);

	// �}�e���A���Z�b�g
	renderer.SetMaterial(m_Material);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_COLOR_TEXCOOD);
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

void ChangeableBillBoardComponent::UpdateVetexData()
{
	//���_��������
	D3D11_MAPPED_SUBRESOURCE msr;
	m_DeviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_COLOR_TEXCOOD* vertex = (VERTEX_COLOR_TEXCOOD*)msr.pData;
	vertex[0] = { {-0.5f,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[1] = { { 0.5f,  0.5f, 0.0f}, {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[2] = { {-0.5f, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
	vertex[3] = { { 0.5f, -0.5f, 0.0f},  {m_Color},{m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
	m_DeviceContext->Unmap(m_VertexBuffer.Get(), 0);

	// �X�V�t���O��߂�
	m_IsUpdate = false;
}

bool ChangeableBillBoardComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init GetShader�����s");
		return false;
	}

	// ���b�V���쐬
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init CreateMesh�����s");
		return false;
	}

	//�@�V�[�������_�[�}�l�[�W���[�擾
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init currentScene��nullptr");
		return false;
	}

	m_SceneRenderManager = currentScene->GetSceneRenderManager();
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::Init m_SceneRenderManager��nullptr");
		return false;
	}

	return true;
}

bool ChangeableBillBoardComponent::CreateMesh()
{
	// �f�o�C�X�̎擾
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// null�`�F�b�N
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::CreateMesh device��nullptr");
		return false;
	}

	HRESULT hr;

	//���_�o�b�t�@����
	//���_���������p
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;//���_���������p
	bd.ByteWidth = sizeof(VERTEX_COLOR_TEXCOOD) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//���_���������p
	bd.MiscFlags = 0;

	hr = device->CreateBuffer(&bd, NULL, m_VertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::CreateMesh CreateBuffer�����s");
		return false;
	}

	// �l�p�`�̃C���f�b�N�X���`
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	//// �o�b�t�@���쐬
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	// �C���f�b�N�X���̒ǉ�
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;                 // �f�t�H���g�A�N�Z�X
	bufferDesc.ByteWidth = sizeof(WORD) * 6;                // �T�C�Y�̓C���f�b�N�X�̐� 6
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;         // �C���f�b�N�X�o�b�t�@���g�p
	bufferDesc.CPUAccessFlags = 0;                          // CPU�̃o�b�t�@�ւ̃A�N�Z�X����

		//// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = index;

	// �C���f�b�N�X�o�b�t�@���쐬
	hr = device->CreateBuffer(&bufferDesc, &initData, m_IndexBuffer.GetAddressOf());
	// null�`�F�b�N
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::CreateMesh CreateBuffer�����s");
		return false;
	}

	return true;
}

bool ChangeableBillBoardComponent::GetShader()
{
	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_UNLITCOLORTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_UNLITCOLORTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_UNLITCOLORTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ChangeableBillBoardComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}
