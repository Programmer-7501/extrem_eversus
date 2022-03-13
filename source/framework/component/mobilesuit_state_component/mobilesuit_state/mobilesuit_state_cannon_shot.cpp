
#include"../../../../system/main.h"
#include "mobilesuit_state_cannon_shot.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor/bullet/cannon.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../mesh_component/animation_model_component.h"


namespace MobileSuitStateCannonShotData
{
	//! �L���m����ł܂ł̃J�E���g
	constexpr int k_DefaultCannonShotCount = 10;

	//! �L���m�����I����J�E���g
	constexpr int k_DefaultCannonShotFinishCount = 90;

	//! �r�[�����C�t����ł���̑S�̃t���[��
	constexpr int k_DefaultCannonShotOverallFrame = 120;

	//! �u�[�X�g�_�b�V�����͎�t����
	constexpr int k_BoostDashInputTime = 12;

	//! �u�[�X�g�_�b�V������
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! �u�[�X�g�_�b�V�����̃u�[�X�g�g�p��
	constexpr float k_BoostDashUseBoost = 20.0f;

	//! ��t���[���ɂ�����u�[�X�g�g�p��
	constexpr float k_CannonUseBoostNum = 2.0f;
}

MobileSuitStateCannonShot::MobileSuitStateCannonShot(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_BoostDashInitialSpeed(MobileSuitStateCannonShotData::k_BoostDashInitialSpeed)
	, m_BoostDashCount(MobileSuitStateCannonShotData::k_BoostDashInputTime + 1)
	, m_FrameCount(0)
	, m_IsShotFlag(false)
	, m_CannonVelocity(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f))
{
}

void MobileSuitStateCannonShot::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("StateJump::Init GetData�����s");
		return;
	}

	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::Init m_MobileSuit��nullptr");
		return;
	}
}

void MobileSuitStateCannonShot::ProcessInput()
{
	if (m_MobileSuit == nullptr || m_Owner == nullptr || m_LockOnCameraComponent == nullptr || m_RigidBodyComponent == nullptr || m_CannonBulletManager == nullptr || m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::ProcessInput nullptr");
		return;
	}

	if (m_FrameCount > 40)
	{
		m_AnimationComponent->StopModel(true, 40);
	}


	// �r�[�����C�t����łJ�E���g�ɂȂ�����
	if (m_FrameCount > MobileSuitStateCannonShotData::k_DefaultCannonShotCount && m_IsShotFlag == false)
	{
		// �G�l�~�[�Ɍ�����
		RotateToEnemy();

		// �r�[����ł�
		if (m_TargetNum == 0)
		{
			m_CannonBullet = m_CannonBulletManager->UseBullet(m_EnemyMobileSuit001, m_MobileSuit->GetPosition(), m_CannonVelocity);
		}
		else
		{
			m_CannonBullet = m_CannonBulletManager->UseBullet(m_EnemyMobileSuit002, m_MobileSuit->GetPosition(), m_CannonVelocity);
		}

		// �V���b�g���������t���O�𗧂Ă�
		m_IsShotFlag = true;
	}

	// �L���m���̓���̃t���[�����𒴂�����
	if (m_FrameCount > MobileSuitStateCannonShotData::k_DefaultCannonShotOverallFrame)
	{
		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}
	}


	//! AI�̏ꍇ
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		InputManager& inputManager = InputManager::GetInstance();

		// �u�[�X�g�_�b�V�����͑ҋ@��ԂōĂуu�[�X�g�_�b�V�����͂����ꂽ��
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount <= MobileSuitStateCannonShotData::k_BoostDashInputTime)
		{
			// �u�[�X�g�_�b�V���ɃL�����Z��
			// �J�����̑O�x�N�g�����擾
			Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
			forward.y = 0.0f;
			forward = DirectX::XMVector3Normalize(forward);

			// y���ŉE��90�x��]�������N�H�[�^�j�I�����쐬
			Conv_XM::Vector4f q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
			Conv_XM::Vector3f right = DirectX::XMVector3Rotate(forward, q1);

			// ���r���X�[�c�A�N�^�[�̑O�x�N�g�����擾
			Conv_XM::Vector3f myForward = m_MobileSuit->GetForward();
			myForward.y = 0.0f;
			myForward = DirectX::XMVector3Normalize(myForward);

			if (inputManager.GetIsPress(InputManager::IN_FRONT))
			{
				RotationActor(forward, myForward, FORWARD);
				m_RigidBodyComponent->PlusVelocity(forward * m_BoostDashInitialSpeed);
			}
			else if (inputManager.GetIsPress(InputManager::IN_BACK))
			{
				RotationActor(forward, myForward, BACK);
				m_RigidBodyComponent->PlusVelocity(forward * (-m_BoostDashInitialSpeed));
			}
			else if (inputManager.GetIsPress(InputManager::IN_RIGHT))
			{
				RotationActor(forward, myForward, RIGHT);
				m_RigidBodyComponent->PlusVelocity(right * m_BoostDashInitialSpeed);
			}
			else if (inputManager.GetIsPress(InputManager::IN_LEFT))
			{
				RotationActor(forward, myForward, LEFT);
				m_RigidBodyComponent->PlusVelocity(right * (-m_BoostDashInitialSpeed));
			}

			m_MobileSuit->SetIsBoostDash(true);
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
			m_BoostComponent->UseBoost(MobileSuitStateCannonShotData::k_BoostDashUseBoost);
		}

		// �u�[�X�g�_�b�V�����͂��Ȃ���ԂŃW�����v�{�^���������ꂽ��
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount > MobileSuitStateCannonShotData::k_BoostDashInputTime)
		{
			// �J�E���g���[���ɂ���
			m_BoostDashCount = 0;
		}
	}

	// �󒆂Ȃ�
	if (m_RigidBodyComponent->GetIsGround() == false)
	{
		// �u�[�X�g���g�p����
		m_BoostComponent->UseBoost(MobileSuitStateCannonShotData::k_CannonUseBoostNum);
	}
}

