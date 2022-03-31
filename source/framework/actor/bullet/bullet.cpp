
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

namespace BulletData
{
	//! �f�t�H���g�̑��x
	static constexpr float k_DefaultSpeed = 0.75f;

	//! �f�t�H���g�̏I���t���[��
	static constexpr int k_DefaultFinishFrame = 300;

	//! �ő��]��
	static constexpr float k_DefaultMaxRotate = 0.25f;
}

Bullet::Bullet()
	: m_RigidBodyComponent(nullptr)
	, m_OBBCollisionComponent(nullptr)
	, m_Speed(BulletData::k_DefaultSpeed)
	, m_FrameCount(0)
	, m_FinishFrame(BulletData::k_DefaultFinishFrame)
	, m_TargetMobileSuit(nullptr)
	, m_Direction(0.0f,0.0f,0.0f)
	, m_TargetFlag(false)
	, m_RotationLimit(BulletData::k_DefaultMaxRotate)
	, m_LandingEffectManager(nullptr)
{
	m_ActorType = Actor::ACTORTYPE_PLAYER_BULLET;
}

Bullet::~Bullet()
{
}

void Bullet::SetDirection(const Conv_XM::Vector3f & direction)
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_RigidBodyComponent��nullptr");
		return;
	}

	m_Direction = DirectX::XMVector3Normalize(direction);

	m_RigidBodyComponent->SetDirection(m_Direction * m_Speed);
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
}

void Bullet::ProcessInputActor()
{
	// �ǐՃt���O�������Ă���Ȃ�ǂ�
	if (m_TargetFlag == true)
	{
		TargetChase();
	}
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
	velDir = DirectX::XMVector3Normalize(m_Direction);

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

		m_Direction = DirectX::XMVector3Rotate(m_Direction, NextVecQuaternion);

		m_Direction = DirectX::XMVector3Normalize(m_Direction);

		// ���W�b�h�{�f�B�ɑ��x����͂���
		m_RigidBodyComponent->SetDirection(m_Direction * m_Speed);
	}
}
