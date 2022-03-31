
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
	// �o���b�g�̓ǂݍ��ݏ���
	Bullet::LoadActor();

	// ���f���R���|�[�l���g�쐬
	m_ModelComponent = CreateComponent<ModelComponent>();
}

void Bazooka::InitActor()
{
	// �o���b�g�̏���������
	Bullet::InitActor();

	if (m_ModelComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bazooka::InitActor m_ModelComponent��nullptr");
		return;
	}

	// ���f���̐ݒ�
	ModelManager& modelManager = ModelManager::GetInstance();
	Model* bazookaModel = modelManager.GetModel(BazookaData::k_ModelFileName);
	if (bazookaModel == nullptr)
	{
		Logger::GetInstance().SetLog("Bazooka::InitActor bazookaModel��nullptr");
		return;
	}

	m_ModelComponent->SetModel(bazookaModel);
	m_ModelComponent->SetOffsetScale(BazookaData::k_OffsetModelScale);
}

void Bazooka::ProcessInputActor()
{
	// �o���b�g�̓��͏���
	Bullet::ProcessInputActor();

	// ���f������]������
	Conv_XM::Vector4f quaternion = Conv_XM::Vector4f::MakeQuaternionFromVec3(m_Direction);
	SetQuaternion(quaternion);
}

void Bazooka::UpdateActor()
{
	// �o���b�g�̍X�V����
	Bullet::UpdateActor();
}

void Bazooka::CollisionDetected(Actor * coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);

	// ��A�N�e�B�u��Ԃɂ���
	SetActive(false);

	// ���e�G�t�F�N�g�𔭐�������
	if (m_LandingEffectManager == nullptr)
	{
		Logger::GetInstance().SetLog("BeamRifle::CollisionDetected m_LandingEffectManager��nullptr");
		return;
	}
	m_LandingEffectManager->UseEffect(m_Position);
}
