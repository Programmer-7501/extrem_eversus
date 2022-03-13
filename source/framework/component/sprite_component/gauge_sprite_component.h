/**
 * @file gauge_sprite_component.h
 * @brief ゲージスプライトコンポーネント
 */

#pragma once

#include"changeable_sprite_component.h"

 /**
 * @brief ゲージスプライトコンポーネント
 */
class GaugeSpriteComponent : public ChangeableSpriteComponent
{
public:
	enum Direction
	{
		LEFT,
		RIGHT,
	};
private:
	//! 動く方向
	Direction m_Direction;

	//! 割合
	float m_Ratio;

	//! 最大の長さ
	float m_MaxLength;

	//! 開始のポジション
	Conv_XM::Vector2f m_StartPosition;
public:
	GaugeSpriteComponent(class Actor* owner);
	virtual ~GaugeSpriteComponent()override;

	/**
	* @brief 更新処理
	*/
	virtual void Update()override;

	inline void SetDirection(Direction dir) { m_Direction = dir; }
	inline void SetRatio(float ratio) { m_Ratio = ratio; }
	inline void SetMaxLength(float maxLength) { m_MaxLength = maxLength; }
	inline void SetStartPosition(const Conv_XM::Vector2f& start) { m_StartPosition = start; }
};