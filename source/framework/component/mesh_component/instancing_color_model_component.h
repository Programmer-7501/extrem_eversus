/**
* @file instancing_color_model_component.h
* @brief 自作モデルをインスタンシング描画するファイル
* @details 途中でインスタンシングデータ変更可能
* @details 影描画は非対応
*/

#pragma once

#include"mesh_component.h"

/**
* @brief 自作モデルをインスタンシング描画するコンポーネント
* @details 途中でインスタンシングデータ変更可能
*/
class InstancingColorModelComponent : public MeshComponent
{
public:
	struct InstanceColorData
	{
		Conv_XM::Matrix4x4f Matrix;
		Conv_XM::Vector4f Color;
	};
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
	std::vector<InstanceColorData> m_InstanceColorData;

	//! 更新フラグ
	bool m_IsUpdate;
public:
	InstancingColorModelComponent(class Actor* owner);
	~InstancingColorModelComponent() {}

	/**
	* @brief 更新処理
	*/
	void Update()override;

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const override;

	/**
	* @brief インスタンシング最大数セット
	* @details 初期化する前に使用する
	*/
	inline void SetMaxInstancingNum(int maxInstancingNum) { m_MaxInstanceNum = maxInstancingNum; }

	/**
	* @brief インスタンシングデータセット
	*/
	inline void SetInstanceData(const std::vector<InstanceColorData>& instanceData) 
	{
		m_IsUpdate = true;
		m_InstanceColorData = instanceData; 
	}


	// セッター
	inline void SetModel(class Model* model) { m_Model = model; }

private:

	/**
	* @brief インスタンシングデータ作成
	* @return bool 成功したかどうか
	*/
	virtual bool CreateInstancingData();

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