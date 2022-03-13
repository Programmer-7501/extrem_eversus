/**
 * @file other_actor_manager.h
 * @brief その他アクターの管理を行う
 */

#pragma once

#include"actor_manager.h"
#include"../actor/actor.h"

 /**
 * @brief その他アクターの管理をするクラス
 */
class OtherActorManager : public ActorManager
{
protected:
	//! アクター集合体
	std::vector<class Actor*> m_Actors;
public:
	OtherActorManager();
	~OtherActorManager();

	/**
	* @brief 読み込み処理
	*/
	void LoadData() override;

	/**
	* @brief 初期化処理
	*/
	void Init() override;

	/**
	* @brief 終了処理
	*/
	void Uninit() override;

	/**
	* @brief 入力処理
	*/
	virtual void ProcessInput()override;

	/**
	* @brief 更新処理
	*/
	void Update() override;

	/**
	* @brief アクター生成関数
	* @return T 作成したアクター
	*/
	template<typename T>
	T* CreateActor()
	{
		// アクターを生成
		T* actor = new T;
		// nullチェック
		if (actor == nullptr)
		{
			Logger::GetInstance().SetLog("OtherActorManager::CreateActor actorがnullptr");
			return nullptr;
		}
		//アクター列に登録
		m_Actors.emplace_back(actor);

		return actor;
	}
};