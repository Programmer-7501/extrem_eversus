/**
 * @file bazooka.h
 * @brief バズーカ
 */

#pragma once

#include"bullet.h"

 /**
 * @brief バズーカクラス
 */
class Bazooka : public Bullet
{
public:
	/**
	* @brief モデル読み込み処理
	*/
	static void LoadModel();

	/**
	* @brief モデル破棄処理
	*/
	static void UnloadModel();
private:
	//! モデルコンポーネント
	class ModelComponent* m_ModelComponent;

public:
	Bazooka();
	~Bazooka();

protected:

	/**
	* @brief バズーカ独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief バズーカ独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief バズーカ独自の入力処理
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief バズーカ独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 当たり判定処理
	* @details 衝突時に呼び出される関数
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;
};