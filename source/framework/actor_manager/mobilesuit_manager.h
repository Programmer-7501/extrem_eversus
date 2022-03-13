/**
 * @file mobilesuit_manager.h
 * @brief モビルスーツの管理を行う
 */

#pragma once

#include"actor_manager.h"
#include"../actor/mobilesuit/mobilesuit.h"

 /**
 * @brief モビルスーツの管理をするクラス
 */
class MobileSuitManager : public ActorManager
{
private:
	//! モビルスーツアクターズ
	std::vector<MobileSuit*> m_MobileSuits;
public:
	MobileSuitManager();
	~MobileSuitManager();

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
	* @brief モビルスーツの生成
	: @details モビルスーツの生成番号
	*/
	void SpawnMobileSuit(int spawnNum);


	/**
	* @brief アクター生成関数
	* @return T 作成したアクター
	*/
	template<typename T>
	T* CreateMobileSuitActor()
	{
		// アクターを生成
		T* mobileSuit = new T;
		// nullチェック
		if (mobileSuit == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitManager::CreateMobileSuitActor mobileSuitがnullptr");
			return nullptr;
		}
		//アクター列に登録
		m_MobileSuits.emplace_back(mobileSuit);

		return mobileSuit;
	}
};