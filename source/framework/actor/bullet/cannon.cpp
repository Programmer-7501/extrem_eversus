
#include"../../../system/main.h"
#include "cannon.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../../particle/particle_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"

namespace CannonData
{
	//! �f�t�H���g�̑��x
	static constexpr float k_DefaultSpeed = 1.0f;

	//! �e�̐F
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.8f, 0.2f, 1.0f);
	//! 1�t���[�����ɔ���������p�[�e�B�N����
	static constexpr int k_DefaultSpawnNum = 10;
	//! ���C�t
	static constexpr float k_DefaultLife = 250.0f;
	//! �p�[�e�B�N���T�C�Y
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(5.0f, 5.0f);
	//! �|�W�V�����̂΂��
	static const Conv_XM::Vector3f k_PositionVariate = Conv_XM::Vector3f(0.1f, 0.1f, 0.1f);
	//! �g�p�G�~�b�^�[�^�C�v
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_RECT;
}

Cannon::Cannon()
	:m_UseParticleName(ParticleManagerData::k_BeamParticleName)
{
	m_ActorType = Actor::ACTORTYPE_CANNON;
}

Cannon::~Cannon()
{
}

void Cannon::SetActive(bool flag)
{
	// ����
	m_Active = flag;
	// �J�E���g������
	m_FrameCount = 0;

	// ��A�N�e�B�u����
	if (m_Active == false)
	{
		return;
	}

	// �|�W�V��������
	m_Emitter.EmitterPosition = m_Position;
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter.MyIndex, m_Emitter);
	particleManager.SpawnParticle(m_UseParticleName, m_Emitter.MyIndex);
}

void Cannon::SetDirection(const Conv_XM::Vector3f & direction)
{
	m_Emitter.ParticleVelocity = direction;
	m_Direction = direction;
	m_Direction = DirectX::XMVector3Normalize(m_Direction);

	Conv_XM::Vector4f q = Conv_XM::Vector4f::MakeQuaternionFromVec3(m_Direction);
	SetQuaternion(q);
	ComputeWorldTransform();
}

void Cannon::LoadActor()
{
	// OBB�R���W�����R���|�[�l���g�쐬
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_OBBCollisionComponent��nullptr");
		return;
	}

	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* currentScene = sceneManager.GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor currentScene��nullptr");
		return;
	}
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor sceneCollisionManager��nullptr");
		return;
	}
	//�@�R���W�����̃Z�b�g
	sceneCollisionManager->SetBulletCollision(m_OBBCollisionComponent);

	// OBB�f�o�b�O�r���[�쐬
	CreateComponent<OBBDebugViewComponent>();
}

void Cannon::InitActor()
{
	m_Emitter.Color = CannonData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = CannonData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = CannonData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = CannonData::k_DefaultLife;
	m_Emitter.Size = CannonData::k_ParticleSize;
	m_Emitter.Speed = CannonData::k_DefaultSpeed;
	m_Emitter.PositionVariance = CannonData::k_PositionVariate;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �G�~�b�^�[��o�^
	m_Emitter.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void Cannon::ProcessInputActor()
{
	//�@�p�[�e�B�N���𔭐�������
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.SpawnParticle(m_UseParticleName, m_Emitter.MyIndex);
}

void Cannon::UpdateActor()
{
	// �t���[���J�E���g���I���t���[���𒴂��Ă���
	if (m_FrameCount > m_FinishFrame)
	{
		// ��A�N�e�B�u��Ԃɂ���
		SetActive(false);
	}

	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Cannon::UpdateActor nullptr");
		return;
	}
	Collision::OBBCollision obb = m_OBBCollisionComponent->GetOBB();

	Conv_XM::Vector3f offsetPosition = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldForward, m_Quaternion);

	// OBB�̒��S�����߂�
	Conv_XM::Vector3f center = m_Position + ((offsetPosition * static_cast<float>(m_FrameCount)) / 2.0f);
	obb.Position = center;
	m_OBBCollisionComponent->SetOffsetTransform(Conv_XM::Vector3f(0.0f,0.0f, static_cast<float>(m_FrameCount) / 2.0f), Conv_XM::Vector3f(3.0f,3.0f, static_cast<float>(m_FrameCount)), DirectX::XMQuaternionIdentity());
	// ���W�X�V������
	m_OBBCollisionComponent->OnUpdateWorldTransform();

	// �J�E���g�A�b�v
	m_FrameCount++;
}

void Cannon::CollisionDetected(class Actor* coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);
}
