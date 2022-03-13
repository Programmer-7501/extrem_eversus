/**
* @file renderer.h
* @brief �`��@�\
* @details DirectX11�̋@�\�g�p
*/

#pragma once

#include"../function/singleton.h"
#include"render_data/vertex_structure.h"
#include"render_data/cbuffer_data.h"
#include"render_data/sampler_data.h"

/**
* @brief �`��@�\�N���X
* @details DirectX11�̋@�\�g�p
*/
class Renderer final : public Singleton<Renderer>
{
public:
	/**
	* @brief �u�����h�X�e�[�g�̖��O
	*/
	enum BlendStateName
	{
		BLENDSTATE_DEFAULT,
		BLENDSTATE_INVISIBLEADD,
		BLENDSTATE_MAX
	};

	/**
	* @brief �f�v�X�X�e���V���X�e�[�g�̖��O
	*/
	enum DepthStencilStateName
	{
		//�[�x�l���Q��
		DEPTHSTENCILSTATE_STANDARD,
		// �[�x�l���������܂Ȃ�
		DEPTHSTENCILSTATE_NOWRITE,
		// �[�x�l�𖳌��ɂ���
		DEPTHSTENCILSTATE_NOWRITE_REFERENCE,
		DEPTHSTENCILSTATE_MAX,
	};
private:
	/**
	* @brief �����_�[�^�[�Q�b�g�̖��O
	*/
	enum RenderTargetName
	{
		RENDERTARGET_BACKBUFFER,
		RENDERTARGET_ALBEDCOLOR,
		RENDERTARGET_NORMAL,
		RENDERTARGET_SPECULAR,
		RENDERTARGET_MAIN,
		RENDERTARGET_DEPTH,
		RENDERTARGET_MAX,
	};

	/**
	* @brief �f�v�X�X�e���V���̖��O
	*/
	enum DepthStencilName
	{
		DEPTHSTENCIL_BACKBUFFER,
		DEPTHSTENCIL_SHADOWMAP,
		DEPTHSTENCIL_MAIN,
		DEPTHSTENCIL_MAX,
	};

private:
	//! �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr;
	//! �f�o�C�X�R���e�L�X�g
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
	//! �X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState = nullptr;

	//! �f�v�X�X�e���V���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthState[DEPTHSTENCILSTATE_MAX] = {};

	//! �u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState[BLENDSTATE_MAX] = {};

	//! �T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState[SamplerData::MAXSAMPLERNUM] = {};

	//! �R���X�^���g�o�b�t�@

	//! ���[���h�}�g���N�X�R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_WorldMatrixConstantBuffer = nullptr;
	//! �`��f�[�^�R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawDataConstantBuffer = nullptr;
	//! �f�B���N�V���i�����C�g�R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DirectionalLightConstantBuffer = nullptr;
	//! �|�C���g���C�g�R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_PointLightConstantBuffer = nullptr;
	//! �}�e���A���R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_MaterialConstantBuffer = nullptr;
	//! ���C�g�r���[�v���W�F�N�V�����R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightViewProjectionConstantBuffer = nullptr;
	//! �{�[���R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_BonesConstantBuffer = nullptr;

	//! �����_�[�^�[�Q�b�g�e�N�X�`��
	Microsoft::WRL::ComPtr <ID3D11Texture2D> m_RenderTarget_LayerBuffer[RENDERTARGET_MAX] = {};
	//! �����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_RenderTargetView[RENDERTARGET_MAX] = {};
	//! �����_�[�^�[�Q�b�g�V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_RenderTarget_ShaderResourceView[RENDERTARGET_MAX] = {};

	//! �f�v�X�X�e���V���e�N�X�`��
	Microsoft::WRL::ComPtr <ID3D11Texture2D> m_DepthStencil_LayerBuffer[DEPTHSTENCIL_MAX] = {};
	//! �f�v�X�X�e���V���r���[
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView>	m_DepthStencilView[DEPTHSTENCIL_MAX] = {};
	//! �f�v�X�X�e���V���V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_DepthStencil_ShaderResourceView[DEPTHSTENCIL_MAX] = {};

