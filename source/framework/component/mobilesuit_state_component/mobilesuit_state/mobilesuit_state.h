/**
 * @file state.h
 * @brief ステート
 */

#pragma once

 /**
 * @brief ステートクラス
 */
class MobileSuitState
{
protected:
	//! 方向
	enum Direction
	{
		// 前
		FORWARD,
		// 後ろ
		BACK,
		// 右
		RIGHT,
		// 左
		LEFT
	};
protected:
	//! 所有されているコンポーネント
	class MobileSuitStateComponent* m_Owner;

	//! リジッドボディコンポーネント
	class RigidBodyComponent* m_RigidBodyComponent;

	//! ロックオンカメラコンポーネント
	class LockOnCameraComponent* m_LockOnCameraComponent;

	//! ブーストコンポーネント
	class BoostComponent* m_BoostComponent;

	//! アニメーションコンポーネント
	class AnimationModelComponent* m_AnimationComponent;

	//! 所有されているモビルスーツアクター
	class MobileSuit* m_MobileSuit;

	//! ビームライフルバレットマネージャー
	class BulletManager* m_BeamRifleBulletManager;

	//! キャノンバレットマネージャー
	class BulletManager* m_CannonBulletManager;

	//! バズーカバレットマネージャー
	class BulletManager* m_BazookaBulletManager;

	//! エネミー001のポインタ
	class MobileSuit* m_EnemyMobileSuit001;
	//! エネミー002のポインタ
	class MobileSuit* m_EnemyMobileSuit002;
	//! ターゲットナンバー
	int m_TargetNum;

	//! ビームサーベル
	class BeamSaber* m_BeamSaber;
public:
	MobileSuitState(class MobileSuitStateComponent* ownerComponent);

	virtual ~MobileSuitState(){}

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

protected:

	/**
	* @brief 親からコンポーネントを取得する
	* @return 成功したかどうか
	*/
	bool GetData();

	/**
	* @brief アクターを回転させる
	* @param[in] const Vector3f&(cameraForward) カメラの前ベクトル
	* @param[in] const Vector3f&(actorForward) 自身の前ベクトル
	* @param[in] Direction(direction) 向きたい方角
	* @param[in] float(rotationLimit) 回転上限(degree)
	*/
	void RotationActor(const Conv_XM::Vector3f& cameraForward, const Conv_XM::Vector3f& actorForward, Direction direction, float rotationLimit = 0.0f);

	/**
	* @brief アクターを回転させる
	* @param[in] const Vector3f&(cameraForward) カメラの前ベクトル
	* @param[in] const Vector3f&(actorForward) 自身の前ベクトル
	* @param[in] Direction(direction) 向きたい方角
	* @param[in] float(rotationLimit) 回転上限(degree)
	*/
	void RotationActor(const Conv_XM::Vector3f& cameraForward, const Conv_XM::Vector3f& actorForward, const Conv_XM::Vector2f& direction, float rotationLimit = 0.0f);

	/**
	* @brief エネミーに時期を向ける
	*/
	void RotateToEnemy();
};

