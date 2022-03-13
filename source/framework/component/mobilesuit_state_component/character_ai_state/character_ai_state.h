/**
 * @file charcter_ai_state.h
 * @brief キャラクターAIステート
 */

#pragma once

 /**
 * @brief キャラクターAIステートクラス
 */
class CharcterAIState
{
protected:
	//! 所有されているコンポーネント
	class MobileSuitAIStateComponent* m_OwnerComponent;

	//! 所有されているモビルスーツアクター
	class MobileSuit* m_MobileSuit;

	//! ブーストコンポーネント
	class BoostComponent* m_BoostComponent;

	//! エネミー001のポインタ
	class MobileSuit* m_EnemyMobileSuit001;
	//! エネミー002のポインタ
	class MobileSuit* m_EnemyMobileSuit002;
	//! ターゲットナンバー
	int m_TargetNum;
public:
	CharcterAIState(class MobileSuitAIStateComponent* ownerComponent)
		:m_OwnerComponent(ownerComponent)
	{}

	virtual ~CharcterAIState() {}

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

	/**
	* @brief 親からコンポーネントを取得する
	* @return 成功したかどうか
	*/
	bool GetData();
};

