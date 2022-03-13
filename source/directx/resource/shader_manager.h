/**
 * @file shader_manager.h
 * @brief �V�F�[�_�[�Ǘ��N���X
 */

#pragma once

#include"../../function/singleton.h"

class ShaderManager : public Singleton<ShaderManager>
{
public:
	/**
	* @brief ���_�V�F�[�_�[�̖��O
	*/
	enum VertexShaderName
	{
		VS_NAME_UNLITTEXTURE,
		VS_NAME_UNLITCOLORTEXTURE,
		VS_NAME_STANDARD3D,
		VS_NAME_GRID,
		VS_NAME_PARTICLE,
		VS_NAME_GBUFFERINPUT,
		VS_NAME_DEFERREDRENDERING,
		VS_NAME_ANIMATION,
		VS_NAME_MESHFIELD,
		VS_NAME_INSTANCING,
		VS_NAME_INSTANCING_COLOR,
	};

	/**
	* @brief �s�N�Z���V�F�[�_�[�̖��O
	*/
	enum PixelShaderName
	{
		PS_NAME_UNLITTEXTURE,
		PS_NAME_UNLITCOLORTEXTURE,
		PS_NAME_STANDARD3D,
		PS_NAME_GRID,
		PS_NAME_PARTICLE,
		PS_NAME_GBUFFERINPUT,
		PS_NAME_DEFERREDRENDERING,
		PS_NAME_MESHFIELD,
		PS_NAME_INSTANCING_COLOR,
	};

	/**
	* @brief �C���v�b�g���C�A�E�g�̖��O
	*/
	enum InputLayoutName
	{
		IL_NAME_UNLITTEXTURE,
		IL_NAME_UNLITCOLORTEXTURE,
		IL_NAME_STANDARD3D,
		IL_NAME_GRID,
		IL_NAME_PARTICLE,
		IL_NAME_GBUFFERINPUT,
		IL_NAME_DEFERREDRENDERING,
		IL_NAME_ANIMATION,
		IL_NAME_MESHFIELD,
		IL_NAME_INSTANCING,
		IL_NAME_INSTANCING_COLOR,
	};


	/**
	* @brief �R���s���[�g�V�F�[�_�[�̖��O
	*/
	enum ComputeShaderName
	{
		CS_NAME_PARTICLE_INIT,
		CS_NAME_PARTICLE_CIRCLE_EMITTER,
		CS_NAME_PARTICLE_RECT_EMITTER,
		CS_NAME_PARTICLE_DEMO_EMITTER,
		CS_NAME_PARTICLE_UPDATE,
	};
private:
	//! �f�o�C�X
	ID3D11Device* m_Device;

	//! ���_�V�F�[�_�[�̃C���^�[�t�F�[�X
	std::unordered_map<VertexShaderName, ID3D11VertexShader*> m_VertexShaders;

	//! �s�N�Z���V�F�[�_�[�̃C���^�[�t�F�[�X
	std::unordered_map<PixelShaderName, ID3D11PixelShader*> m_PixelShaders;

	//! �C���v�b�g���C�A�E�g�̃C���^�[�t�F�[�X
	std::unordered_map<InputLayoutName, ID3D11InputLayout*> m_InputLayouts;

	//! �R���s���[�g�V�F�[�_�[�̃C���^�[�t�F�[�X
	std::unordered_map<ComputeShaderName, ID3D11ComputeShader*> m_ComputeShaders;
private:
	friend class Singleton<ShaderManager>;

	ShaderManager() = default;
	~ShaderManager() = default;

public:
	/**
	* @brief ������
	*/
	void Init();

	/**
	* @brief �I������
	*/
	void Uninit();

	// �Q�b�^�[
	ID3D11VertexShader* GetVertexShader(VertexShaderName vertexShaderName) const;
	ID3D11PixelShader* GetPixelShader(PixelShaderName pixelShaderName) const;
	ID3D11InputLayout* GetInputLayout(InputLayoutName inputLayout) const;
	ID3D11ComputeShader* GetComputeShader(ComputeShaderName computeShaderName) const;

private:

	bool CreateVertexShaderInputLayout(const char* FileName, VertexShaderName vsName, InputLayoutName ilName, const D3D11_INPUT_ELEMENT_DESC* inputLayoutElementDesc, int elementNum);

	bool CreatePixelShader(const char* FileName, PixelShaderName psName);

	bool CreateComputeShader(const char* FileName, ComputeShaderName csName);

	bool LoadShaderFile(const char* FileName, long* size, unsigned char** data);

	/**
	* @brief �V�F�[�_�[�쐬
	*/
	void CreateShader();

	/**
	* @brief �V�F�[�_�[�A�����[�h
	*/
	void UnloadShader();
};