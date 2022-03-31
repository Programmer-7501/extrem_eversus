
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
	//! プレイヤーの地上のオフセットポジション
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
	// エネミーの当たり判定
	for (int i = 0; i < MOBILESUIT_MAX; i++)
	{
		// nullチェック
		if (m_MobileSuits[i] == nullptr || m_RigidBodyComponent[i] == nullptr || m_OBBCollision[i] == nullptr)
		{
			Logger::GetInstance().SetLog("SceneCollisionManager::Update nullptr");
			continue;
		}

		if (m_MobileSuits[i]->GetActive() == false)
		{
			continue;
		}

		// ポジション取得
		Conv_XM::Vector3f position = m_MobileSuits[i]->GetPosition();
		// アクターの位置にあるメッシュフィールドの高さを取得
		float height = m_MeshFieldComponent->GetHeight(position);

		// 接地しているとき
		if (m_RigidBodyComponent[i]->GetIsGround() == true)
		{
			// 建物に接地している
			if (m_RigidBodyComponent[i]->GetIsObjectGround() == true)
			{
				// 接地しているかどうか
				bool isGroundFlag = false;
				//　障害物の当たり判定
				for (auto* obstacleObbCollision : m_ObstacleCollisions)
				{
					// nullチェック
					if (obstacleObbCollision == nullptr)
					{
						Logger::GetInstance().SetLog("SceneCollisionManager::Update obstacleObbCollisionがnullptr");
						continue;
					}

					// 弾OBB取得
					Collision::AABBCollision obstacleAABB = obstacleObbCollision->GetAABB();

					// OBB取得
					Collision::AABBCollision mobileSuitAABB = m_AABBCollision[i]->GetAABB();

					// 当たり判定
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
				// 地面に衝突している
				position.y = height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition;
				m_MobileSuits[i]->SetPosition(position);
			}
		}
		else// 接地していない時
		{
			// ブーストダッシュ中の場合
			if (m_MobileSuits[i]->GetIsBoostDash() == true)
			{
				// 建物に接地している
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
				//　障害物の当たり判定
				for (auto* obstacleObbCollision : m_ObstacleCollisions)
				{
					// nullチェック
					if (obstacleObbCollision == nullptr)
					{
						Logger::GetInstance().SetLog("SceneCollisionManager::Update obstacleObbCollisionがnullptr");
						continue;
					}

					// 弾OBB取得
					Collision::AABBCollision obstacleAABB = obstacleObbCollision->GetAABB();

					// OBB取得
					Collision::AABBCollision mobileSuitAABB = m_AABBCollision[i]->GetAABB();

					Conv_XM::Vector3f hitInfo = Conv_XM::Vector3f::Zero;
					// 当たり判定
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

				// 地面に衝突している
				if (position.y < height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition)
				{
					m_RigidBodyComponent[i]->SetIsGround(true);
					m_RigidBodyComponent[i]->SetVelocityY(0.0f);
					position.y = height + SceneCollisionManagerData::k_PlayerOffsetHeightPosition;
					m_MobileSuits[i]->SetPosition(position);
				}
			}
		}

		//　障害物の当たり判定
		for (auto* obstacleObbCollision : m_ObstacleCollisions)
		{
			// nullチェック
			if (obstacleObbCollision == nullptr)
			{
				Logger::GetInstance().SetLog("SceneCollisionManager::Update obstacleObbCollisionがnullptr");
				continue;
			}

			// 弾OBB取得
			Collision::AABBCollision obstacleAABB = obstacleObbCollision->GetAABB();

			// OBB取得
			Collision::AABBCollision mobileSuitAABB = m_AABBCollision[i]->GetAABB();

			Conv_XM::Vector3f hitInfo = Conv_XM::Vector3f::Zero;

			// 当たり判定
			if (Collision::Intersect(obstacleAABB, mobileSuitAABB, &hitInfo) == true)
			{
				if ((fabs(hitInfo.y) > 0.0f) == false && obstacleAABB.Max.y > mobileSuitAABB.Min.y + 0.3f)
				{
					m_MobileSuits[i]->CollisionDetected(nullptr);
				}
			}
		}

		// プレイヤーか味方の場合
		if (i == PLAYER || i == ALLY)
		{
			// エネミーの弾の当たり判定
			for (auto* enemyBulletCollision : m_EnemyBulletCollisions)
			{
				// nullチェック
				if (enemyBulletCollision == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update enemyBulletCollisionがnullptr");
					continue;
				}

				// アクター取得
				Actor* enemyBullet = enemyBulletCollision->GetOwner();

				// nullチェック
				if (enemyBullet == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update enemyBulletがnullptr");
					continue;
				}

				// アクティブチェック
				if (enemyBullet->GetActive() == false || enemyBulletCollision->GetIsUseFlag() == false)
				{
					continue;
				}

				// 弾OBB取得
				Collision::OBBCollision enemyBulletOBB = enemyBulletCollision->GetOBB();


				// OBB取得
				Collision::OBBCollision myOBB = m_OBBCollision[i]->GetOBB();

				// 当たり判定
				if (Collision::Intersect(myOBB, enemyBulletOBB) == true)
				{
					m_MobileSuits[i]->CollisionDetected(enemyBullet);
					enemyBullet->CollisionDetected(m_MobileSuits[i]);
				}

				// 地形の当たり判定
				float enemyHeight = m_MeshFieldComponent->GetHeight(enemyBulletOBB.Position);
				if (enemyBulletOBB.Position.y < enemyHeight)
				{
					enemyBullet->SetActive(false);
				}
			}
		}
		else
		{
			// 弾
			for (auto* bulletCollision : m_BulletCollisions)
			{
				// nullチェック
				if (bulletCollision == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update bulletCollisionがnullptr");
					continue;
				}

				// アクター取得
				Actor* bullet = bulletCollision->GetOwner();

				// nullチェック
				if (bullet == nullptr)
				{
					Logger::GetInstance().SetLog("SceneCollisionManager::Update bulletOwnerがnullptr");
					continue;
				}

				// アクティブチェック
				if (bullet->GetActive() == false || bulletCollision->GetIsUseFlag() == false)
				{
					continue;
				}

				// 弾OBB取得
				Collision::OBBCollision bulletOBB = bulletCollision->GetOBB();

				// OBB取得
				Collision::OBBCollision enemyOBB = m_OBBCollision[i]->GetOBB();

				// 当たり判定
				if (Collision::Intersect(enemyOBB, bulletOBB) == true)
				{
					m_MobileSuits[i]->CollisionDetected(bullet);
					bullet->CollisionDetected(m_MobileSuits[i]);
				}


				// 地形の当たり判定
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
		Logger::GetInstance().SetLog("SceneCollisionManager::SetBulletCollision bulletCollisionがnullptr");
		return;
	}

	m_BulletCollisions.emplace_back(bulletCollision);
}

void SceneCollisionManager::SetBeamSaberCollision(OBBCollisionComponent * beamSaberCollision)
{
	if (beamSaberCollision == nullptr)
	{
		Logger::GetInstance().SetLog("SceneCollisionManager::SetBeamSaberCollision beamSaberCollisionがnullptr");
		return;
	}

	m_BeamSaberCollisions.emplace_back(beamSaberCollision);
}

void SceneCollisionManager::SetEnemyBulletCollision(OBBCollisionComponent * bulletCollision)
{
	if (bulletCollision == nullptr)
	{
		Logger::GetInstance().SetLog("SceneCollisionManager::SetEnemyBulletCollision bulletCollisionがnullptr");
		return;
	}

	m_EnemyBulletCollisions.emplace_back(bulletCollision);
}

void SceneCollisionManager::SetObstacleCollision(AABBCollisionComponent * obstacleCollision)
{
	if (obstacleCollision == nullptr)
	{
		Logger::GetInstance().SetLog("SceneCollisionManager::SetObstacleCollision obstacleCollisionがnullptr");
		return;
	}

	m_ObstacleCollisions.emplace_back(obstacleCollision);
}