	//! �t�H���[�h�����_�����O
	class DeferredRendering* m_DeferredRendering = nullptr;
	//! �����_�����O���ʕ\��
	class OutPutRenderingResult* m_OutPutRenderingResult = nullptr;

	//! �J�����f�[�^�\����
	CBufferData::DrawData m_DrawData = {};
private:
	friend class Singleton<Renderer>;

	Renderer() = default;
	~Renderer() = default;

public:
	/**
	* @brief ������
	* @return bool �������������������ۂ�
	*/
	bool Init();

	/**
	* @brief �����_�����O�p�C�v���C���쐬
	* @return bool �������������������ۂ�
	* @details �V�F�[�_�[��������ɌĂяo��
	*/
	bool CreateRenderingPipline();

	/**
	* @brief �I������
	*/
	void Uninit();

	/**
	* @brief �`��J�n
	*/
	void DrawBegin();

	/**
	* @brief �`��I��
	*/
	void DrawEnd();

	/**
	* @brief ShadowInput�J�n
	*/
	void ShadowInputStart();

	/**
	* @brief ShadowInput�I��
	*/
	void ShadowInputEnd();

	/**
	* @brief GBufferInput�J�n
	*/
	void GBufferInputStart();

	/**
	* @brief GBufferInput�I��
	*/
	void GBufferInputEnd();

	/**
	* @brief �f�B�t�@�[�h�����_�����O���s
	*/
	void ExcuteDeferredRendering();

	/**
	* @brief �t�H���[�h�����_�����O�J�n
	*/
	void ForwardRenderingStart();

	/**
	* @brief �t�H���[�h�����_�����O�I��
	*/
	void ForwardRenderingEnd();

	/**
	* @brief �����_�����O���ʕ`��
	*/
	void ExcuteOutPutRenderingResult();

	/**
	* @brief 2D�`��ɃZ�b�g
	*/
	void SetWorldViewProjection2D();

	/**
	* @brief ���[���h�s��Z�b�g
	* @param[in] const Conv_XM::Matrix4x4f&(worldMatrix) ���[���h�s��
	*/
	void SetWorldMatrix(const Conv_XM::Matrix4x4f& worldMatrix);

	/**
	* @brief �`��f�[�^�Z�b�g
	* @param[in] const CBufferData::CameraData&(cameraData) �J�����f�[�^
	*/
	void SetDrawData(const CBufferData::DrawData& cameraData);

	/**
	* @brief �r���[&�v���W�F�N�V�����s��Z�b�g
	* @param[in] const Conv_XM::Matrix4x4f&(viewMatrix) �r���[�s��
		* @param[in] const Conv_XM::Matrix4x4f&(projectionMatrix) �v���W�F�N�V�����s��
	*/
	void SetViewProjectionMatrix(const Conv_XM::Matrix4x4f& viewMatrix, const Conv_XM::Matrix4x4f& projectionMatrix);

	/**
	* @brief �J�����̃r���[�v���W�F�N�V�����t�s��Z�b�g
	* @param[in] const Conv_XM::Matrix4x4f&(viewMatrix) �r���[�v���W�F�N�V�����s��Z�b�g
	*/
	void Set3DCameraViewProjectionInverse(const Conv_XM::Matrix4x4f& viewProjectionInverse);

	/**
	* @brief �J�����̃|�W�V�����Z�b�g
	* @param[in] const Conv_XM::Vector4f&(cameraPosition) �J�����̃|�W�V�����Z�b�g�Z�b�g
	*/
	void SetCameraPositon(const Conv_XM::Vector4f& cameraPosition);


	/**
	* @brief �����Z�b�g
	* @param[in] float(randomNum) ����
	*/
	void SetRandomNumber(float randomNum);

