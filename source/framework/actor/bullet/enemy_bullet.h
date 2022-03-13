/**
 * @file bullet.h
 * @brief バレット
 */

#pragma once

#include"bullet.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief バレットクラス
 */
class EnemyBullet : public Bullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	/**
	* @brief バレット独自の読み込み処理
	*/
	virtual void LoadActor() override;
};