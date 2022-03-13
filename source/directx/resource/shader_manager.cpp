
#include"../../system/main.h"
#include "shader_manager.h"
#include"../renderer.h"
#include <io.h>

namespace ShaderFileName
{
	static const char* k_UnlitTexture_VS = "asset/shader/unlit_texture_vs.cso";
	static const char* k_UnlitTexture_PS = "asset/shader/unlit_texture_ps.cso";
	static const char* k_UnlitColorTexture_VS = "asset/shader/unlit_color_texture_vs.cso";
	static const char* k_UnlitColorTexture_PS = "asset/shader/unlit_color_texture_ps.cso";
	static const char* k_Standard3D_VS = "asset/shader/standard3d_vs.cso";
	static const char* k_Standard3D_PS = "asset/shader/standard3d_ps.cso";
	static const char* k_Grid_VS = "asset/shader/grid_vs.cso";
	static const char* k_Grid_PS = "asset/shader/grid_ps.cso";

	static const char* k_GBufferInput_VS = "asset/shader/gbuffer_input_vs.cso";
	static const char* k_GBufferInput_PS = "asset/shader/gbuffer_input_ps.cso";

	static const char* k_DeferredRendering_VS = "asset/shader/deferred_rendering_vs.cso";
	static const char* k_DeferredRendering_PS = "asset/shader/deferred_rendering_ps.cso";

	static const char* k_Animation_VS = "asset/shader/animation_vs.cso";

	static const char* k_MeshField_VS = "asset/shader/meshfield_vs.cso";
	static const char* k_MeshField_PS = "asset/shader/meshfield_ps.cso";

	static const char* k_Instancing_VS = "asset/shader/instancing_vs.cso";

	static const char* k_Instancing_Color_VS = "asset/shader/instancing_color_vs.cso";
	static const char* k_Instancing_Color_PS = "asset/shader/instancing_color_ps.cso";

	static const char* k_Particle_Init_CS = "asset/shader/particle_init_cs.cso";
	static const char* k_Particle_Circle_Emitter_CS = "asset/shader/particle_circle_emitter_cs.cso";
	static const char* k_Particle_Rect_Emitter_CS = "asset/shader/particle_rect_emitter_cs.cso";
	static const char* k_Particle_Demo_Emitter_CS = "asset/shader/particle_demo_emitter_cs.cso";
	static const char* k_Particle_Update_CS = "asset/shader/particle_update_cs.cso";
	static const char* k_Particle_VS = "asset/shader/particle_render_vs.cso";
	static const char* k_Particle_PS = "asset/shader/particle_render_ps.cso";
}

void ShaderManager::Init()
{
	m_Device = nullptr;

	// �f�o�C�X�擾
	m_Device = Renderer::GetInstance().GetDevice();
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("ShaderManager::Init m_Device��nullptr");
		return;
	}

	// �V�F�[�_�[�̍쐬
	CreateShader();
}

void ShaderManager::Uninit()
{
	// �V�F�[�_�[�A�����[�h
	UnloadShader();

	m_Device = nullptr;
}

ID3D11VertexShader * ShaderManager::GetVertexShader(VertexShaderName vertexShaderName) const
{
	const auto& iter = m_VertexShaders.find(vertexShaderName);
	// ����������Ԃ�
	if (iter != m_VertexShaders.end())
	{
		return iter->second;
	}

	// ������Ȃ��Ȃ�nullptr
	return nullptr;
}

ID3D11PixelShader * ShaderManager::GetPixelShader(PixelShaderName pixelShaderName) const
{
	const auto& iter = m_PixelShaders.find(pixelShaderName);
	// ����������Ԃ�
	if (iter != m_PixelShaders.end())
	{
		return iter->second;
	}

	// ������Ȃ��Ȃ�nullptr
	return nullptr;
}

ID3D11InputLayout * ShaderManager::GetInputLayout(InputLayoutName inputLayout) const
{
	const auto& iter = m_InputLayouts.find(inputLayout);
	// ����������Ԃ�
	if (iter != m_InputLayouts.end())
	{
		return iter->second;
	}

	// ������Ȃ��Ȃ�nullptr
	return nullptr;
}

