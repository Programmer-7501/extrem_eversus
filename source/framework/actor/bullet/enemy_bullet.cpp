
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
	// ���W�b�h�{�f�B�R���|�[�l���g�쐬
	m_RigidBodyComponent = CreateComponent<RigidBodyComponent>();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor m_RigidBodyComponent��nullptr");
		return;
	}

	// OBB�R���W�����R���|�[�l���g�쐬
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor m_OBBCollisionComponent��nullptr");
		return;
	}

	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* currentScene = sceneManager.GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor currentScene��nullptr");
		return;
	}
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("EnemyBullet::InitActor sceneCollisionManager��nullptr");
		return;
	}
	//�@�R���W�����̃Z�b�g
	sceneCollisionManager->SetEnemyBulletCollision(m_OBBCollisionComponent);

	// OBB�f�o�b�O�r���[�쐬
	CreateComponent<OBBDebugViewComponent>();
}
