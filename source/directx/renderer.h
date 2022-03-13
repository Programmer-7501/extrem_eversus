/**
* @file renderer.h
* @brief 描画機能
* @details DirectX11の機能使用
*/

#pragma once

#include"../function/singleton.h"
#include"render_data/vertex_structure.h"
#include"render_data/cbuffer_data.h"
#include"render_data/sampler_data.h"

/**
* @brief 描画機能クラス
* @details DirectX11の機能使用
*/
class Renderer final : public Singleton<Renderer>
{
public:
	/**
	* @brief ブレンドステートの名前
	*/
	enum BlendStateName
	{
		BLENDSTATE_DEFAULT,
		BLENDSTATE_INVISIBLEADD,
		BLENDSTATE_MAX
	};

	/**
	* @brief デプスステンシルステートの名前
	*/
	enum DepthStencilStateName
	{
		//深度値を参照
		DEPTHSTENCILSTATE_STANDARD,
		// 深度値を書き込まない
		DEPTHSTENCILSTATE_NOWRITE,
		// 深度値を無効にする
		DEPTHSTENCILSTATE_NOWRITE_REFERENCE,
		DEPTHSTENCILSTATE_MAX,
	};
private:
	/**
	* @brief レンダーターゲットの名前
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
	* @brief デプスステンシルの名前
	*/
	enum DepthStencilName
	{
		DEPTHSTENCIL_BACKBUFFER,
		DEPTHSTENCIL_SHADOWMAP,
		DEPTHSTENCIL_MAIN,
		DEPTHSTENCIL_MAX,
	};

private:
	//! デバイス
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr;
	//! デバイスコンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
	//! スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState = nullptr;

	//! デプスステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthState[DEPTHSTENCILSTATE_MAX] = {};

	//! ブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState[BLENDSTATE_MAX] = {};

	//! サンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState[SamplerData::MAXSAMPLERNUM] = {};

	//! コンスタントバッファ

	//! ワールドマトリクスコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_WorldMatrixConstantBuffer = nullptr;
	//! 描画データコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawDataConstantBuffer = nullptr;
	//! ディレクショナルライトコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DirectionalLightConstantBuffer = nullptr;
	//! ポイントライトコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_PointLightConstantBuffer = nullptr;
	//! マテリアルコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_MaterialConstantBuffer = nullptr;
	//! ライトビュープロジェクションコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightViewProjectionConstantBuffer = nullptr;
	//! ボーンコンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_BonesConstantBuffer = nullptr;

	//! レンダーターゲットテクスチャ
	Microsoft::WRL::ComPtr <ID3D11Texture2D> m_RenderTarget_LayerBuffer[RENDERTARGET_MAX] = {};
	//! レンダーターゲットビュー
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_RenderTargetView[RENDERTARGET_MAX] = {};
	//! レンダーターゲットシェーダーリソースビュー
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_RenderTarget_ShaderResourceView[RENDERTARGET_MAX] = {};

	//! デプスステンシルテクスチャ
	Microsoft::WRL::ComPtr <ID3D11Texture2D> m_DepthStencil_LayerBuffer[DEPTHSTENCIL_MAX] = {};
	//! デプスステンシルビュー
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView>	m_DepthStencilView[DEPTHSTENCIL_MAX] = {};
	//! デプスステンシルシェーダーリソースビュー
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_DepthStencil_ShaderResourceView[DEPTHSTENCIL_MAX] = {};

	//! フォワードレンダリング
	class DeferredRendering* m_DeferredRendering = nullptr;
	//! レンダリング結果表示
	class OutPutRenderingResult* m_OutPutRenderingResult = nullptr;

	//! カメラデータ構造体
	CBufferData::DrawData m_DrawData = {};
private:
	friend class Singleton<Renderer>;

	Renderer() = default;
	~Renderer() = default;

public:
	/**
	* @brief 初期化
	* @return bool 初期化が成功したか否か
	*/
	bool Init();

	/**
	* @brief レンダリングパイプライン作成
	* @return bool 初期化が成功したか否か
	* @details シェーダー初期化後に呼び出す
	*/
	bool CreateRenderingPipline();

	/**
	* @brief 終了処理
	*/
	void Uninit();

	/**
	* @brief 描画開始
	*/
	void DrawBegin();

	/**
	* @brief 描画終了
	*/
	void DrawEnd();

	/**
	* @brief ShadowInput開始
	*/
	void ShadowInputStart();

	/**
	* @brief ShadowInput終了
	*/
	void ShadowInputEnd();

	/**
	* @brief GBufferInput開始
	*/
	void GBufferInputStart();

	/**
	* @brief GBufferInput終了
	*/
	void GBufferInputEnd();

	/**
	* @brief ディファードレンダリング実行
	*/
	void ExcuteDeferredRendering();

	/**
	* @brief フォワードレンダリング開始
	*/
	void ForwardRenderingStart();

	/**
	* @brief フォワードレンダリング終了
	*/
	void ForwardRenderingEnd();

	/**
	* @brief レンダリング結果描画
	*/
	void ExcuteOutPutRenderingResult();