ID3D11ComputeShader * ShaderManager::GetComputeShader(ComputeShaderName computeShaderName) const
{
	const auto& iter = m_ComputeShaders.find(computeShaderName);
	// ����������Ԃ�
	if (iter != m_ComputeShaders.end())
	{
		return iter->second;
	}

	// ������Ȃ��Ȃ�nullptr
	return nullptr;
}

bool ShaderManager::CreateVertexShaderInputLayout(const char * FileName, VertexShaderName vsName, InputLayoutName ilName, const D3D11_INPUT_ELEMENT_DESC* inputLayoutElementDesc, int elementNum)
{
	long size = 0;
	unsigned char* data = nullptr;

	// �t�@�C���ǂݍ���
	if (LoadShaderFile(FileName,&size, &data) == false)
	{
		// �f�[�^�j��
		if (data)
		{
			delete[] data;
			data = nullptr;
		}
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout �t�@�C���ǂݍ��ݎ��s");
		return false;
	}

	// null�`�F�b�N
	if (m_Device == nullptr)
	{
		// �f�[�^�j��
		if (data)
		{
			delete[] data;
			data = nullptr;
		}
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout m_Device��nullptr");
		return false;
	}

	// ���_�V�F�[�_�[�쐬
	HRESULT hr;
	ID3D11VertexShader* vertexShader = nullptr;

	hr = m_Device->CreateVertexShader(data, size, nullptr, &vertexShader);
	if (FAILED(hr))
	{
		// �f�[�^�j��
		if (data)
		{
			delete[] data;
			data = nullptr;
		}
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return false;
	}
	// �o�^����
	m_VertexShaders.emplace(vsName, vertexShader);

	// �C���v�b�g���C�A�E�g�쐬
	ID3D11InputLayout* inputLayout = nullptr;

	hr = m_Device->CreateInputLayout(inputLayoutElementDesc,
		elementNum,
		data,
		size,
		&inputLayout);

	// �f�[�^�j��
	if (data)
	{
		delete[] data;
		data = nullptr;
	}

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateInputLayout���s");
		return false;
	}

	// �C���v�b�g���C�A�E�g��o�^
	m_InputLayouts.emplace(ilName, inputLayout);


	return true;
}

bool ShaderManager::CreatePixelShader(const char * FileName, PixelShaderName psName)
{
	long size = 0;
	unsigned char* data = nullptr;

	// �t�@�C���ǂݍ���
	if (LoadShaderFile(FileName, &size, &data) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreatePixelShader �t�@�C���ǂݍ��ݎ��s");
		return false;
	}

	// null�`�F�b�N
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreatePixelShader m_Device��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	HRESULT hr;
	ID3D11PixelShader* pixelShader = nullptr;

	hr = m_Device->CreatePixelShader(data, size, nullptr, &pixelShader);

	// �f�[�^�j��
	if (data)
	{
		delete[] data;
		data = nullptr;
	}

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ShaderManager::CreatePixelShader CreatePixelShader���s");
		return false;
	}
	// �o�^����
	m_PixelShaders.emplace(psName, pixelShader);

	return true;
}

bool ShaderManager::CreateComputeShader(const char * FileName, ComputeShaderName csName)
{
	long size = 0;
	unsigned char* data = nullptr;

	// �t�@�C���ǂݍ���
	if (LoadShaderFile(FileName, &size, &data) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateComputeShader �t�@�C���ǂݍ��ݎ��s");
		return false;
	}

	// null�`�F�b�N
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateComputeShader m_Device��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	HRESULT hr;
	ID3D11ComputeShader* computeShader = nullptr;

	hr = m_Device->CreateComputeShader(data, size, nullptr, &computeShader);

	// �f�[�^�j��
	if (data)
	{
		delete[] data;
		data = nullptr;
	}

	if (FAILED(hr))
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateComputeShader CreateComputeShader���s");
		return false;
	}
	// �o�^����
	m_ComputeShaders.emplace(csName, computeShader);

	return true;
}

