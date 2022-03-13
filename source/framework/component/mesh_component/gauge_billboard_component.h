/**
 * @file gauge_billboard_component.h
 * @brief ゲージビルボードコンポーネント
 */

#pragma once

#include"changeable_billboard_component.h"

 /**
 * @brief ゲージスプライトコンポーネント
 */
class GaugeBillBoardComponent : public ChangeableBillBoardComponent
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

	//! 長さ
	float m_Length;

	//! 開始のポジション
	Conv_XM::Vector3f m_StartPosition;
public:
	GaugeBillBoardComponent(class Actor* owner);
	virtual ~GaugeBillBoardComponent()override;

	/**
	* @brief 更新処理
	*/
	virtual void Update()override;

	inline void SetDirection(Direction dir) { m_Direction = dir; }
	inline void SetRatio(float ratio) 
	{
		m_Ratio = ratio;
		m_IsUpdate = true;
	}
	inline void SetMaxLength(float maxLength) { m_MaxLength = maxLength; }
	inline void SetStartPosition(const Conv_XM::Vector3f& start) { m_StartPosition = start; }

private:

	/**
	* @brief 頂点構造体の更新
	*/
	virtual void UpdateVetexData() override;
};