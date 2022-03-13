/**
* @file animation_model.h
* @brief 自作アニメモデルファイル
*/

#pragma once

#include"../render_data/vertex_structure.h"
#include"../render_data/cbuffer_data.h"

/**
* @brief 自作アニメモデルを扱うクラス
*/
class AnimationModel final
{
private:
	/**
	* @brief メッシュデータ構造体
	*/
	struct AnimationMeshData
	{
		std::vector<ANIMATION_VERTEX_3D> Vertices;
		std::vector<UINT> Indices;
		std::string TextureName;

		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		ID3D11ShaderResourceView* TextureData;
	};

	struct FRAMEDATA
	{
		int Frame;
		std::vector<Conv_XM::Matrix4x4f> Bones;
	};
protected:
	//! ファイル名
	std::string m_FileName;
	//! メッシュリスト
	std::vector<AnimationMeshData> m_MeshList;
	//! デバイス
	ID3D11Device* m_Device;
	//! デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	//! アニメーションデータ
	std::unordered_map<std::string, std::vector<FRAMEDATA>> m_Animation_Datas;

	//! 現在のアニメーションデータ
	std::vector<FRAMEDATA> m_CurrentAnimeData;

	//! ファイルから読み込んだ現在のボーンデータ
	std::vector<Conv_XM::Matrix4x4f> m_CurrentBones;

	//! 今回のフレームで使用されるボーンデータ
	CBufferData::BONES m_Bones;

	//!フレームカウント
	int m_CurrentFrame;

	//! 何もしない
	bool m_IsIdentity;

public:
	AnimationModel();
	virtual ~AnimationModel();

	/**
	* @brief 自作モデルの読み込み
	* @param[in] const std::string&(fileName) ファイル名
	* @return bool 読み込みが成功したかどうか
	*/
	virtual bool Load(const std::string& fileName);

	/**
	* @brief 自作モデルの破棄
	*/
	virtual void Unload();

	/**
	* @brief 自作モデルの更新
	*/
	virtual void Update();

	/**
	* @brief 自作モデルの描画
	*/
	virtual void Draw() const;

	/**
	* @brief 行列のセット
	* @param[in] int(frame) アニメーションフレーム
	* @return int 現在のアニメーションフレーム
	*/
	int SetMatrix(const std::string& animationName, int frame);

	/**
	* @brief 行列のセット(ブレンド版)
	* @param[in] const std::string&(animation001Name) アニメーションネーム
	* @param[in] int(frame001) アニメーションフレーム
	* @param[in] const std::string&(animation001Name) アニメーションネーム
	* @param[in] int(frame002) アニメーションフレーム
	* @param[in] float(t) 補間係数(0 ～ 1)
	* @return int 現在のアニメーションフレーム
	*/
	int SetBlendMatrix(const std::string& animation001Name, int frame001, const std::string& animation002Name, int frame002, float t);

	///**
	//* @brief 行列のセット(現在のアニメーションとブレンド版)
	//* @param[in] const std::string&(animation001Name) アニメーションネーム
	//* @param[in] int(frame001) アニメーションフレーム
	//* @param[in] float(t) 補間係数(0 ～ 1)
	//* @return int 現在のアニメーションフレーム
	//*/
	//int SetBlendMatrix(const std::string& blendanimationName, int frame, float t);


	int GetMaxFrame(const std::string& animationName);

	inline void SetIsIdentity(bool flag) { m_IsIdentity = flag; }

protected:
	/**
	* @brief 自作モデルの更新
	* @return bool 頂点バッファの作成が成功したかどうか
	*/
	bool CreateVertexBuffer();

	/**
	* @brief 自作モデルの更新
	* @return bool インデックスバッファの作成が成功したかどうか
	*/
	bool CreateIndexBuffer();

	/**
	* @brief 自作モデルの更新
	* @return bool テクスチャの読み込みが成功したかどうか
	*/
	bool LoadTexture();
};