bool ShaderManager::LoadShaderFile(const char * FileName, long* size, unsigned char** data)
{
	FILE* file = nullptr;

	file = fopen(FileName, "rb");

	if (file == nullptr)
	{
		Logger::GetInstance().SetLog("�t�@�C���I�[�v�����s");
		return false;
	}

	// �T�C�Y�擾
	*size = _filelength(_fileno(file));
	if (*size == 0)
	{
		Logger::GetInstance().SetLog("�t�@�C���̒��g����");
		return false;
	}

	if (*data)
	{
		delete[] *data;
		*data = nullptr;
	}

	*data = new unsigned char[*size];

	// �t�@�C����ǂݍ���
	fread(*data, *size, 1, file);

	// �t�@�C�������
	fclose(file);

	return true;
}

void ShaderManager::CreateShader()
{
	if (m_Device == nullptr)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader m_Device��nullptr");
		return;
	}

	//---------------------------UnlitTexture--------------------------------//

		//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC unlitTextureLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT unlitTextureElements = ARRAYSIZE(unlitTextureLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_UnlitTexture_VS, VS_NAME_UNLITTEXTURE, IL_NAME_UNLITTEXTURE, unlitTextureLayout, unlitTextureElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_UnlitTexture_PS, PS_NAME_UNLITTEXTURE) == false)
	{

		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}

	//---------------------------UnlitColorTexture--------------------------------//

	//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC unlitColorTextureLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT unlitColorTextureElements = ARRAYSIZE(unlitColorTextureLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_UnlitColorTexture_VS, VS_NAME_UNLITCOLORTEXTURE, IL_NAME_UNLITCOLORTEXTURE, unlitColorTextureLayout, unlitColorTextureElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_UnlitColorTexture_PS, PS_NAME_UNLITCOLORTEXTURE) == false)
	{

		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}

	//---------------------------standard3d--------------------------------//

	//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC standard3dLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT standard3dElements = ARRAYSIZE(standard3dLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_Standard3D_VS, VS_NAME_STANDARD3D, IL_NAME_STANDARD3D, standard3dLayout, standard3dElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_Standard3D_PS, PS_NAME_STANDARD3D) == false)
	{

		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}

	//---------------------------grid--------------------------------//

	//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC colorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT colorElements = ARRAYSIZE(colorLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_Grid_VS, VS_NAME_GRID, IL_NAME_GRID, colorLayout, colorElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_Grid_PS, PS_NAME_GRID) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}

	//---------------------------particle--------------------------------//

	//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC particleLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT particleElements = ARRAYSIZE(particleLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_Particle_VS, VS_NAME_PARTICLE, IL_NAME_PARTICLE, particleLayout, particleElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_Particle_PS, PS_NAME_PARTICLE) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}


	//---------------------------gbufferinput--------------------------------//

	//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC gbufferInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT gbufferInputElements = ARRAYSIZE(gbufferInputLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_GBufferInput_VS, VS_NAME_GBUFFERINPUT, IL_NAME_GBUFFERINPUT, gbufferInputLayout, gbufferInputElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_GBufferInput_PS, PS_NAME_GBUFFERINPUT) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}

	//---------------------------deferredrendering--------------------------------//

	//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC deferredRenderingInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT deferredRenderingInputElements = ARRAYSIZE(deferredRenderingInputLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_DeferredRendering_VS, VS_NAME_DEFERREDRENDERING, IL_NAME_DEFERREDRENDERING, deferredRenderingInputLayout, deferredRenderingInputElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_DeferredRendering_PS, PS_NAME_DEFERREDRENDERING) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}


	//---------------------------animation--------------------------------//

	//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC animationInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_UINT, 0, 4 * 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_UINT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT animationInputElements = ARRAYSIZE(animationInputLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_Animation_VS, VS_NAME_ANIMATION, IL_NAME_ANIMATION, animationInputLayout, animationInputElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	//---------------------------meshfield--------------------------------//

		//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC meshfieldLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT meshfieldElements = ARRAYSIZE(meshfieldLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_MeshField_VS, VS_NAME_MESHFIELD, IL_NAME_MESHFIELD, meshfieldLayout, meshfieldElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

		// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_MeshField_PS, PS_NAME_MESHFIELD) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}


	//---------------------------instancing--------------------------------//

		//InputLayout�쐬
	D3D11_INPUT_ELEMENT_DESC instancingLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATRIX",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX",   1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX",   2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX",   3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	UINT instancingElements = ARRAYSIZE(instancingLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_Instancing_VS, VS_NAME_INSTANCING, IL_NAME_INSTANCING, instancingLayout, instancingElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}


	//---------------------------instancing_color--------------------------------//


	D3D11_INPUT_ELEMENT_DESC instancingColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// ���̓A�Z���u���ɃW�I���g�������p�̍s���ǉ��ݒ肷��
		//���̓Z���}�e�B�N�X�̃C���f�b�N�X(matrix������Ƃ��Ɏl�̔z��ɂ���
		//�E�̈����ŃC���X�^���X�f�[�^�������A�C���X�^���X�p�ŗv�f���J��グ��
		{ "MATRIX",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX",   1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX",   2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX",   3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16 * 4, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	UINT instancingColorElements = ARRAYSIZE(instancingColorLayout);

	// ���_�V�F�[�_�[,�C���v�b�g���C�A�E�g�쐬
	if (CreateVertexShaderInputLayout(ShaderFileName::k_Instancing_Color_VS, VS_NAME_INSTANCING_COLOR, IL_NAME_INSTANCING_COLOR, instancingColorLayout, instancingColorElements) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateVertexShaderInputLayout CreateVertexShaderInputLayout���s");
		return;
	}

	// �s�N�Z���V�F�[�_�[�쐬
	if (CreatePixelShader(ShaderFileName::k_Instancing_Color_PS, PS_NAME_INSTANCING_COLOR) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreatePixelShader���s");
		return;
	}


	// �R���s���[�g�V�F�[�_�[�쐬
	if (CreateComputeShader(ShaderFileName::k_Particle_Init_CS, CS_NAME_PARTICLE_INIT) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreateComputeShader���s");
		return;
	}

	// �R���s���[�g�V�F�[�_�[�쐬
	if (CreateComputeShader(ShaderFileName::k_Particle_Circle_Emitter_CS, CS_NAME_PARTICLE_CIRCLE_EMITTER) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreateComputeShader���s");
		return;
	}

	// �R���s���[�g�V�F�[�_�[�쐬
	if (CreateComputeShader(ShaderFileName::k_Particle_Rect_Emitter_CS, CS_NAME_PARTICLE_RECT_EMITTER) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreateComputeShader���s");
		return;
	}

	// �R���s���[�g�V�F�[�_�[�쐬
	if (CreateComputeShader(ShaderFileName::k_Particle_Demo_Emitter_CS, CS_NAME_PARTICLE_DEMO_EMITTER) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreateComputeShader���s");
		return;
	}

	// �R���s���[�g�V�F�[�_�[�쐬
	if (CreateComputeShader(ShaderFileName::k_Particle_Update_CS, CS_NAME_PARTICLE_UPDATE) == false)
	{
		Logger::GetInstance().SetLog("ShaderManager::CreateShader CreateComputeShader���s");
		return;
	}
}

