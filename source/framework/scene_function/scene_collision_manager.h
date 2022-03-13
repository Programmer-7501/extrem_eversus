/**
 * @file scene_collision_manager.h
 * @brief �V�[���R���W�����̊Ǘ�
 */

#pragma once

 /**
 * @brief �V�[���R���W�����̊Ǘ��N���X
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
	//! ���b�V���t�B�[���h�R���|�[�l���g
	class MeshFieldComponent* m_MeshFieldComponent;

	//! �e�̃R���W�����R���|�[�l���g
	std::vector<class OBBCollisionComponent*> m_BulletCollisions;

	//! �G�l�~�[�̃R���W�����R���|�[�l���g
	std::vector<class OBBCollisionComponent*> m_EnemyCollisions;

	//! �r�[���T�[�x���̃R���W����
	std::vector<class OBBCollisionComponent*> m_BeamSaberCollisions;

	//! �G�l�~�[�̒e�̃R���W�����R���|�[�l���g
	std::vector<class OBBCollisionComponent*> m_EnemyBulletCollisions;

	//! �ق���Q���̃R���W�����R���|�[�l���g
	std::vector<class AABBCollisionComponent*> m_ObstacleCollisions;

	//! ���r���X�[�c�̃|�C���^
	class MobileSuit* m_MobileSuits[MOBILESUIT_MAX];

	//! ���r���X�[�c�̃��W�b�h�{�f�B
	class RigidBodyComponent* m_RigidBodyComponent[MOBILESUIT_MAX];

	//! ���r���X�[�c�̃R���W����
	class OBBCollisionComponent* m_OBBCollision[MOBILESUIT_MAX];
	class AABBCollisionComponent* m_AABBCollision[MOBILESUIT_MAX];

public:
	SceneCollisionManager();
	~SceneCollisionManager();

	/**
	* @brief ����������
	*/
	void Init();

	/**
	* @brief �X�V����
	*/
	void Update();

	/**
	* @brief �I������
	*/
	void Uninit();

	/**
	* @brief ���b�V���t�B�[���h�Z�b�g
	* @param[in] class MeshFieldComponent*(meshField) ���b�V���t�B�[���h�̃|�C���^
	*/
	inline void SetMeshField(class MeshFieldComponent* meshField) { m_MeshFieldComponent = meshField; }

	/**
	* @brief ���r���X�[�c�̏��Z�b�g
	* @param[in] class MeshFieldComponent*(meshField) ���b�V���t�B�[���h�̃|�C���^
	*/
	inline void SetMobileSuitData(MOBILESUIT_NAME mobileSuitName, class MobileSuit* mobileSuit, class  RigidBodyComponent* rigidBodyComponent, class OBBCollisionComponent* obbCollisionComponent, class AABBCollisionComponent* aabbCollisionComponent)
	{
		m_MobileSuits[mobileSuitName] = mobileSuit;
		m_RigidBodyComponent[mobileSuitName] = rigidBodyComponent;
		m_OBBCollision[mobileSuitName] = obbCollisionComponent;
		m_AABBCollision[mobileSuitName] = aabbCollisionComponent;
	}

	/**
	* @brief �e�̃R���W�����R���|�[�l���g�Z�b�g
	* @param[in] class OBBCollisionComponent*(bulletCollision) �e�̃R���W�����R���|�[�l���g�̃|�C���^
	*/
	void SetBulletCollision(class OBBCollisionComponent* bulletCollision);

	/**
	* @brief �r�[���T�[�x���̃R���W�����R���|�[�l���g�Z�b�g
	* @param[in] class OBBCollisionComponent*(enemyCollision) �r�[���T�[�x���̃R���W�����R���|�[�l���g�̃|�C���^
	*/
	void SetBeamSaberCollision(class OBBCollisionComponent* beamSaberCollision);

	/**
	* @brief �G�l�~�[�̒e�̃R���W�����R���|�[�l���g�Z�b�g
	* @param[in] class OBBCollisionComponent*(bulletCollision) �e�̃R���W�����R���|�[�l���g�̃|�C���^
	*/
	void SetEnemyBulletCollision(class OBBCollisionComponent* bulletCollision);

	/**
	* @brief �ق���Q���̃R���W�����R���|�[�l���g�Z�b�g
	* @param[in] class AABBCollisionComponent*(bulletCollision) ��Q���̃R���W�����R���|�[�l���g�̃|�C���^
	*/
	void SetObstacleCollision(class AABBCollisionComponent* obstacleCollision);
};