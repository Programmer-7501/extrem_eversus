/**
 * @file changeable_color_sprite_component.h
 * @brief 変更可能スプライトコンポーネント
 */

#pragma once

#include"sprite_component.h"

 /**
 * @brief 変更可能スプライトコンポーネント
  * @details 設定を更新中に変更する必要のあるスプライトに使用
 */
class ChangeableSpriteComponent : public SpriteComponent
{
protected:
	//! 色
	Conv_XM::Vector4f m_Color;
	//! 更新フラグ
	bool m_IsUpdate;
public:
	ChangeableSpriteComponent(class Actor* owner);
	virtual ~ChangeableSpriteComponent()override;

	// セッター、ゲッター
	void SetColor(const Conv_XM::Vector4f color);

protected:
	/**
	* @brief 初期化処理
	*/
	virtual void Init()override;

	/**
	* @brief 終了処理
	*/
	virtual void Uninit()override;

	/**
	* @brief 更新処理
	*/
	virtual void Update()override;

	/**
	* @brief 描画処理
	*/
	virtual void Draw() const override;

private:

	/**
	* @brief 頂点構造体の更新
	*/
	void UpdateVetexData();

	/**
	* @brief シェーダーの取得
	* @return bool 成功したかどうか
	*/
	bool GetShader() override;
};