/**
 * @file bullet.h
 * @brief �o���b�g
 */

#pragma once

#include"bullet.h"
#include"../../../directx/render_data/cbuffer_data.h"

 /**
 * @brief �o���b�g�N���X
 */
class EnemyBullet : public Bullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	/**
	* @brief �o���b�g�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;
};