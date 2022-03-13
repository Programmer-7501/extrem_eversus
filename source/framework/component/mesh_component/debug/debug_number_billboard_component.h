/**
 * @file billboard_component.h
 * @brief ビルボードコンポーネント
 */

#pragma once

#include"../changeable_billboard_component.h"

class DebugNumberBillBoardComponent : public ChangeableBillBoardComponent
{
public:
	/**
	* @brief 数字テクスチャの読み込み
	*/
	static void LoadNumberTexture();

	/**
	* @brief 数字テクスチャの破棄
	*/
	static void UnloadNumberTexture();
private:
	int m_DrawNumber;

public:
	DebugNumberBillBoardComponent(class Actor* owner);
	virtual ~DebugNumberBillBoardComponent();

	void SetNumber(int num);
};