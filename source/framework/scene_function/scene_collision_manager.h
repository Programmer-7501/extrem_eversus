/**
 * @file scene_collision_manager.h
 * @brief シーンコリジョンの管理
 */

#pragma once

 /**
 * @brief シーンコリジョンの管理クラス
 */
class SceneCollisionManager
{
public:
	enum MOBILESUIT_NAME
	{
		PLAYER,
		ALLY,
		ENEMY001,
		ENEMY002,
		MOBILESUIT_MAX
	};
private:
	//! メッシュフィールドコンポーネント
	class MeshFieldComponent* m_MeshFieldComponent;

	//! 弾のコリジョンコンポーネント
	std::vector<class OBBCollisionComponent*> m_BulletCollisions;

	//! エネミーのコリジョンコンポーネント
	std::vector<class OBBCollisionComponent*> m_EnemyCollisions;

	//! ビームサーベルのコリジョン
	std::vector<class OBBCollisionComponent*> m_BeamSaberCollisions;

	//! エネミーの弾のコリジョンコンポーネント
	std::vector<class OBBCollisionComponent*> m_EnemyBulletCollisions;

	//! ほか障害物のコリジョンコンポーネント
	std::vector<class AABBCollisionComponent*> m_ObstacleCollisions;

	//! モビルスーツのポインタ
	class MobileSuit* m_MobileSuits[MOBILESUIT_MAX];

	//! モビルスーツのリジッドボディ
	class RigidBodyComponent* m_RigidBodyComponent[MOBILESUIT_MAX];

	//! モビルスーツのコリジョン
	class OBBCollisionComponent* m_OBBCollision[MOBILESUIT_MAX];
	class AABBCollisionComponent* m_AABBCollision[MOBILESUIT_MAX];

public:
	SceneCollisionManager();
	~SceneCollisionManager();

	/**
	* @brief 初期化処理
	*/
	void Init();

	/**
	* @brief 更新処理
	*/
	void Update();

	/**
	* @brief 終了処理
	*/
	void Uninit();

	/**
	* @brief メッシュフィールドセット
	* @param[in] class MeshFieldComponent*(meshField) メッシュフィールドのポインタ
	*/
	inline void SetMeshField(class MeshFieldComponent* meshField) { m_MeshFieldComponent = meshField; }

	/**
	* @brief モビルスーツの情報セット
	* @param[in] class MeshFieldComponent*(meshField) メッシュフィールドのポインタ
	*/
	inline void SetMobileSuitData(MOBILESUIT_NAME mobileSuitName, class MobileSuit* mobileSuit, class  RigidBodyComponent* rigidBodyComponent, class OBBCollisionComponent* obbCollisionComponent, class AABBCollisionComponent* aabbCollisionComponent)
	{
		m_MobileSuits[mobileSuitName] = mobileSuit;
		m_RigidBodyComponent[mobileSuitName] = rigidBodyComponent;
		m_OBBCollision[mobileSuitName] = obbCollisionComponent;
		m_AABBCollision[mobileSuitName] = aabbCollisionComponent;
	}

	/**
	* @brief 弾のコリジョンコンポーネントセット
	* @param[in] class OBBCollisionComponent*(bulletCollision) 弾のコリジョンコンポーネントのポインタ
	*/
	void SetBulletCollision(class OBBCollisionComponent* bulletCollision);

	/**
	* @brief ビームサーベルのコリジョンコンポーネントセット
	* @param[in] class OBBCollisionComponent*(enemyCollision) ビームサーベルのコリジョンコンポーネントのポインタ
	*/
	void SetBeamSaberCollision(class OBBCollisionComponent* beamSaberCollision);

	/**
	* @brief エネミーの弾のコリジョンコンポーネントセット
	* @param[in] class OBBCollisionComponent*(bulletCollision) 弾のコリジョンコンポーネントのポインタ
	*/
	void SetEnemyBulletCollision(class OBBCollisionComponent* bulletCollision);

	/**
	* @brief ほか障害物のコリジョンコンポーネントセット
	* @param[in] class AABBCollisionComponent*(bulletCollision) 障害物のコリジョンコンポーネントのポインタ
	*/
	void SetObstacleCollision(class AABBCollisionComponent* obstacleCollision);
};