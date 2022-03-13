
#include"../../../../system/main.h"
#include "mobilesuit_state_beamrifle_shot.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"

namespace MobileSuitStateBeamRifleShotData
{
	//! �r�[�����C�t����ł܂ł̃J�E���g
	constexpr int k_DefaultBeamRifleShotCount = 10;

	//! �r�[�����C�t����ł���̑S�̃t���[��
	constexpr int k_DefaultBeamRifleShotOverallFrame = 40;

	//! �u�[�X�g�_�b�V�����͎�t����
	constexpr int k_BoostDashInputTime = 12;

	//! �u�[�X�g�_�b�V������
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! �f�t�H���g�̕�������
	constexpr float k_DefaultWalkSpeed = 0.02f;

	//! �f�t�H���g�̉�]���(degree)
	constexpr float k_DefaultRotationLimit = 2.5f;
}

MobileSuitStateBeamRifleShot::MobileSuitStateBeamRifleShot(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_WalkSpeed(MobileSuitStateBeamRifleShotData::k_DefaultWalkSpeed)
	, m_BoostDashInitialSpeed(MobileSuitStateBeamRifleShotData::k_BoostDashInitialSpeed)
	, m_BoostDashCount(MobileSuitStateBeamRifleShotData::k_BoostDashInputTime + 1)
	, m_FrameCount(0)
	, m_IsShotFlag(false)
	, m_EnemyMobileSuit001(nullptr)
	, m_EnemyMobileSuit002(nullptr)
	, m_TargetNum(0)
	, m_BeamVelocity(Conv_XM::Vector3f(0.0f,0.0f,0.0f))
{

}

void MobileSuitStateBeamRifleShot::Init()
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

	// �G�l�~�[�����
	m_EnemyMobileSuit001 = m_MobileSuit->GetEnemyMobileSuit001();
	m_EnemyMobileSuit002 = m_MobileSuit->GetEnemyMobileSuit002();
}

void MobileSuitStateBeamRifleShot::ProcessInput()
{
	if (m_MobileSuit == nullptr || m_Owner == nullptr || m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::ProcessInput nullptr");
		return;
	}

	// �J�����̑O�x�N�g�����擾
	Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
	forward.y = 0.0f;
	forward = DirectX::XMVector3Normalize(forward);

	// �J�����̉E�x�N�g�����擾
	Conv_XM::Vector3f right = m_LockOnCameraComponent->GetRight();
	right.y = 0.0f;
	right = DirectX::XMVector3Normalize(right);

	// ���r���X�[�c�A�N�^�[�̑O�x�N�g�����擾
	Conv_XM::Vector3f myForward = m_MobileSuit->GetForward();

	// �r�[�����C�t����łJ�E���g�ɂȂ�����
	if (m_FrameCount > MobileSuitStateBeamRifleShotData::k_DefaultBeamRifleShotCount && m_IsShotFlag == false)
	{
		// �r�[����ł�
		if (m_TargetNum == 0)
		{
			m_BeamRifleBulletManager->UseBullet(m_EnemyMobileSuit001, m_MobileSuit->GetPosition(), m_BeamVelocity);
		}
		else
		{
			m_BeamRifleBulletManager->UseBullet(m_EnemyMobileSuit002, m_MobileSuit->GetPosition(), m_BeamVelocity);
		}

		// �V���b�g���������t���O�𗧂Ă�
		m_IsShotFlag = true;
	}

	// �r�[�����C�t���̓���̃t���[�����𒴂�����
	if (m_FrameCount > MobileSuitStateBeamRifleShotData::k_DefaultBeamRifleShotOverallFrame)
	{
		m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
	}


	//! AI�̏ꍇ
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		InputManager& inputManager = InputManager::GetInstance();

		// �u�[�X�g�_�b�V�����͑ҋ@��ԂōĂуu�[�X�g�_�b�V�����͂����ꂽ��
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount <= MobileSuitStateBeamRifleShotData::k_BoostDashInputTime)
		{
			// �u�[�X�g�_�b�V���ɃL�����Z��
			m_RigidBodyComponent->PlusVelocity(m_BoostDashInitialSpeed * forward);
			m_MobileSuit->SetIsBoostDash(true);
			// �u�[�X�g�_�b�V����Ԃɂ���
			m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
		}

		// �u�[�X�g�_�b�V�����͂��Ȃ���ԂŃW�����v�{�^���������ꂽ��
		if (inputManager.GetIsTrigger(InputManager::IN_JUMP) && m_BoostDashCount > MobileSuitStateBeamRifleShotData::k_BoostDashInputTime)
		{
			// �J�E���g���[���ɂ���
			m_BoostDashCount = 0;
		}

		if (inputManager.GetIsPress(InputManager::IN_FRONT))
		{
			RotationActor(forward, myForward, FORWARD, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(forward * m_WalkSpeed);
		}
		else if (inputManager.GetIsPress(InputManager::IN_BACK))
		{
			RotationActor(forward, myForward, BACK, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(forward * (-m_WalkSpeed));
		}
		else if (inputManager.GetIsPress(InputManager::IN_RIGHT))
		{
			RotationActor(forward, myForward, RIGHT, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(right * m_WalkSpeed);
		}
		else if (inputManager.GetIsPress(InputManager::IN_LEFT))
		{
			RotationActor(forward, myForward, LEFT, MobileSuitStateBeamRifleShotData::k_DefaultRotationLimit);
			m_RigidBodyComponent->PlusVelocity(right * (-m_WalkSpeed));
		}
	}
}

void MobileSuitStateBeamRifleShot::Update()
{
	// �J�E���g�A�b�v
	m_FrameCount++;
	m_BoostDashCount++;
}

void MobileSuitStateBeamRifleShot::OnEnter()
{
	// �J�E���g������
	m_FrameCount = 0;
	// �V���b�g�t���O������
	m_IsShotFlag = false;
	// �u�[�X�g�_�b�V�����͎��ԏ�����
	m_BoostDashCount = MobileSuitStateBeamRifleShotData::k_BoostDashInputTime + 1;

	if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnEnter nullptr");
		return;
	}
	// �ǂ����_���Ă��邩
	m_TargetNum = m_MobileSuit->GetTargetNumber();

	if (m_TargetNum == 0)
	{
		// �����Ƃ�
		m_BeamVelocity = m_EnemyMobileSuit001->GetPosition() - m_MobileSuit->GetPosition();
		// ���K������
		m_BeamVelocity = DirectX::XMVector3Normalize(m_BeamVelocity);
	}
	else
	{
		// �����Ƃ�
		m_BeamVelocity = m_EnemyMobileSuit002->GetPosition() - m_MobileSuit->GetPosition();
		// ���K������
		m_BeamVelocity = DirectX::XMVector3Normalize(m_BeamVelocity);
	}


}

void MobileSuitStateBeamRifleShot::OnExit()
{
	// �J�E���g������
	m_FrameCount = 0;
	// �V���b�g�t���O������
	m_IsShotFlag = false;
	// �u�[�X�g�_�b�V�����͎��ԏ�����
	m_BoostDashCount = MobileSuitStateBeamRifleShotData::k_BoostDashInputTime + 1;
}
