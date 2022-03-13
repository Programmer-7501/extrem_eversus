/**
 * @file changeable_billboard_component.h
 * @brief 変更可能ビルボードコンポーネント
 */

#pragma once

#include"billboard_component.h"

class ChangeableBillBoardComponent : public BillBoardComponent
{
protected:
	//! 色
	Conv_XM::Vector4f m_Color;
	//! 更新フラグ
	bool m_IsUpdate;

public:
	ChangeableBillBoardComponent(class Actor* owner);
	virtual ~ChangeableBillBoardComponent();


	/**
	* @brief 更新処理
	*/
	virtual void Update()override;

	/**
	* @brief 描画処理
	*/
	void Draw() const override;
private:

	/**
	* @brief 頂点構造体の更新
	*/
	virtual void UpdateVetexData();

	/**
	* @brief データ読み込み
	* @return bool 成功したかどうか
	*/
	bool LoadData() override;

	/**
	* @brief メッシュ作成
	* @return bool 成功したかどうか
	*/
	bool CreateMesh() override;

	/**
	* @brief シェーダーの取得
	* @return bool 成功したかどうか
	*/
	bool GetShader() override;
};