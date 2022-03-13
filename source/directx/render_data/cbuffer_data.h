/**
* @file cbuffer_data.h
* @brief コンスタントバッファの情報集約
*/

#pragma once

#include"../../shader/shader_data.h"

/**
* @namespace CBufferData
* @brief コンスタントバッファの情報
*/
namespace CBufferData
{
	enum Index
	{
		//! ワールド行列のインデックス番号
		WORLDMATRIX_INDEX,
		//! 描画データのインデックス番号
		DRAWDATA_INDEX,
		//! パーティクルエミッター配列のインデックス番号
		PARTICLEEMITTERARRAY_INDEX,
		//! 死亡パーティクル数のインデックス番号
		DEADPARTICLENUMBER_INDEX,
		//! ディレクショナルライトのインデックス番号
		DIRECTIONALLIGHT_INDEX,
		//! ポイントライトのインデックス番号
		POINTLIGHT_INDEX,
		//! マテリアルデータのインデックス番号
		MATERIAL_INDEX,
		//! ライトカメラのインデックス番号
		LIGHTVIEWPROJECTION_INDEX,
		//! 使用パーティクルエミッターのインデックス番号
		USEPARTICLEEMITTER_INDEX,
		//! ボーンデータのインデックス番号
		BONE_INDEX,
		//! インデックス数
		MAX_INDEX,
	};


	struct DrawData
	{
		Conv_XM::Matrix4x4f View;
		Conv_XM::Matrix4x4f Projection;
		Conv_XM::Matrix4x4f CameraViewProjectionInverse;
		Conv_XM::Vector3f CameraPosition;
		float RandomNumber;
	};

	struct BONES
	{
		Conv_XM::Matrix4x4f bones[MAX_BONE_NUM];
	};

	enum EmitterType
	{
		//! スフィア型に発生
		ET_SPHERE,
		//! レクト型に発生
		ET_RECT,
		//! デモ用
		ET_DEMO,
		ET_MAX,
	};

	/**
	* @brief エミッターデータ構造体
	*/
	struct ParticleEmitter
	{
		//! エミッターのポジション
		Conv_XM::Vector3f EmitterPosition;
		//! ライフによってサイズを減らすかどうか
		UINT SizeOverLifeTime;

		//! パーティクルの速度
		Conv_XM::Vector3f ParticleVelocity;
		float pad2;

		//! パーティクルの生成ポジションのばらつき
		Conv_XM::Vector3f PositionVariance;
		float pad3;

		//! カラー
		Conv_XM::Vector4f Color;

		//! このフレームにおけるパーティクルの最大生成数
		int MaxSpawnParticlesThisFrame;
		//! このフレームの生存時間
		float ParticleLifeSpan;
		//! 使用するか否か
		int IsUse;
		//! 自身のインデックス番号
		UINT MyIndex;

		//! 速度
		float Speed;
		//! エミッターの種類
		UINT EmitterType;
		//! パーティクルのサイズ
		Conv_XM::Vector2f Size;

		// 速度のばらつき
		Conv_XM::Vector3f VelocityVariance;
		// 重力
		float Gravity;
	};

	/**
	* @brief ディレクショナルライトデータ構造体
	*/
	struct DirectionalLight
	{
		//! ディレクショナルライトの角度
		Conv_XM::Vector3f Direction;
		float pad0;
		//! ディレクショナルライトの色
		Conv_XM::Vector3f Color;
		float pad1;
		//! アンビエントライト
		Conv_XM::Vector3f Ambient;
		float pad2;
	};

	/**
	* @brief ポイントライトデータ構造体
	*/
	struct PointLight
	{
		//! ポイントライトのポジション
		Conv_XM::Vector3f Position;
		float pad;
		//! ポイントライトの色
		Conv_XM::Vector3f Color;
		//! ポイントライトの有効範囲
		float Range;
	};

	//!	マテリアルデータ
	struct Material
	{
		Conv_XM::Vector4f Ambient;
		Conv_XM::Vector4f Diffuse;
		float Metaric;
		float Smoothness;
		Conv_XM::Vector2f pad;
	};

	struct ShadowData
	{
		Conv_XM::Matrix4x4f LightViewProjection;
		Conv_XM::Matrix4x4f ShadowBiasMatrix;
	};
}
