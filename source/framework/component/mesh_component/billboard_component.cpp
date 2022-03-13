#include"../../../system/main.h"
#include "billboard_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../actor/actor.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../component/cameracomponent/camera_component.h"

BillBoardComponent::BillBoardComponent(Actor * owner)
	:MeshComponent(owner)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_ShaderResourceView(nullptr)
	, m_SceneRenderManager(nullptr)
	, m_Position(0.0f,0.0f,0.0f)
	, m_Size(1.0f,1.0f)
	, m_BillBoardTextureConfig{ Conv_XM::Vector2f(0.0f,0.0f),Conv_XM::Vector2f(1.0f,1.0f) }
	, m_ShaftRock{}
{
	// ���C�g�̉e�����󂯂Ȃ����߃t�H���[�h�����_�����O
	m_RenderingWay = RENDERING_FORWARD;
}

BillBoardComponent::~BillBoardComponent()
{
}

void BillBoardComponent::Draw() const
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
		Logger::GetInstance().SetLog("BillBoardComponent::Draw nullptr");
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

	// X����]���̑ł�����
	if (m_ShaftRock[0] == true)
	{
		invView._11 = 1.0f;
		invView._12 = 0.0f;
		invView._13 = 0.0f;
	}
	if(m_ShaftRock[1] == true)
	{

		// Y����]���̑ł�����
		invView._21 = 0.0f;
		invView._22 = 1.0f;
		invView._23 = 0.0f;
	}
	if (m_ShaftRock[2] == true)
	{
		// Z����]���̑ł�����
		invView._31 = 0.0f;
		invView._32 = 0.0f;
		invView._33 = 1.0f;
	}

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxWorld = DirectX::XMMatrixIdentity();
	Conv_XM::Matrix4x4f mtxTranslate = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	Conv_XM::Matrix4x4f mtxScale = DirectX::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f);
	mtxWorld = (DirectX::XMMATRIX)mtxScale * (DirectX::XMMATRIX)invView * (DirectX::XMMATRIX)mtxTranslate;
	renderer.SetWorldMatrix(mtxWorld);

	// �}�e���A���Z�b�g
	renderer.SetMaterial(m_Material);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_TEXCOOD);
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

bool BillBoardComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init GetShader�����s");
		return false;
	}

	// ���b�V���쐬
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init CreateMesh�����s");
		return false;
	}

	//�@�V�[�������_�[�}�l�[�W���[�擾
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init currentScene��nullptr");
		return false;
	}

	m_SceneRenderManager = currentScene->GetSceneRenderManager();
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init m_SceneRenderManager��nullptr");
		return false;
	}

	return true;
}

bool BillBoardComponent::CreateMesh()
{
	// �f�o�C�X�̎擾
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// null�`�F�b�N
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init device��nullptr");
		return false;
	}

	HRESULT hr;

	// ���_�ݒ�
	VERTEX_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f,  0.5f , 0.0f}, {m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[1] = { { 0.5f,  0.5f , 0.0f}, {m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y} };
	vertex[2] = { {-0.5f, -0.5f, 0.0f},  {m_BillBoardTextureConfig.TexCut.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };
	vertex[3] = { { 0.5f, -0.5f, 0.0f},  {m_BillBoardTextureConfig.TexCut.x + m_BillBoardTextureConfig.TexCutSize.x,m_BillBoardTextureConfig.TexCut.y + m_BillBoardTextureConfig.TexCutSize.y} };


	// ���_�o�b�t�@����
	// ������������
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_TEXCOOD) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = vertex;

	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("BillBoardComponent::Init CreateBuffer���s");
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
		Logger::GetInstance().SetLog("BillBoardComponent::Init CreateBuffer���s");
		return false;
	}

	return true;
}

bool BillBoardComponent::GetDeviceContext()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetInterface m_DeviceContext��nullptr");
		return false;
	}

	return true;
}

bool BillBoardComponent::GetShader()
{
	// �V�F�[�_�[�}�l�[�W���[�̎擾
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_UNLITTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_UNLITTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_UNLITTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("BillBoardComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}
