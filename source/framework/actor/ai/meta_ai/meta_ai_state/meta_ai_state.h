/**
 * @file meta_ai_state.h
 * @brief メタAIステート
 */

#pragma once

#include"../meta_ai.h"

 /**
 * @brief メタAIステートクラス
 */
class MetaAIState
{
private:
	class MetaAI* m_Owner;
public:
	MetaAIState(class MetaAI* owner)
		:m_Owner(owner)
	{}

	virtual ~MetaAIState() {}

	/**
	* @brief 初期化処理
	*/
	virtual void Init() = 0;

	/**
	* @brief 入力処理
	*/
	virtual void ProcessInput() = 0;

	/**
	* @brief 更新処理
	*/
	virtual void Update() = 0;

	/**
	* @brief 状態に入った時の処理
	*/
	virtual void OnEnter() = 0;

	/**
	* @brief 状態から出るときの処理
	*/
	virtual void OnExit() = 0;

	/**
	* @brief 状態名
	*/
	virtual const char* GetName() const = 0;
};

