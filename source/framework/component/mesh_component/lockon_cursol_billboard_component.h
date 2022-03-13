/**
* @file lockon_cursol_billboard_component.h
* @brief ロックオンカーソルビルボードコンポーネント
*/

#pragma once

#include"mesh_component.h"

/**
* @brief ロックオンカーソルビルボードコンポーネント
*/
class LockonCursolBillboardComponent : public MeshComponent
{
public:
	enum LockOnState
	{
		LOCKONSTATE_RED,
		LOCKONSTATE_GREEN,
		LOCKONSTATE_YELLOW,
		LOCKONSTATE_MAX,
	};
private:
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;

	//! インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! GBuffer入力用頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! テクスチャ
	ID3D11ShaderResourceView* m_ShaderResourceView[LOCKONSTATE_MAX];

	//! ポジション
	Conv_XM::Vector3f m_Position;
	//! サイズ
	Conv_XM::Vector2f m_Size;

	//! シーンレンダーマネージャー
	class SceneRenderManager* m_SceneRenderManager;

	//! ロックオン状態
	LockOnState m_LockOnState;
public:
	LockonCursolBillboardComponent(class Actor* owner);
	~LockonCursolBillboardComponent() {}

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const override;

	inline void SetSRV(ID3D11ShaderResourceView* srv, LockOnState lockOnState) { m_ShaderResourceView[lockOnState] = srv; }
	inline void SetPosition(const Conv_XM::Vector3f& position) { m_Position = position; }
	inline void SetSize(const Conv_XM::Vector2f& size) { m_Size = size; }
	inline void SetLockOnState(LockOnState lockOnState) { m_LockOnState = lockOnState; }
	inline LockOnState GetLockOnState() const { return m_LockOnState; }
private:



	/**
	* @brief データ読み込み
	* @return bool 成功したかどうか
	*/
	virtual bool LoadData();

	/**
	* @brief メッシュ作成
	* @return bool 成功したかどうか
	*/
	bool CreateMesh();

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