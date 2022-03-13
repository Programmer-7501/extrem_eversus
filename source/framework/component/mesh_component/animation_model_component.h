/**
* @file model_component.h
* @brief 自作モデルファイル
*/

#pragma once

#include"mesh_component.h"

/**
* @brief 自作モデルを扱うコンポーネント
*/
class AnimationModelComponent : public MeshComponent
{
private:
	//! 自作モデルクラス
	class AnimationModel* m_AnimationModel;

	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! 頂点シェーダー
	ID3D11VertexShader* m_VertexShader;
	//! ピクセルシェーダーシェーダー
	ID3D11PixelShader* m_PixelShader;
	//! インプットレイアウト
	ID3D11InputLayout* m_InputLayout;

	//! 現在のアニメーションの名前
	std::string m_CurrentAnimeName;

	//! オフセットサイズ
	Conv_XM::Vector3f m_OffsetScale;

	//! オフセットローテーション
	Conv_XM::Vector4f m_Quaternion;

	//!フレームカウント
	int m_CurrentFrame;

	//! 最大カウント
	int m_MaxCount;

	//! 停止フラグ
	bool m_StopFlag;

	//! 再生速度用積み上げるカウント
	float m_Counter;

	//! カウントを増やす速度
	float m_CounterSpeed;

	//! ブレンドモード
	bool m_IsBlendMode;

	//! ブレンド時間
	int m_MaxBlendCount;

	//! ブレンド時間計測タイマー
	int m_BlendCount;

	//! ブレンドするアニメーションの名前
	std::string m_BlendAnime001Name;

	//! ブレンドするアニメーションのカウント
	int m_BlendAnime001Count;

	//! ブレンドするアニメーションの最大カウント
	int m_BlendAnime001MaxCount;

	//! ブレンドするアニメーションの名前
	std::string m_BlendAnime002Name;

	//! ブレンドするアニメーションのカウント
	int m_BlendAnime002Count;

	//! ブレンドするアニメーションの最大カウント
	int m_BlendAnime002MaxCount;
public:
	AnimationModelComponent(class Actor* owner);
	~AnimationModelComponent() {}

	/**
	* @brief 更新処理
	*/
	virtual void Update() override;

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const override;

	/**
	* @brief 影描画処理
	*/
	virtual void ShadowDraw() const override;


	/**
	* @brief 現在のアニメーションとブレンド(ブレンド終了後は入力したアニメーション
	* @param[in] const std::string&(blendAnimationName) ブレンドアニメーション(次回に設定するアニメーション)
	* @param[in] float(time) ブレンド時間
	*/
	void SetCurrentAnimationBlendToNextAnimation(const std::string& blendAnimationName, int time);

	/**
	* @brief 使用パーティクルの名前のセット
	* @param[in] const std::string&(blendAnimationName001) ブレンドアニメーション
	* @param[in] const std::string&(blendAnimationName002) ブレンドアニメーション(次回に設定するアニメーション)
	* @param[in] float(time) ブレンド時間
	*/
	void SetBlendAnimation(const std::string& blendAnimationName001, const std::string& blendAnimationName002, int time);

	// セッター
	void SetAnimationModel(class AnimationModel* animationModel);

	void SetAnimation(const std::string& name);

	void ResetAnimation(const std::string& name);

	inline void SetOffsetScale(const Conv_XM::Vector3f scale) { m_OffsetScale = scale; }

	inline void SetOffsetQuaternion(const Conv_XM::Vector4f quaternion) { m_Quaternion = quaternion; }

	inline void SetCounetSpeed(float counterSpeed) { m_CounterSpeed = counterSpeed; }

	inline void SetCurrentFrame(int currentFrame) { m_CurrentFrame = currentFrame; }

	void SetIsIdentity(bool flag);

	/**
	* @brief アニメーション停止
	* @param[in] bool(flag) 止めるかどうか
	* @param[in] int(time) 停止時間
	*/
	void StopModel(bool flag, int time = 0);
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