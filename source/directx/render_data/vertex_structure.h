/**
* @file vertex_structure.h
* @brief 描画機能補助ファイル
* @see https://note.com/buti_programer/n/n87f38b2f0ee0
*/

#pragma once

/**
* @namespace VertexInformation
* @brief 頂点情報
*/
namespace VertexInformation 
{

	/**
	* @brief 頂点座標構造体
	*/
	struct VERTEX
	{
		Conv_XM::Vector3f Position;
		VERTEX(const Conv_XM::Vector3f& position) :Position(position) {}
		VERTEX() {}
	};

	/**
	* @brief uv座標構造体
	*/
	struct TEXCOORD
	{
		Conv_XM::Vector2f TexCoord;
		TEXCOORD(const Conv_XM::Vector2f& texcood) :TexCoord(texcood) {};
		TEXCOORD() {}
	};

	/**
	* @brief 法線構造体
	*/
	struct NORMAL
	{
		Conv_XM::Vector3f Normal;
		NORMAL(const Conv_XM::Vector3f& normal) :Normal(normal) {}
		NORMAL() {}
	};

	/**
	* @brief カラー構造体
	*/
	struct COLOR
	{
		Conv_XM::Vector4f Color;
		COLOR(const Conv_XM::Vector4f& color) :Color(color) {}
		COLOR() {}
	};

	/**
	* @brief ボーンID
	*/
	struct BONEID
	{
		Conv_XM::Vector4i BoneID1;
		Conv_XM::Vector4i BoneID2;
		BONEID(const Conv_XM::Vector4i& boneID1, const Conv_XM::Vector4i& boneID2) :BoneID1(boneID1), BoneID2(boneID2) {}
		BONEID() {}
	};

	/**
	* @brief ボーンウェイト
	*/
	struct BONEWEIGHT
	{
		Conv_XM::Vector4f BoneWeight1;
		Conv_XM::Vector4f BoneWeight2;
		BONEWEIGHT(const Conv_XM::Vector4f& boneWeight1, const Conv_XM::Vector4f& boneWeight2) :BoneWeight1(boneWeight1), BoneWeight2(boneWeight2) {}
		BONEWEIGHT() {}
	};


	/**
	* @brief マルチテクスチャブレンド変数
	*/
	struct TEXTUREBLENDE
	{
		Conv_XM::Vector4f TextureBlend;
		TEXTUREBLENDE(const Conv_XM::Vector4f& textureBlend) :TextureBlend(textureBlend) {}
		TEXTUREBLENDE() {}
	};
}

/**
* @brief 頂点座標とuv構造体
*/
struct VERTEX_TEXCOOD :public VertexInformation::VERTEX, public VertexInformation::TEXCOORD
{
	VERTEX_TEXCOOD(const Conv_XM::Vector3f& position, const Conv_XM::Vector2f& texcood) :VERTEX(position), TEXCOORD(texcood) {}
	VERTEX_TEXCOOD() {}
};

/**
* @brief 頂点座標と法線構造体
*/
struct VERTEX_NORMAL :public VertexInformation::VERTEX, public VertexInformation::NORMAL
{
	VERTEX_NORMAL(const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& normal) :VERTEX(position), NORMAL(normal) {}
	VERTEX_NORMAL() {}
};

/**
* @brief 頂点座標とカラー構造体
*/
struct VERTEX_COLOR :public VertexInformation::VERTEX, public VertexInformation::COLOR
{
	VERTEX_COLOR(const Conv_XM::Vector3f& position, const Conv_XM::Vector4f& color) : VERTEX(position), COLOR(color) {}
	VERTEX_COLOR() {}
};

/**
* @brief 頂点座標と法線とuv構造体
*/
struct VERTEX_NORMAL_TEXCOOD :public VertexInformation::VERTEX, public VertexInformation::NORMAL, public VertexInformation::TEXCOORD
{
	VERTEX_NORMAL_TEXCOOD(const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& normal, const Conv_XM::Vector2f& texcood) : VERTEX(position), NORMAL(normal), TEXCOORD(texcood) {}
	VERTEX_NORMAL_TEXCOOD() {}
};


/**
* @brief 頂点座標とカラー構造体
*/
struct VERTEX_COLOR_TEXCOOD :public VertexInformation::VERTEX, public VertexInformation::COLOR, public VertexInformation::TEXCOORD
{
	VERTEX_COLOR_TEXCOOD(const Conv_XM::Vector3f& position, const Conv_XM::Vector4f& color, const Conv_XM::Vector2f& texcood) : VERTEX(position), COLOR(color), TEXCOORD(texcood) {}
	VERTEX_COLOR_TEXCOOD() {}
};


/**
* @brief 頂点座標と法線とカラーとuv構造体
*/
struct VERTEX_NORMAL_COLOR_TEXCOOD :public VertexInformation::VERTEX, public VertexInformation::NORMAL, public VertexInformation::COLOR, public VertexInformation::TEXCOORD
{
	VERTEX_NORMAL_COLOR_TEXCOOD(const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& normal, const Conv_XM::Vector4f& color, const Conv_XM::Vector2f& texcood) : VERTEX(position), NORMAL(normal), COLOR(color), TEXCOORD(texcood) {}
	VERTEX_NORMAL_COLOR_TEXCOOD() {}
};

/**
* @brief アニメーション頂点構造体
*/
struct ANIMATION_VERTEX_3D : public VertexInformation::VERTEX, public VertexInformation::NORMAL, public VertexInformation::TEXCOORD, public VertexInformation::BONEID, public VertexInformation::BONEWEIGHT
{
	ANIMATION_VERTEX_3D(const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& normal, const Conv_XM::Vector2f& texcood, const Conv_XM::Vector4i boneID1, const Conv_XM::Vector4i boneID2, const Conv_XM::Vector4f& boneWeight1, const Conv_XM::Vector4f& boneWeight2) : VERTEX(position), NORMAL(normal), TEXCOORD(texcood), BONEID(boneID1, boneID2), BONEWEIGHT(boneWeight1, boneWeight2) {}
	ANIMATION_VERTEX_3D() {}
};

struct VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND :public VertexInformation::VERTEX, public VertexInformation::NORMAL, public VertexInformation::TEXCOORD, public VertexInformation::TEXTUREBLENDE
{
	VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND(const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& normal, const Conv_XM::Vector2f& texcood, const Conv_XM::Vector4f textureBlend) : VERTEX(position), NORMAL(normal), TEXCOORD(texcood), TEXTUREBLENDE(textureBlend) {}
	VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND() {}
};