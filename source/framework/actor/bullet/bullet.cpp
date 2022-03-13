
#include"../../../system/main.h"
#include "bullet.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../../particle/particle_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../actor_manager/effect_manager.h"

namespace BulletData
{
	//! �f�t�H���g�̑��x
	static constexpr float k_DefaultSpeed = 0.75f;

	//! �f�t�H���g�̏I���t���[��
	static constexpr int k_DefaultFinishFrame = 300;

	//! �e�̐F
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.8f, 1.0f);
	//! 1�t���[�����ɔ���������p�[�e�B�N����
	static constexpr int k_DefaultSpawnNum = 2;
	//! ���C�t
	static constexpr float k_DefaultLife = 5.0f;
	//! �p�[�e�B�N���T�C�Y
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(1.0f, 1.0f);
	//! �g�p�G�~�b�^�[�^�C�v
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_SPHERE;

	//! �ő��]��
	static constexpr float k_MaxRotate = 0.25f;
}

Bullet::Bullet()
	: m_RigidBodyComponent(nullptr)
	, m_OBBCollisionComponent(nullptr)
	, m_Speed(BulletData::k_DefaultSpeed)
	, m_Emitter{}
	, m_ParticleEmitterIndex(-1)
	, m_FrameCount(0)
	, m_FinishFrame(BulletData::k_DefaultFinishFrame)
	, m_LandingEffectManager(nullptr)
	, m_TargetMobileSuit(nullptr)
	, m_Velocity(0.0f,0.0f,0.0f)
	, m_TargetFlag(false)
	, m_RotationLimit(BulletData::k_MaxRotate)
{
	m_ActorType = Actor::ACTORTYPE_PLAYER_BULLET;
}

Bullet::~Bullet()
{
}

void Bullet::SetVelocity(const Conv_XM::Vector3f & direction)
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_RigidBodyComponent��nullptr");
		return;
	}

	m_Velocity = DirectX::XMVector3Normalize(direction);

	m_RigidBodyComponent->SetVelocity(m_Velocity * m_Speed);
}

void Bullet::SetActive(bool flag)
{
	// ����
	m_Active = flag;

	if (m_Active == false)
	{
		m_FrameCount = 0;
		m_TargetFlag = false;
		m_TargetMobileSuit = nullptr;
	}
}

void Bullet::LoadActor()
{
	// ���W�b�h�{�f�B�R���|�[�l���g�쐬
	m_RigidBodyComponent = CreateComponent<RigidBodyComponent>();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_RigidBodyComponent��nullptr");
		return;
	}

	// OBB�R���W�����R���|�[�l���g�쐬
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_OBBCollisionComponent��nullptr");
		return;
	}
	m_OBBCollisionComponent->SetOffsetScale(Conv_XM::Vector3f(0.5f, 0.5f, 0.5f));

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

void Bullet::InitActor()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor nullptr");
		return;
	}

	m_RigidBodyComponent->SetGravity(0.0f);
	m_RigidBodyComponent->SetAirFriction(0.0f);
	m_RigidBodyComponent->SetIsGround(false);

	m_Emitter.Color = BulletData::k_DefaultParticleColor;
	m_Emitter.EmitterPosition = m_Position;
	m_Emitter.EmitterType = BulletData::k_ParticleEmitterType;
	m_Emitter.IsUse = 1;
	m_Emitter.MaxSpawnParticlesThisFrame = BulletData::k_DefaultSpawnNum;
	m_Emitter.ParticleLifeSpan = BulletData::k_DefaultLife;
	m_Emitter.Size = BulletData::k_ParticleSize;
	m_Emitter.Speed = 0.001f;

	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �G�~�b�^�[��o�^
	m_ParticleEmitterIndex = particleManager.RegisterParticleEmitter(m_Emitter);
}

void Bullet::ProcessInputActor()
{
	// �ǐՃt���O�������Ă���Ȃ�ǂ�
	if (m_TargetFlag == true)
	{
		TargetChase();
	}

	// �p�[�e�B�N���𐶐�������
	m_Emitter.EmitterPosition = m_Position;
	ParticleManager& particleManager = ParticleManager::GetInstance();
	particleManager.UpdateParticleEmitterConstantBuffer(m_ParticleEmitterIndex, m_Emitter);
	particleManager.SpawnParticle(m_UseParticleName, m_ParticleEmitterIndex);
}

void Bullet::UpdateActor()
{
	// �t���[���J�E���g���I���t���[���𒴂��Ă���
	if (m_FinishFrame < m_FrameCount)
	{
		// ��A�N�e�B�u��Ԃɂ���
		SetActive(false);
	}
	//�@�J�E���g�A�b�v
	m_FrameCount++;
}

void Bullet::CollisionDetected(class Actor* coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);

	// ��A�N�e�B�u��Ԃɂ���
	SetActive(false);

	// ���e�G�t�F�N�g�𔭐�������
	if (m_LandingEffectManager == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::CollisionDetected m_LandingEffectManager��nullptr");
		return;
	}
	m_LandingEffectManager->UseEffect(m_Position);
}

void Bullet::TargetChase()
{
	if (m_TargetFlag == false || m_TargetMobileSuit == nullptr)
	{
		return;
	}

	// �^�[�Q�b�g�̃|�W�V�����擾
	Conv_XM::Vector3f targetPos = m_TargetMobileSuit->GetPosition();

	// ��������^�[�Q�b�g�̒P�ʃx�N�g���擾
	Conv_XM::Vector3f myPosToTargetVec = targetPos - m_Position;
	myPosToTargetVec = DirectX::XMVector3Normalize(myPosToTargetVec);

	// ���x�̒P�ʃx�N�g���擾
	Conv_XM::Vector3f velDir;
	velDir = DirectX::XMVector3Normalize(m_Velocity);

	//0�x�N�g�����ǂ����G���[�`�F�b�N
	float error = Conv_XM::Vector3f::LengthSq(myPosToTargetVec);
	if (error < Math::k_Epsilon)
	{
		return;
	}

	//���ςƊp�x���Z�o
	float dot = Conv_XM::Vector3f::Dot(velDir, myPosToTargetVec);

	if (dot < 0.0f)
	{
		m_TargetFlag = false;
	}
	else
	{
		Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(velDir, myPosToTargetVec);
		float angle = xmangle.x;

		Conv_XM::Vector4f NextVecQuaternion;

		// �������p�Ȃ�
		if (dot > 0.9999f)
		{
			NextVecQuaternion = DirectX::XMQuaternionIdentity();
		}
		else
		{
			if (fabsf(angle) > DirectX::XMConvertToRadians(m_RotationLimit))
			{
				angle = DirectX::XMConvertToRadians(m_RotationLimit);
			}
			//�O�ς����߉�]�������߂�
			Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(velDir, myPosToTargetVec);

			// ���̃x�N�g���ƃA�N�^�[�̃x�N�g������������Ȃ�
			if (Conv_XM::Vector3f::LengthSq(axis) < Math::k_Epsilon)
			{
				NextVecQuaternion = DirectX::XMQuaternionIdentity();
			}
			else
			{
				//�N�H�[�^�j�I�������߂�
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
			}
		}

		m_Velocity = DirectX::XMVector3Rotate(m_Velocity, NextVecQuaternion);

		m_Velocity = DirectX::XMVector3Normalize(m_Velocity);

		// ���W�b�h�{�f�B�ɑ��x����͂���
		m_RigidBodyComponent->SetVelocity(m_Velocity * m_Speed);
	}
}
