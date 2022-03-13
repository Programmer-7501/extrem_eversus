/**
 * @file bullet_manager.h
 * @brief バレットの管理を行う
 */

#pragma once

#include"actor_manager.h"
#include"../actor/bullet/bullet.h"

 /**
 * @brief バレットの管理をするクラス
 */
class BulletManager : public ActorManager
{
	enum ReloadMethod
	{
		//! 一個ずつ
		RM_ONEBYONE,
		//! 使い切ったら
		RM_ALLUSE,
		//! リロードしない
		RM_NONE_RELOAD,
		RM_MAX,
	};
private:
	//! バレットアクターズ
	std::vector<Bullet*> m_Bullets;

	//! 弾最大数
	int m_MaxBullet;

	//! 残弾数
	int m_CurrentBullet;

	//! リロード方法
	ReloadMethod m_ReloadMethod;

	//! リロードカウント
	int m_ReloadCount;

	//! リロード時間
	int m_ReloadTime;
public:
	BulletManager();
	~BulletManager();

	/**
	* @brief 弾の使用
	* @param[in] class MobileSuit*(target) 狙う相手
	* @param[in] const Conv_XM::Vector3f&(position) 弾の発生ポジション
	* @param[in] const Conv_XM::Vector3f&(velocity) 弾の速度
	* @return class Bullet* 使用するバレット
	*/
	class Bullet* UseBullet(class MobileSuit* target, const Conv_XM::Vector3f& position, const Conv_XM::Vector3f& velocity);

	/**
	* @brief アクター生成関数
	* @param[in] int(maxInstanceNum) 管理する弾数
	* @param[in] int(maxBulletNum) 最大の弾数
	*/
	template<typename T>
	void CreateBulletActors(int maxInstanceNum, int maxBulletNum)
	{
		m_MaxBullet = maxBulletNum;
		m_CurrentBullet = maxBulletNum;
		m_Bullets.reserve(maxInstanceNum);

		for (int i = 0; i < maxInstanceNum; i++)
		{
			// アクターを生成
			T* bullet = new T;
			// nullチェック
			if (bullet == nullptr)
			{
				Logger::GetInstance().SetLog("BulletManager::CreateBulletActors bulletがnullptr");
				return;
			}
			//アクター列に登録
			m_Bullets.emplace_back(bullet);
		}
	}


	/**
	* @brief リロード処理
	*/
	void Reload();

	/**
	* @brief 初期化
	*/
	void Reset();


	//セッター、ゲッター
	inline void SetReloadMethod(ReloadMethod reloadMethod, int reloadTime = 0)
	{
		m_ReloadMethod = reloadMethod;
		m_ReloadTime = reloadTime;
	}

	inline int GetMaxBullet() const { return m_MaxBullet; }
	inline int GetCurrentBullet() const { return m_CurrentBullet; }

	void SetLandingEffectManager(class EffectManager* effectManager);
private:

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

};