	/**
	* @brief �f�B���N�V���i�����C�g�Z�b�g
	* @param[in] const CBufferData::DirectionalLight&(directionalLight) �f�B���N�V���i�����C�g
	*/
	void SetDirectionalLight(const CBufferData::DirectionalLight& directionalLight);

	/**
	* @brief �|�C���g���C�g�Z�b�g
	* @param[in] const CBufferData::PointLight&(pointLight) �|�C���g���C�g�Z�b�g
	*/
	void SetPointLight(const CBufferData::PointLight& pointLight);

	/**
	* @brief �}�e���A���Z�b�g
	* @param[in] const CBufferData::Material&(material) �}�e���A���Z�b�g
	*/
	void SetMaterial(const CBufferData::Material& material);

	/**
	* @brief ���C�g�r���[�v���W�F�N�V�����Z�b�g
	* @param[in] const Conv_XM::Matrix4x4f&(lightViewProjection) ���C�g�r���[�v���W�F�N�V����
	*/
	void SetLightViewProjection(const Conv_XM::Matrix4x4f& lightViewProjection);

	/**
	* @brief�{�[���Z�b�g
	* @param[in] const CBufferData::BONES&(bones) �{�[���f�[�^
	*/
	void SetBones(const CBufferData::BONES& bones);

	/**
	* @brief �u�����h�X�e�[�g�Z�b�g
	* @param[in] BlendStateName(blendStateName) �u�����h�X�e�[�g
	*/
	void SetBlendState(BlendStateName blendStateName);

	/**
	* @brief �f�v�X�X�e���V���X�e�[�g�Z�b�g
	* @param[in] BlendStateName(blendStateName) �f�v�X�X�e���V���X�e�[�g
	*/
	void SetDepthStencilState(DepthStencilStateName depthStencilStateName);

	/**
	* @brief �r���[�|�[�g�o�^
	* @param[in] D3D11_VIEWPORT(viewPort) �r���[�|�[�g�\����
	*/
	void SetViewPort(D3D11_VIEWPORT viewPort);

	/**
	* @brief �f�o�C�X�̎擾
	*/
	inline ID3D11Device* GetDevice(void) const { return m_Device.Get(); }

	/**
	* @brief �f�o�C�X�R���e�L�X�g�̎擾
	*/
	inline ID3D11DeviceContext* GetDeviceContext(void) const { return m_DeviceContext.Get(); }

	/**
	* @brief �X���b�v�`�F�[���̎擾
	*/
	inline IDXGISwapChain* GetSwapChain() const { return m_SwapChain.Get(); }

	/**
	* @brief �f�B�t�@�[�h�����_�����O�N���X�̎擾
	*/
	inline class DeferredRendering* GetDeferredRendering() const { return m_DeferredRendering; }

	/**
	* @brief �����_�����O���ʕ\���N���X�̎擾
	*/
	inline class OutPutRenderingResult* GetOutPutRenderingResult() const { return m_OutPutRenderingResult; }
private:
	/**
	* @brief �X���b�v�`�F�[���쐬
	*/
	bool CreateDeviceSwapChain();

	/**
	* @brief �r���[�|�[�g�o�^
	*/
	void RegisterViewport();

	/**
	* @brief ���X�^���C�U�[�쐬
	*/
	bool CreateRasterizer();

	/**
	* @brief �����_�[�^�[�Q�b�g�쐬
	*/
	bool CreateRenderTarget();

	/**
	* @brief �f�v�X�X�e���V���쐬
	*/
	bool CreateDepthSetncil();

	/**
	* @brief �f�v�X�X�e���V���X�e�[�g�쐬
	*/
	bool CreateDepthStencilState();

	/**
	* @brief �u�����h�X�e�[�g�쐬
	*/
	bool CreatBlendState();

	/**
	* @brief �T���v���[�X�e�[�g�쐬
	*/
	bool CreateSamplerState();

	/**
	* @brief �R���X�^���g�o�b�t�@�쐬
	*/
	bool CreateConstantBuffer();

	/**
	* @brief �����_�����O�p�C�v���C���j��
	*/
	void UnloadRenderingPipline();
};
