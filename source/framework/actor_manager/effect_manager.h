/**
 * @file effect_manager.h
 * @brief エフェクトの管理を行う
 */

#pragma once

#include"actor_manager.h"
#include"../actor/effect/effect.h"

 /**
 * @brief エフェクトの管理をするクラス
 */
class EffectManager : public ActorManager
{
private:
	//! バレットアクターズ
	std::vector<Effect*> m_Effects;

public:
	EffectManager();
	~EffectManager();

	/**
	* @brief エフェクトの使用
	* @param[in] const Conv_XM::Vector3f&(position) エフェクトの発生ポジション
	* @return class Bullet* 使用するエフェクト
	*/
	class Effect* UseEffect(const Conv_XM::Vector3f& position);

	/**
	* @brief アクター生成関数
	* @param[in] int(maxInstanceNum) 管理するエフェクト数
	* @param[in] int(maxEffectNum) 最大のエフェクト数
	*/
	template<typename T>
	void CreateEffectActors(int maxInstanceNum)
	{
		m_Effects.reserve(maxInstanceNum);

		for (int i = 0; i < maxInstanceNum; i++)
		{
			// アクターを生成
			T* effect = new T;
			// nullチェック
			if (effect == nullptr)
			{
				Logger::GetInstance().SetLog("BulletManager::CreateBulletActors effectがnullptr");
				return;
			}
			//アクター列に登録
			m_Effects.emplace_back(effect);
		}
	}

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