void MobileSuitStateCannonShot::Update()
{
	// �J�E���g�A�b�v
	m_FrameCount++;
	m_BoostDashCount++;
}

void MobileSuitStateCannonShot::OnEnter()
{
	// �J�E���g������
	m_FrameCount = 0;
	// �V���b�g�t���O������
	m_IsShotFlag = false;
	// �u�[�X�g�_�b�V�����͎��ԏ�����
	m_BoostDashCount = MobileSuitStateCannonShotData::k_BoostDashInputTime + 1;

	if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr || m_RigidBodyComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnEnter nullptr");
		return;
	}
	// �ǂ����_���Ă��邩
	m_TargetNum = m_MobileSuit->GetTargetNumber();

	if (m_TargetNum == 0)
	{
		// �����Ƃ�
		m_CannonVelocity = m_EnemyMobileSuit001->GetPosition() - m_MobileSuit->GetPosition();
		// ���K������
		m_CannonVelocity = DirectX::XMVector3Normalize(m_CannonVelocity);
	}
	else
	{
		// �����Ƃ�
		m_CannonVelocity = m_EnemyMobileSuit002->GetPosition() - m_MobileSuit->GetPosition();
		// ���K������
		m_CannonVelocity = DirectX::XMVector3Normalize(m_CannonVelocity);
	}


	// ���x��0�ɂ���
	m_RigidBodyComponent->SetVelocity(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	m_RigidBodyComponent->SetIsUseGravity(false);

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("cannon", 10);
}

void MobileSuitStateCannonShot::OnExit()
{
	// �J�E���g������
	m_FrameCount = 0;
	// �V���b�g�t���O������
	m_IsShotFlag = false;
	// �u�[�X�g�_�b�V�����͎��ԏ�����
	m_BoostDashCount = MobileSuitStateCannonShotData::k_BoostDashInputTime + 1;

	// �d�͂��I���ɂ���
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}
	m_RigidBodyComponent->SetIsUseGravity(true);

	// �O�Ɍ�����
	RotateToForward();

	if (m_CannonBullet == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}
	// ��A�N�e�B�u��Ԃɂ���
	m_CannonBullet->SetActive(false);

	m_AnimationComponent->StopModel(false);
}

void MobileSuitStateCannonShot::RotateToEnemy()
{
	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}

	//Conv_XM::Vector3f actorForward = m_MobileSuit->GetForward();
	//���ςƊp�x���Z�o
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, m_CannonVelocity);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(Conv_XM::Vector3f::WorldForward, m_CannonVelocity);
	float angle = xmangle.x;
	
	Conv_XM::Vector4f NextVecQuaternion;

	// �������p�Ȃ�
	if (dot > 0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionIdentity();
	}
	else if (dot < -0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis((DirectX::XMVECTOR)Conv_XM::Vector3f::WorldUp, DirectX::XM_PI);
	}
	else
	{
		//�O�ς����߉�]�������߂�
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, m_CannonVelocity);

		//�N�H�[�^�j�I�������߂�
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	//���߂��N�H�[�^�j�I������������
	Conv_XM::Vector4f myQuaternion = m_MobileSuit->GetQuaternion();
	Conv_XM::Vector4f q = DirectX::XMQuaternionMultiply(myQuaternion, NextVecQuaternion);
	m_MobileSuit->SetQuaternion(NextVecQuaternion);
}

void MobileSuitStateCannonShot::RotateToForward()
{
	if (m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}

	Conv_XM::Vector3f nextVec = m_LockOnCameraComponent->GetForward();
	nextVec.y = 0.0f;
	nextVec = DirectX::XMVector3Normalize(nextVec);

	//���ςƊp�x���Z�o
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, nextVec);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(Conv_XM::Vector3f::WorldForward, nextVec);
	float angle = xmangle.x;

	Conv_XM::Vector4f NextVecQuaternion;

	// �������p�Ȃ�
	if (dot > 0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionIdentity();
	}
	else if (dot < -0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis((DirectX::XMVECTOR)Conv_XM::Vector3f::WorldUp, DirectX::XM_PI);
	}
	else
	{
		//�O�ς����߉�]�������߂�
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, nextVec);

		//�N�H�[�^�j�I�������߂�
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	//���߂��N�H�[�^�j�I�����Z�b�g����
	m_MobileSuit->SetQuaternion(NextVecQuaternion);
}