void ShaderManager::UnloadShader()
{
	// �s�N�Z���V�F�[�_�[�A�����[�h
	for (auto&& pixelShader : m_PixelShaders)
	{
		if (pixelShader.second)
		{
			pixelShader.second->Release();
			pixelShader.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("ShaderManager::UnloadShader pixelShader.second��nullptr");
		}
	}
	m_PixelShaders.clear();

	// �C���v�b�g���C�A�E�g�A�����[�h
	for (auto&& inputLayout : m_InputLayouts)
	{
		if (inputLayout.second)
		{
			inputLayout.second->Release();
			inputLayout.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("ShaderManager::UnloadShader inputLayout.second��nullptr");
		}
	}
	m_InputLayouts.clear();

	// ���_�V�F�[�_�[�A�����[�h
	for (auto&& vertexShader : m_VertexShaders)
	{
		if (vertexShader.second)
		{
			vertexShader.second->Release();
			vertexShader.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("ShaderManager::UnloadShader vertexShader.second��nullptr");
		}
	}
	m_VertexShaders.clear();

	// �R���s���[�g�V�F�[�_�[�A�����[�h
	for (auto&& computeShader : m_ComputeShaders)
	{
		if (computeShader.second)
		{
			computeShader.second->Release();
			computeShader.second = nullptr;
		}
		else
		{
			Logger::GetInstance().SetLog("ShaderManager::UnloadShader computeShader.second��nullptr");
		}
	}
	m_ComputeShaders.clear();
}
