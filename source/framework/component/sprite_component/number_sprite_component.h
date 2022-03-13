/**
 * @file number_sprite_component.h
 * @brief ナンバースプライトコンポーネント
 */

#pragma once

#include"changeable_sprite_component.h"

 /**
 * @brief ナンバースプライトコンポーネント
 * @details 0から9まで対応
 */
class NumberSpriteComponent : public ChangeableSpriteComponent
{
public:
	//! 使用する数字テクスチャ
	static ID3D11ShaderResourceView* m_NumberTexture;

	/**
	* @brief 数字テクスチャの読み込み
	*/
	static void LoadNumberTexture();

	/**
	* @brief 数字テクスチャの破棄
	*/
	static void UnloadNumberTexture();
private:
	//! 表示する数字
	int m_DrawNumber;
public:
	NumberSpriteComponent(class Actor* owner);
	virtual ~NumberSpriteComponent()override;

	/**
	* @brief 表示する数字セット
	*/
	void SetNumber(int num);
};