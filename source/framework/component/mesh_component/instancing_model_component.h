/**
* @file instancing_model_component.h
* @brief 自作モデルをインスタンシング描画するファイル
*/

#pragma once

#include"mesh_component.h"

/**
* @brief 自作モデルをインスタンシング描画するコンポーネント
*/
class InstancingModelComponent : public MeshComponent
{
private:
	//! 自作モデルクラス
	class Model* m_Model;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! GBuffer入力用頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! 最大数
	int m_MaxInstanceNum;

	//! インスタンシングマトリクスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceMatrixBuffer;

	//! インスタンシングマトリクスデータ
	std::vector<Conv_XM::Matrix4x4f> m_InstanceMatrixData;
public:
	InstancingModelComponent(class Actor* owner);
	~InstancingModelComponent() {}

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const override;

	/**
	* @brief 影描画処理
	*/
	virtual void ShadowDraw() const override;

	/**
	* @brief インスタンシングマトリクスデータセット
	* @details 初期化する前に使用する
	*/
	void SetInstanceMatrixData(const std::vector<Conv_XM::Matrix4x4f>& instanceMatrixData);

	// セッター
	inline void SetModel(class Model* model) { m_Model = model; }

private:
	/**
	* @brief データ読み込み
	* @return bool 成功したかどうか
	*/
	virtual bool LoadData();

	/**
	* @brief デバイスコンテキストの取得
	* @return bool 成功したかどうか
	*/
	bool GetDeviceContext();

	/**
	* @brief シェーダーの取得
	* @return bool 成功したかどうか
	*/
	bool GetShader();
};