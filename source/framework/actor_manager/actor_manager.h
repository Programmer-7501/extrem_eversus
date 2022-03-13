/**
 * @file actor_manager.h
 * @brief アクターの管理を行う
 */

#pragma once

#include"../actor/actor.h"

 /**
 * @brief アクターの管理をするクラス
 */
class ActorManager
{
public:
	ActorManager() {}
	virtual ~ActorManager() {}

	/**
	* @brief 読み込み処理
	*/
	virtual void LoadData() = 0;

	/**
	* @brief 初期化処理
	*/
	virtual void Init() = 0;

	/**
	* @brief 終了処理
	*/
	virtual void Uninit() = 0;

	/**
	* @brief 入力処理
	*/
	virtual void ProcessInput() = 0;

	/**
	* @brief 更新処理
	*/
	virtual void Update() = 0;
};