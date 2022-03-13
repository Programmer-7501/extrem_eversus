
#include"../../../../system/main.h"
#include "mobilesuit_state_idle.h"
#include"../../move_component/rigidbody_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../../system/input_manager.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"
#include"../../mesh_component/animation_model_component.h"


namespace StateIdleData
{
	//! �f�t�H���g�̃W�����v���x
	static constexpr float k_DefaultJumpSpeed = 0.1f;
}

MobileSuitStateIdle::MobileSuitStateIdle(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_JumpSpeed(StateIdleData::k_DefaultJumpSpeed)
{
}

void MobileSuitStateIdle::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetData�����s");
		return;
	}
}

void MobileSuitStateIdle::ProcessInput()
{
	// null�`�F�b�N
	if (m_Owner == nullptr || m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}

	//! AI�̏ꍇ
	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		// �C���v�b�g�}�l�[�W���[�擾
		InputManager& inputManager = InputManager::GetInstance();


		if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) &&
			m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) && 
			m_CannonBulletManager->GetCurrentBullet() > 0)// ����ˌ�����
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateCannonShot");
		}
		else if (m_Owner->GetIsPressInput(MobileSuitStateComponent::IN_LEFT_STICK) == true)// ������ԂɈڍs
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateWalk");

			m_Owner->SetStepReserve(MobileSuitStateComponent::SD_FORWARD);
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) == true)// �u�[�X�g�W�����v��ԂɈڍs
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateJumpStandby");
		}
		else if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_SHAGEKI) == true)// �ˌ�����
		{
			m_MobileSuit->BeamRifleShot();
		}

		// �i������
		if (inputManager.GetIsTrigger(InputManager::IN_KAKUTOU))
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateNKakutouFirst");
		}
	}
}

void MobileSuitStateIdle::Update()
{

}

void MobileSuitStateIdle::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		//�@�������O�Ɏg���\��������̂�
		m_AnimationComponent = m_Owner->GetAnimationModelComponent();

		if (m_AnimationComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateIdle::OnEnter m_AnimationComponent��nullptr");
			m_AnimationComponent->SetAnimation("idle");

			return;
		}
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("idle", 20);
}

void MobileSuitStateIdle::OnExit()
{
}