	/**
	* @brief 2D描画にセット
	*/
	void SetWorldViewProjection2D();

	/**
	* @brief ワールド行列セット
	* @param[in] const Conv_XM::Matrix4x4f&(worldMatrix) ワールド行列
	*/
	void SetWorldMatrix(const Conv_XM::Matrix4x4f& worldMatrix);

	/**
	* @brief 描画データセット
	* @param[in] const CBufferData::CameraData&(cameraData) カメラデータ
	*/
	void SetDrawData(const CBufferData::DrawData& cameraData);

	/**
	* @brief ビュー&プロジェクション行列セット
	* @param[in] const Conv_XM::Matrix4x4f&(viewMatrix) ビュー行列
		* @param[in] const Conv_XM::Matrix4x4f&(projectionMatrix) プロジェクション行列
	*/
	void SetViewProjectionMatrix(const Conv_XM::Matrix4x4f& viewMatrix, const Conv_XM::Matrix4x4f& projectionMatrix);

	/**
	* @brief カメラのビュープロジェクション逆行列セット
	* @param[in] const Conv_XM::Matrix4x4f&(viewMatrix) ビュープロジェクション行列セット
	*/
	void Set3DCameraViewProjectionInverse(const Conv_XM::Matrix4x4f& viewProjectionInverse);

	/**
	* @brief カメラのポジションセット
	* @param[in] const Conv_XM::Vector4f&(cameraPosition) カメラのポジションセットセット
	*/
	void SetCameraPositon(const Conv_XM::Vector4f& cameraPosition);


	/**
	* @brief 乱数セット
	* @param[in] float(randomNum) 乱数
	*/
	void SetRandomNumber(float randomNum);

	/**
	* @brief ディレクショナルライトセット
	* @param[in] const CBufferData::DirectionalLight&(directionalLight) ディレクショナルライト
	*/
	void SetDirectionalLight(const CBufferData::DirectionalLight& directionalLight);

	/**
	* @brief ポイントライトセット
	* @param[in] const CBufferData::PointLight&(pointLight) ポイントライトセット
	*/
	void SetPointLight(const CBufferData::PointLight& pointLight);

	/**
	* @brief マテリアルセット
	* @param[in] const CBufferData::Material&(material) マテリアルセット
	*/
	void SetMaterial(const CBufferData::Material& material);

	/**
	* @brief ライトビュープロジェクションセット
	* @param[in] const Conv_XM::Matrix4x4f&(lightViewProjection) ライトビュープロジェクション
	*/
	void SetLightViewProjection(const Conv_XM::Matrix4x4f& lightViewProjection);

	/**
	* @briefボーンセット
	* @param[in] const CBufferData::BONES&(bones) ボーンデータ
	*/
	void SetBones(const CBufferData::BONES& bones);

	/**
	* @brief ブレンドステートセット
	* @param[in] BlendStateName(blendStateName) ブレンドステート
	*/
	void SetBlendState(BlendStateName blendStateName);

	/**
	* @brief デプスステンシルステートセット
	* @param[in] BlendStateName(blendStateName) デプスステンシルステート
	*/
	void SetDepthStencilState(DepthStencilStateName depthStencilStateName);

	/**
	* @brief ビューポート登録
	* @param[in] D3D11_VIEWPORT(viewPort) ビューポート構造体
	*/
	void SetViewPort(D3D11_VIEWPORT viewPort);

	/**
	* @brief デバイスの取得
	*/
	inline ID3D11Device* GetDevice(void) const { return m_Device.Get(); }

	/**
	* @brief デバイスコンテキストの取得
	*/
	inline ID3D11DeviceContext* GetDeviceContext(void) const { return m_DeviceContext.Get(); }

	/**
	* @brief スワップチェーンの取得
	*/
	inline IDXGISwapChain* GetSwapChain() const { return m_SwapChain.Get(); }

	/**
	* @brief ディファードレンダリングクラスの取得
	*/
	inline class DeferredRendering* GetDeferredRendering() const { return m_DeferredRendering; }

	/**
	* @brief レンダリング結果表示クラスの取得
	*/
	inline class OutPutRenderingResult* GetOutPutRenderingResult() const { return m_OutPutRenderingResult; }
private:
	/**
	* @brief スワップチェーン作成
	*/
	bool CreateDeviceSwapChain();

	/**
	* @brief ビューポート登録
	*/
	void RegisterViewport();

	/**
	* @brief ラスタライザー作成
	*/
	bool CreateRasterizer();

	/**
	* @brief レンダーターゲット作成
	*/
	bool CreateRenderTarget();

	/**
	* @brief デプスステンシル作成
	*/
	bool CreateDepthSetncil();

	/**
	* @brief デプスステンシルステート作成
	*/
	bool CreateDepthStencilState();

	/**
	* @brief ブレンドステート作成
	*/
	bool CreatBlendState();

	/**
	* @brief サンプラーステート作成
	*/
	bool CreateSamplerState();

	/**
	* @brief コンスタントバッファ作成
	*/
	bool CreateConstantBuffer();

	/**
	* @brief レンダリングパイプライン破棄
	*/
	void UnloadRenderingPipline();
};
