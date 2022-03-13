
#include"../../../system/main.h"
#include "lockon_cursol_billboard_component.h"
#include"../../../directx/renderer.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../scene_function/scene_render_manager.h"
#include"../../scene/scene.h"
#include"../../scene/scene_manager.h"
#include"../cameracomponent/camera_component.h"

LockonCursolBillboardComponent::LockonCursolBillboardComponent(Actor * owner)
	:MeshComponent(owner)
	, m_DeviceContext(nullptr)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_Position(0.0f,0.0f,0.0f)
	, m_Size(1.0f,1.0f)
	, m_SceneRenderManager(nullptr)
	, m_LockOnState(LOCKONSTATE_GREEN)
{
	m_RenderingWay = RENDERING_FORWARD;
}

void LockonCursolBillboardComponent::Draw() const
{
	if (m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr || m_VertexBuffer == nullptr || m_IndexBuffer == nullptr || m_ShaderResourceView == nullptr)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::Draw nullptr");
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
	class CameraComponent* cameraComponent = m_SceneRenderManager->GetMainCameraComponent();
	if (cameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::Draw cameraComponent��nullptr");
		return;
	}
	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f matView = cameraComponent->GetViewMatrix();

	//�r���[�s��̋t�s��łȂ������s�ړ�������0
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	matView._44 = 1.0f;

	DirectX::XMVECTOR determinant;
	Conv_XM::Matrix4x4f matInvView = DirectX::XMMatrixInverse(&determinant, matView);

	Conv_XM::Matrix4x4f mtxWorld, mtxScaling, mtxTranslate;
	mtxScaling = DirectX::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f);
	mtxTranslate = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	mtxWorld = (DirectX::XMMATRIX)mtxScaling * (DirectX::XMMATRIX)matInvView * (DirectX::XMMATRIX)mtxTranslate;

	renderer.SetWorldMatrix(mtxWorld);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �\������C���f�b�N�X�o�b�t�@��I��
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//�e�N�X�`���ݒ�
	m_DeviceContext->PSSetShaderResources(0, 1, &m_ShaderResourceView[m_LockOnState]);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �[�x�l�𖳌��ɂ���
	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_NOWRITE_REFERENCE);

	//�`��
	m_DeviceContext->DrawIndexed(6, 0, 0);

	// �[�x�l��߂�
	renderer.SetDepthStencilState(Renderer::DEPTHSTENCILSTATE_STANDARD);
}

bool LockonCursolBillboardComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::LoadData GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::LoadData GetShader�����s");
		return false;
	}

	// ���b�V���̐���
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("LockonCursolBillboardComponent::LoadData CreateMesh�����s");
		return false;
	}

	return true;
}

bool LockonCursolBillboardComponent::CreateMesh()
{
	// �f�o�C�X�̎擾
	ID3D11Device* device = Renderer::GetInstance().GetDevice();
	// null�`�F�b�N
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init device��nullptr");
		return false;
	}

	HRESULT hr;

	//---------------------------------------------------------------------------------------------
	//      ���_�o�b�t�@�̍쐬
	//---------------------------------------------------------------------------------------------

	// ���_�ݒ�
	VERTEX_TEXCOOD vertex[4];
	vertex[0] = { {-0.5f, 0.5f,0.0f}, {0.0f,0.0f} };
	vertex[1] = { { 0.5f, 0.5f,0.0f}, {1.0f,0.0f} };
	vertex[2] = { {-0.5f, -0.5f ,0.0f}, {0.0f,1.0f} };
	vertex[3] = { { 0.5f, -0.5f ,0.0f}, {1.0f,1.0f} };


	// ���_�o�b�t�@����
	// ������������
	D3D11_BUFFER_DESC vertexBufferDesc = {};
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer���s");
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//      �C���f�b�N�X�o�b�t�@�̍쐬
	//---------------------------------------------------------------------------------------------

	// �l�p�`�̃C���f�b�N�X���`
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// �o�b�t�@���쐬
	D3D11_BUFFER_DESC indexBufferDesc = {};
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
		Logger::GetInstance().SetLog("Particle::Init CreateBuffer���s");
		return false;
	}

	return true;
}

bool LockonCursolBillboardComponent::GetDeviceContext()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_DeviceContext��nullptr");
		return false;
	}

	return true;
}

bool LockonCursolBillboardComponent::GetShader()
{
	// �V�F�[�_�[�}�l�[�W���[�擾
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_UNLITTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_UNLITTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_UNLITTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::GetInterface m_InputLayout��nullptr");
		return false;
	}

	//�@�V�[�������_�[�}�l�[�W���[�擾
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init currentScene��nullptr");
		return false;
	}

	m_SceneRenderManager = currentScene->GetSceneRenderManager();
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("Particle::Init m_SceneRenderManager��nullptr");
		return false;
	}

	return true;
}
