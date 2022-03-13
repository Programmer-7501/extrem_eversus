
#include"../../../../system/main.h"
#include "mobilesuit_state_step.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../cameracomponent/lockon_camera_component.h"

namespace MobileSuitStateStepData
{
	//! �f�t�H���g�̃X�e�b�v���x
	constexpr float k_DefaultStepSpeed = 0.1f;

	//! �f�t�H���g�̃X�e�b�v�t���[��
	constexpr int k_DefaultStepFrame = 20;

	//! �f�t�H���g�̃u�[�X�g�g�p��
	constexpr float k_DefaultBoostDashUseBoostNum = 10.0f;
}

MobileSuitStateStep::MobileSuitStateStep(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_FrameCount(0)
{
}

void MobileSuitStateStep::Init()
{
	// �f�[�^�擾�擾
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetData�����s");
		return;
	}
}

void MobileSuitStateStep::ProcessInput()
{
	if (m_RigidBodyComponent == nullptr || m_Owner == nullptr || m_BoostComponent == nullptr || m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}
	InputManager& inputManager = InputManager::GetInstance();

	// �K�莞�Ԃ𒴂�����
	if (m_FrameCount > MobileSuitStateStepData::k_DefaultStepFrame)
	{
		if (m_RigidBodyComponent->GetIsGround() == false)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
	}

	// �J�����̑O�x�N�g�����擾
	Conv_XM::Vector3f forward = m_LockOnCameraComponent->GetForward();
	forward.y = 0.0f;
	forward = DirectX::XMVector3Normalize(forward);

	// �J�����̉E�x�N�g�����擾
	Conv_XM::Vector3f right = m_LockOnCameraComponent->GetRight();
	right.y = 0.0f;
	right = DirectX::XMVector3Normalize(right);

	if (inputManager.GetIsPress(InputManager::IN_FRONT))
	{
		m_RigidBodyComponent->PlusVelocity(forward * MobileSuitStateStepData::k_DefaultStepSpeed);
	}
	else if (inputManager.GetIsPress(InputManager::IN_BACK))
	{
		m_RigidBodyComponent->PlusVelocity(forward * (-MobileSuitStateStepData::k_DefaultStepSpeed));
	}
	else if (inputManager.GetIsPress(InputManager::IN_RIGHT))
	{
		m_RigidBodyComponent->PlusVelocity(right * MobileSuitStateStepData::k_DefaultStepSpeed);
	}
	else if (inputManager.GetIsPress(InputManager::IN_LEFT))
	{
		m_RigidBodyComponent->PlusVelocity(right * (-MobileSuitStateStepData::k_DefaultStepSpeed));
	}
}

void MobileSuitStateStep::Update()
{
	// �J�E���g�A�b�v
	m_FrameCount++;
}

void MobileSuitStateStep::OnEnter()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateStep::OnEnter m_RigidBodyComponent��nullptr");
		return;
	}

	// �d�͂��I�t�ɂ���
	m_RigidBodyComponent->SetIsUseGravity(false);

	// �t���[���J�E���g������
	m_FrameCount = 0;
}

void MobileSuitStateStep::OnExit()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateStep::OnExit m_RigidBodyComponent��nullptr");
		return;
	}

	// �d�͂��I���ɂ���
	m_RigidBodyComponent->SetIsUseGravity(true);
}
