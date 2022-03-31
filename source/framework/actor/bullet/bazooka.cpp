
#include"../../../system/main.h"
#include "bazooka.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../../particle/particle_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/model_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../../directx/resource/model_manager.h"
#include"../../actor_manager/effect_manager.h"
#include"bullet_data.h"

void Bazooka::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();
	modelManager.Load(BazookaData::k_ModelFileName);
}

void Bazooka::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();
	modelManager.Unload(BazookaData::k_ModelFileName);
}

Bazooka::Bazooka()
{
}

Bazooka::~Bazooka()
{
}

void Bazooka::LoadActor()
{
	// バレットの読み込み処理
	Bullet::LoadActor();

	// モデルコンポーネント作成
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void Bazooka::InitActor()
{
	// バレットの初期化処理
	Bullet::InitActor();

	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bazooka::InitActor m_ModelComponentがnullptr");
		return;
	}

	// モデルの設定
	ModelManager& modelManager = ModelManager::GetInstance();
	Model* bazookaModel = modelManager.GetModel(BazookaData::k_ModelFileName);
	if (bazookaModel == nullptr)
	{
		Logger::GetInstance().SetLog("Bazooka::InitActor bazookaModelがnullptr");
		return;
	}

	m_ModelComponent->SetModel(bazookaModel);
	m_ModelComponent->SetOffsetScale(BazookaData::k_OffsetModelScale);
}

void Bazooka::ProcessInputActor()
{
	// バレットの入力処理
	Bullet::ProcessInputActor();

	// モデルを回転させる
	Conv_XM::Vector4f quaternion = Conv_XM::Vector4f::MakeQuaternionFromVec3(m_Direction);
	SetQuaternion(quaternion);
}

void Bazooka::UpdateActor()
{
	// バレットの更新処理
	Bullet::UpdateActor();
}

void Bazooka::CollisionDetected(Actor * coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);

	// 非アクティブ状態にする
	SetActive(false);

	// 着弾エフェクトを発生させる
	if (m_LandingEffectManager == nullptr)
	{
		Logger::GetInstance().SetLog("BeamRifle::CollisionDetected m_LandingEffectManagerがnullptr");
		return;
	}
	m_LandingEffectManager->UseEffect(m_Position);
}
