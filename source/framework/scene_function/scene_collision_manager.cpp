
#include"../../system/main.h"
#include "scene_collision_manager.h"
#include"../component/mesh_component/mesh_field_component.h"
#include"../actor/actor.h"
#include"../actor/mobilesuit/player.h"
#include"../actor/mobilesuit/enemy.h"
#include"../component/move_component/rigidbody_component.h"
#include"../component/collision_component/obb_collision_component.h"
#include"../component/collision_component/aabb_collision_component.h"
#include"../../math/collision/collision_detection.h"

namespace SceneCollisionManagerData
{
	//! �v���C���[�̒n��̃I�t�Z�b�g�|�W�V����
	constexpr float k_PlayerOffsetHeightPosition = 2.4f;
}

SceneCollisionManager::SceneCollisionManager()
{
}

SceneCollisionManager::~SceneCollisionManager()
{
}

void SceneCollisionManager::Init()
{

}

void SceneCollisionManager::Update()
{
	// �G�l�~�[�̓����蔻��
	for (int i = 0; i < MOBILESUIT_MAX; i++)
	{
		// null�`�F�b�N
		if (m_MobileSuits[i] == nullptr || m_RigidBodyComponent[i] == nullptr || m_OBBCollision[i] == nullptr)
		{
			Logger::GetInstance().SetLog("SceneCollisionManager::Update nullptr");
			continue;
		}

		if (m_MobileSuits[i]->GetActive() == false)
		{
			continue;
		}

		// �|�W�V�����擾
		Conv_XM::Vector3f position = m_MobileSuits[i]->GetPosition();
		// �A�N�^�[�̈ʒu�ɂ��郁�b�V���t�B�[���h�̍������擾
		float height = m_MeshFieldComponent->GetHeight(position);

		// �ڒn���Ă���Ƃ�
		if (m_RigidBodyComponent[i]->GetIsGround() == true)
		{
			// �����ɐڒn���Ă���
			if (m_RigidBodyComponent[i]->GetIsObjectGround() == true)
			{
				// �ڒn���Ă��邩�ǂ���
				bool isGroundFlag = false;
				//�@��Q���̓����蔻��
				for (auto* obstacleObbCollision : m_ObstacleCollisions)
				{
					// null�`�F�b�N
					if (obstacleObbCollision == nullptr)
					{
						Logger::GetInstance().SetLog("SceneCollisionManager::Update obstacleObbCollision��nullptr");
						continue;
					}

					// �eOBB�擾
					Collision::AABBCollision obstacleAABB = obstacleObbCollision->GetAABB();

					// OBB�擾
					Collision::AABBCollision mobileSuitAABB = m_AABBCollision[i]->GetAABB();

					// �����蔻��
					if (Collision::Intersect(obstacleAABB, mobileSuitAABB, nullptr) == true)
					{
						isGroundFlag = true;
					}
				}

				if (isGroundFlag == false)
				{
					m_RigidBodyComponent[i]->SetIsObjectGround(false);
					m_RigidBodyComponent[i]->SetIsGround(false);
				}
			}
			else
			{
				// �n�ʂɏՓ˂��Ă���
				position.y = height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition;
				m_MobileSuits[i]->SetPosition(position);
			}
		}
		else// �ڒn���Ă��Ȃ���
		{
			// �u�[�X�g�_�b�V�����̏ꍇ
			if (m_MobileSuits[i]->GetIsBoostDash() == true)
			{
				// �����ɐڒn���Ă���
				if (m_RigidBodyComponent[i]->GetIsObjectGround() == true)
				{

				}
				else
				{
					if (position.y <= height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition)
					{
						position.y = height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition;
						m_MobileSuits[i]->SetPosition(position);
					}
				}
			}
			else
			{
				//�@��Q���̓����蔻��
				for (auto* obstacleObbCollision : m_ObstacleCollisions)
				{
					// null�`�F�b�N
					if (obstacleObbCollision == nullptr)
					{
						Logger::GetInstance().SetLog("SceneCollisionManager::Update obstacleObbCollision��nullptr");
						continue;
					}

					// �eOBB�擾
					Collision::AABBCollision obstacleAABB = obstacleObbCollision->GetAABB();

					// OBB�擾
					Collision::AABBCollision mobileSuitAABB = m_AABBCollision[i]->GetAABB();

					Conv_XM::Vector3f hitInfo = Conv_XM::Vector3f::Zero;
					// �����蔻��
					if (Collision::Intersect(obstacleAABB, mobileSuitAABB, &hitInfo) == true)
					{
						if (fabs(hitInfo.y) > 0.0f)
						{
							m_RigidBodyComponent[i]->SetIsObjectGround(true);
							m_RigidBodyComponent[i]->SetVelocityY(0.0f);
							m_RigidBodyComponent[i]->SetIsGround(true);
							position.y = obstacleAABB.Max.y + (mobileSuitAABB.Max.y - mobileSuitAABB.Min.y) - 0.3f;
							m_MobileSuits[i]->SetPosition(position);
						}
					}
				}

				// �n�ʂɏՓ˂��Ă���
				if (position.y < height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition)
				{
					m_RigidBodyComponent[i]->SetIsGround(true);
					m_RigidBodyComponent[i]->SetVelocityY(0.0f);
					position.y = height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition;
					m_MobileSuits[i]->SetPosition(position);
				}
			}
		}

		//�@��Q���̓����蔻��
		for (auto* obstacleObbCollision : m_ObstacleCollisions)
		{
			// null�`�F�b�N
			if (obstacleObbCollision == nullptr)
			{
				Logger::GetInstance().SetLog("SceneCollisionManager::Update obstacleObbCollision��nullptr");
				continue;
			}

			// �eOBB�擾
			Collision::AABBCollision obstacleAABB = obstacleObbCollision->GetAABB();

			// OBB�擾
			Collision::AABBCollision mobileSuitAABB = m_AABBCollision[i]->GetAABB();

			Conv_XM::Vector3f hitInfo = Conv_XM::Vector3f::Zero;

			// �����蔻��
			if (Collision::Intersect(obstacleAABB, mobileSuitAABB, &hitInfo) == true)
			{
				if ((fabs(hitInfo.y) > 0.0f) == false && obstacleAABB.Max.y > mobileSuitAABB.Min.y + 0.3f)
				{
					m_MobileSuits[i]->CollisionDetected(nullptr);
				}
			}
		}

		// �v���C���[�������̏ꍇ
		if (i == PLAYER || i == ALLY)
		{
			// �G�l�~�[�̒e�̓����蔻��
			for (auto* enemyBulletCollision : m_EnemyBulletCollisions)
			{
				// null�`�F�b�N
				if (enemyBulletCollision == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update enemyBulletCollision��nullptr");
					continue;
				}

				// �A�N�^�[�擾
				Actor* enemyBullet = enemyBulletCollision->GetOwner();

				// null�`�F�b�N
				if (enemyBullet == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update enemyBullet��nullptr");
					continue;
				}

				// �A�N�e�B�u�`�F�b�N
				if (enemyBullet->GetActive() == false || enemyBulletCollision->GetIsUseFlag() == false)
				{
					continue;
				}

				// �eOBB�擾
				Collision::OBBCollision enemyBulletOBB = enemyBulletCollision->GetOBB();


				// OBB�擾
				Collision::OBBCollision myOBB = m_OBBCollision[i]->GetOBB();

				// �����蔻��
				if (Collision::Intersect(myOBB, enemyBulletOBB) == true)
				{
					m_MobileSuits[i]->CollisionDetected(enemyBullet);
					enemyBullet->CollisionDetected(m_MobileSuits[i]);
				}

				// �n�`�̓����蔻��
				float enemyHeight = m_MeshFieldComponent->GetHeight(enemyBulletOBB.Position);
				if (enemyBulletOBB.Position.y < enemyHeight)
				{
					enemyBullet->SetActive(false);
				}
			}
		}
		else
		{
			// �e
			for (auto* bulletCollision : m_BulletCollisions)
			{
				// null�`�F�b�N
				if (bulletCollision == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update bulletCollision��nullptr");
					continue;
				}

				// �A�N�^�[�擾
				Actor* bullet = bulletCollision->GetOwner();

				// null�`�F�b�N
				if (bullet == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update bulletOwner��nullptr");
					continue;
				}

				// �A�N�e�B�u�`�F�b�N
				if (bullet->GetActive() == false || bulletCollision->GetIsUseFlag() == false)
				{
					continue;
				}

				// �eOBB�擾
				Collision::OBBCollision bulletOBB = bulletCollision->GetOBB();

				// OBB�擾
				Collision::OBBCollision enemyOBB = m_OBBCollision[i]->GetOBB();

				// �����蔻��
				if (Collision::Intersect(enemyOBB, bulletOBB) == true)
				{
					m_MobileSuits[i]->CollisionDetected(bullet);
					bullet->CollisionDetected(m_MobileSuits[i]);
				}


				// �n�`�̓����蔻��
				float enemyHeight = m_MeshFieldComponent->GetHeight(bulletOBB.Position);
				if (bulletOBB.Position.y < enemyHeight)
				{
					bullet->SetActive(false);
				}
			}
		}
	}
}

