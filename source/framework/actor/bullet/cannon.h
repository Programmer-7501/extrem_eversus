/**
 * @file bullet.h
 * @brief バレット
 */

#pragma once

#include"bullet.h"

 /**
 * @brief キャノンクラス
 */
class Cannon : public Bullet
{
private:
	//! 弾の速度
	Conv_XM::Vector3f m_Velocity;
public:
	Cannon();
	~Cannon();

	/**
	* @brief アクティブ処理
	*/
	virtual void SetActive(bool flag)override;

	/**
	* @brief 速度のセット
	* @param[in] const Conv_XM::Vector3f&(velocity) 速度
	*/
	virtual void SetVelocity(const Conv_XM::Vector3f& velocity) override;

protected:

	/**
	* @brief キャノン独自の読み込み処理
	*/
	virtual void LoadActor() override;

	/**
	* @brief キャノン独自の初期化処理
	*/
	virtual void InitActor() override;

	/**
	* @brief キャノン独自の入力処理
	*/
	virtual void ProcessInputActor()override;

	/**
	* @brief キャノン独自の更新処理
	*/
	virtual void UpdateActor()override;

	/**
	* @brief 当たり判定処理
	* @details 衝突時に呼び出される関数
	*/
	virtual void CollisionDetected(class Actor* coleObj) override;
};