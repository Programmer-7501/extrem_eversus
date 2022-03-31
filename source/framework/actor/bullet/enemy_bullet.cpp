
#include"../../../system/main.h"
#include "enemy_bullet.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"


EnemyBullet::EnemyBullet()
{
	m_ActorType = Actor::ACTORTYPE_ENEMY_BULLET;
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::LoadActor()
{
	// リジッドボディコンポーネント作成
	m_RigidBodyComponent = CreateComponent<RigidBodyComponent>();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor m_RigidBodyComponentがnullptr");
		return;
	}

	// OBBコリジョンコンポーネント作成
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor m_OBBCollisionComponentがnullptr");
		return;
	}

	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* currentScene = sceneManager.GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor currentSceneがnullptr");
		return;
	}
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor sceneCollisionManagerがnullptr");
		return;
	}
	//　コリジョンのセット
	sceneCollisionManager->SetEnemyBulletCollision(m_OBBCollisionComponent);

	// OBBデバッグビュー作成
	CreateComponent<OBBDebugViewComponent>();
}
