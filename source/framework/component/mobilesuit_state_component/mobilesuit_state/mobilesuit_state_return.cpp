
#include"../../../../system/main.h"
#include "mobilesuit_state_return.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../mobilesuit_state_component.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../move_component/rigidbody_component.h"

namespace MobileSuitAIStateReturnData
{
	//! �f�t�H���g�̂̂����莞��
	static constexpr int k_DefaultBendBackWardTime = 50;
}


MobileSuitStateReturn::MobileSuitStateReturn(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
{
}

void MobileSuitStateReturn::Init()
{
	// �f�[�^�擾�擾
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::Init GetData�����s");
		return;
	}
}

void MobileSuitStateReturn::ProcessInput()
{
}

void MobileSuitStateReturn::Update()
{
	// �J�E���g�A�b�v
	m_CountFrame++;

	//�@�d�����Ԃ��߂�����
	if (m_CountFrame > MobileSuitAIStateReturnData::k_DefaultBendBackWardTime)
	{
		if (m_Owner == nullptr || m_RigidBodyComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateLandingRecovery::Update nullptr");
			return;
		}

		m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
	}
}

void MobileSuitStateReturn::OnEnter()
{
	if (m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr || m_BoostComponent == nullptr || m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter nullptr");
		return;
	}

	// �J�E���g������
	m_CountFrame = 0;

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("return", 5);
}

void MobileSuitStateReturn::OnExit()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::OnEnter m_AnimationComponent��nullptr");
		return;
	}

	m_AnimationComponent->SetAnimation("idle");
}
