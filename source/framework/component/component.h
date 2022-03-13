/**
 * @file component.h
 * @brief ゲームオブジェクトのコンポーネント
 */

#pragma once

 /**
  * @brief ゲームオブジェクトのコンポーネント
  */
class Component
{
protected:
	enum UPDATEORDER
	{
		UPDATEORDER_UNKNOWN,
	};
protected:
	//! 所有アクター
	class Actor* m_Owner;

	//! 更新順
	int m_UpdateOrder;
public:
	Component(class Actor* owner,int updateOrder = UPDATEORDER_UNKNOWN);
	virtual ~Component();

	/**
	* @brief 読み込み処理
	*/
	virtual void Load();

	/**
	* @brief 初期化処理
	*/
	virtual void Init();

	/**
	* @brief 終了処理
	*/
	virtual void Uninit();

	/**
	* @brief 入力処理
	*/
	virtual void ProcessInput();

	/**
	* @brief 更新処理
	*/
	virtual void Update();

	/**
	* @brief 座標更新処理関数
	* @details 所有アクターが座標更新を行った時に呼び出される関数
	*/
	virtual void OnUpdateWorldTransform();

	// ゲッター
	inline int GetUpdateOrder() const { return m_UpdateOrder; }
	
	inline Actor* GetOwner() const { return m_Owner; }
};