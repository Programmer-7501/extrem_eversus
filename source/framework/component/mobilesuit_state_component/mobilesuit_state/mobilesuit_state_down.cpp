
#include"../../../../system/main.h"
#include "mobilesuit_state_down.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../move_component/rigidbody_component.h"


namespace MobileSuitAIStateDownData
{
	//! �f�t�H���g�̂̂����莞��
	static constexpr int k_DefaultBendBackWardTime = 70;
}


MobileSuitStateDown::MobileSuitStateDown(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
{
}

void MobileSuitStateDown::Init()
{
	// �f�[�^�擾�擾
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateDown::Init GetData�����s");
		return;
	}
}

void MobileSuitStateDown::ProcessInput()
{
}

void MobileSuitStateDown::Update()
{
	// �J�E���g�A�b�v
	m_CountFrame++;

	//�@�d�����Ԃ��߂�����
	if (m_CountFrame > MobileSuitAIStateDownData::k_DefaultBendBackWardTime)
	{
		if (m_Owner == nullptr || m_RigidBodyComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateDown::Update nullptr");
			return;
		}

		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateReturn");
		}
	}
}

void MobileSuitStateDown::OnEnter()
{
	if (m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr || m_BoostComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter nullptr");
		return;
	}

	// �J�E���g������
	m_CountFrame = 0;

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("down", 5);

	m_RigidBodyComponent->SetIsUseGravity(true);
}

void MobileSuitStateDown::OnExit()
{
}