void SceneCollisionManager::Uninit()
{
}

void SceneCollisionManager::SetBulletCollision(OBBCollisionComponent * bulletCollision)
{
	if (bulletCollision == nullptr)
	{
		Logger::GetInstance().SetLog("SceneCollisionManager::SetBulletCollision bulletCollision��nullptr");
		return;
	}

	m_BulletCollisions.emplace_back(bulletCollision);
}

void SceneCollisionManager::SetBeamSaberCollision(OBBCollisionComponent * beamSaberCollision)
{
	if (beamSaberCollision == nullptr)
	{
		Logger::GetInstance().SetLog("SceneCollisionManager::SetBeamSaberCollision beamSaberCollision��nullptr");
		return;
	}

	m_BeamSaberCollisions.emplace_back(beamSaberCollision);
}

void SceneCollisionManager::SetEnemyBulletCollision(OBBCollisionComponent * bulletCollision)
{
	if (bulletCollision == nullptr)
	{
		Logger::GetInstance().SetLog("SceneCollisionManager::SetEnemyBulletCollision bulletCollision��nullptr");
		return;
	}

	m_EnemyBulletCollisions.emplace_back(bulletCollision);
}

void SceneCollisionManager::SetObstacleCollision(AABBCollisionComponent * obstacleCollision)
{
	if (obstacleCollision == nullptr)
	{
		Logger::GetInstance().SetLog("SceneCollisionManager::SetObstacleCollision obstacleCollision��nullptr");
		return;
	}

	m_ObstacleCollisions.emplace_back(obstacleCollision);
}
