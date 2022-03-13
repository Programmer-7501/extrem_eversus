/**
 * @file mobilesuit_infomation_ui.h
 * @brief モビルスーツの情報UI
 */

#pragma once

#include"../../actor.h"

 /**
 * @brief モビルスーツの情報UIクラス
 */
class MobileSuitInfomationUI : public Actor
{
public:
	enum MobileSuitName
	{
		ENEMY001,
		ENEMY002,
		ALLY,
	};
public:

	/**
	* @brief テクスチャ読み込み
	*/
	static void LoadTexture();

	/**
	* @brief テクスチャ破棄
	*/
	static void UnloadTexture();

protected:
	//! 親モビルスーツ
	class MobileSuit* m_ParentActor;

	//! ライフコンポーネント
	class LifeComponent* m_LifeComponent;

	//! 表示名ビルボードコンポーネント
	class BillBoardComponent* m_NameTagBillBoardComponent;

	//! ライフ表示ビルボードコンポーネント
	class GaugeBillBoardComponent* m_LifeGaugeBillBoardComponent;

	//! モビルスーツの名前
	MobileSuitName m_MobileSuitName;
public:
	MobileSuitInfomationUI();
	~MobileSuitInfomationUI();

	inline void SetParentActor(class MobileSuit* mobileSuit) { m_ParentActor = mobileSuit; }

	inline void SetMobileSuitName(MobileSuitName mobileSuitName) { m_MobileSuitName = mobileSuitName; }
protected:

	/**
	* @brief モビルスーツの情報UI独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief モビルスーツの情報UI独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief モビルスーツの情報UI独自の更新処理
	*/
	virtual void UpdateActor() override;
};