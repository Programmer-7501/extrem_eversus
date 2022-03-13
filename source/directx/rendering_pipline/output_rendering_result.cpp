
#include"../../system/main.h"
#include "../rendering_pipline/output_rendering_result.h"
#include"../renderer.h"
#include"../resource/shader_manager.h"
#include"../render_data/srv_data.h"

OutPutRenderingResult::OutPutRenderingResult(
	ID3D11ShaderResourceView * renderingResultSRV,
	float clearColor_R,
	float clearColor_B,
	float clearColor_G,
	float clearColor_A
)
	: m_RenderingResultSRV(renderingResultSRV)
	, m_Color{ clearColor_R ,clearColor_B ,clearColor_G ,clearColor_A }
{
}

OutPutRenderingResult::~OutPutRenderingResult()
{
}

bool OutPutRenderingResult::Init()
{
	Renderer& renderer = Renderer::GetInstance();

	m_DeviceContext = renderer.GetDeviceContext();

	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::Init m_DeviceContext��nullptr");
		return false;
	}

	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("DeferredRendering::Init GetShader�����s");
		return false;
	}

	if (CreatePolygon() == false)
	{
		Logger::GetInstance().SetLog("DeferredRendering::Init CreatePolygon�����s");
		return false;
	}

	return true;
}

void OutPutRenderingResult::Draw() const
{
	Renderer& renderer = Renderer::GetInstance();

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	renderer.SetWorldViewProjection2D();

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_TEXCOOD);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �\������C���f�b�N�X�o�b�t�@��I��
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//�e�N�X�`���ݒ�
	m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &m_RenderingResultSRV);

	// �g�p����v���~�e�B�u�^�C�v��ݒ�
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�`��
	m_DeviceContext->DrawIndexed(6, 0, 0);
}

bool OutPutRenderingResult::GetShader()
{
	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = ShaderManager::GetInstance().GetVertexShader(ShaderManager::VS_NAME_UNLITTEXTURE);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = ShaderManager::GetInstance().GetPixelShader(ShaderManager::PS_NAME_UNLITTEXTURE);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = ShaderManager::GetInstance().GetInputLayout(ShaderManager::IL_NAME_UNLITTEXTURE);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}

bool OutPutRenderingResult::CreatePolygon()
{
	Renderer& renderer = Renderer::GetInstance();
	HRESULT hr;

	// �f�o�C�X�擾
	ID3D11Device* device = renderer.GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("DeferredRendering::CreatePolygon device��nullptr");
		return false;
	}

	VERTEX_TEXCOOD vertex[4];

	float width = static_cast<float>(SystemData::k_ScreenWidth);
	float height = static_cast<float>(SystemData::k_ScreenHeight);
	vertex[0] = { {0.0f		, 0.0f	, 0.0f},{0.0f, 0.0f} };
	vertex[1] = { {width	, 0.0f	, 0.0f},{1.0f, 0.0f} };
	vertex[2] = { {0.0f		, height, 0.0f},{0.0f, 1.0f} };
	vertex[3] = { {width	, height, 0.0f},{1.0f, 1.0f} };

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_TEXCOOD) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = vertex;

	hr = device->CreateBuffer(&vertexBufferDesc, &vertexInitData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("DeferredRendering::CreatePolygon CreateBuffer�����s");
		return false;
	}

	// �l�p�`�̃C���f�b�N�X���`
	WORD index[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	//// �o�b�t�@���쐬
	D3D11_BUFFER_DESC indexBufferDesc = {};
	// �C���f�b�N�X���̒ǉ�
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	//// ���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = index;

	// �C���f�b�N�X�o�b�t�@���쐬
	hr = device->CreateBuffer(&indexBufferDesc, &indexInitData, m_IndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("DeferredRendering::CreatePolygon CreateBuffer�����s");
		return false;
	}

	return true;
}
