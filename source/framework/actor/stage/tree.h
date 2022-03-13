/**
 * @file tree.h
 * @brief 木のオブジェクト
 */

#pragma once

#include"../actor.h"

 /**
 * @brief 木のオブジェクト配置クラス
 */
class Tree : public Actor
{
public:

	/**
	* @brief テクスチャ読み込み
	*/
	static void LoadTexture();

	/**
	* @brief テクスチャの破棄
	*/
	static void UnloadTexture();

private:
	//! ビルボードコンポーネント
	std::vector<class BillBoardComponent*> m_BillBoardComponents;

public:
	Tree();
	~Tree() override;

private:

	/**
	* @brief 読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief 初期化処理
	*/
	void InitActor() override;
};
