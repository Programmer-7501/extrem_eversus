
#include"../../../../system/main.h"
#include "mobilesuit_state_bazooka.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../cameracomponent/lockon_camera_component.h"
#include"../../../actor_manager/bullet_manager.h"

namespace MobileSuitStateBazookaData
{
	//! �f�t�H���g�̃X�e�[�g����
	static constexpr int k_DefaultTime = 60;

	//! �u�[�X�g�_�b�V�����͎�t����
	constexpr int k_BoostDashInputTime = 12;

	//! �u�[�X�g�_�b�V������
	constexpr float k_BoostDashInitialSpeed = 1.0f;

	//! �u�[�X�g�_�b�V�����Ɏg�p������
	constexpr float k_BoostDashUseBoostNum = 20.0f;

	//! �󒆔w�ʂ����̏ꍇ�̈�t���[���ɂ�����u�[�X�g�g�p��
	constexpr float k_TurnAroundAirUseBoostNum = 2.0f;

	//! �o�Y�[�J��ł܂ł̃J�E���g
	constexpr int k_DefaultBazookaShotCount = 10;
}

MobileSuitStateBazooka::MobileSuitStateBazooka(MobileSuitStateComponent * ownerComponent)
	: MobileSuitState(ownerComponent)
	, m_CountFrame(0)
	, m_BoostDashCount(0)
	, m_BoostDashInitialSpeed(MobileSuitStateBazookaData::k_BoostDashInitialSpeed)
	, m_IsShotFlag(false)
	, m_BazookaSpeed(0)
{
}

void MobileSuitStateBazooka::Init()
{
	// �f�[�^�擾�擾
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateDown::Init GetData�����s");
		return;
	}
}

void MobileSuitStateBazooka::ProcessInput()
{

	//�@�d�����Ԃ��߂�����
	if (m_CountFrame > MobileSuitStateBazookaData::k_DefaultTime)
	{
		if (m_Owner == nullptr || m_RigidBodyComponent == nullptr)
		{
			Logger::GetInstance().SetLog("MobileSuitStateBazooka::Update nullptr");
			return;
		}

		if (m_RigidBodyComponent->GetIsGround() == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateIdle");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}

	}

	// �r�[�����C�t����łJ�E���g�ɂȂ�����
	if (m_CountFrame > MobileSuitStateBazookaData::k_DefaultBazookaShotCount && m_IsShotFlag == false)
	{
		// �r�[����ł�
		if (m_TargetNum == 0)
		{
			m_BazookaBulletManager->UseBullet(m_EnemyMobileSuit001, m_MobileSuit->GetPosition(), m_MobileSuit->GetForward());
		}
		else
		{
			m_BazookaBulletManager->UseBullet(m_EnemyMobileSuit002, m_MobileSuit->GetPosition(), m_MobileSuit->GetForward());
		}

		// �V���b�g���������t���O�𗧂Ă�
		m_IsShotFlag = true;
	}



	// �u�[�X�g�_�b�V�����͑ҋ@��ԂōĂуu�[�X�g�_�b�V�����͂����ꂽ��
	if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount <= MobileSuitStateBazookaData::k_BoostDashInputTime)
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

		RotationActor(forward, myForward, m_Owner->GetInputDirection());
		Conv_XM::Vector3f dir = m_MobileSuit->GetForward();
		m_RigidBodyComponent->PlusVelocity(dir * m_BoostDashInitialSpeed);

		m_MobileSuit->SetIsBoostDash(true);
		m_Owner->ChangeMobileSuitState("MobileSuitStateBoostDash");
		m_BoostComponent->UseBoost(MobileSuitStateBazookaData::k_BoostDashUseBoostNum);
	}

	// �u�[�X�g�_�b�V�����͂��Ȃ���ԂŃW�����v�{�^���������ꂽ��
	if (m_Owner->GetIsTriggerInput(MobileSuitStateComponent::IN_JUMP) && m_BoostDashCount > MobileSuitStateBazookaData::k_BoostDashInputTime)
	{
		// �J�E���g���[���ɂ���
		m_BoostDashCount = 0;
	}


	// �󒆂ɂ���ꍇ�u�[�X�g���g�p����
	if (m_RigidBodyComponent->GetIsGround() == false)
	{
		m_BoostComponent->UseBoost(MobileSuitStateBazookaData::k_TurnAroundAirUseBoostNum);
	}
}

void MobileSuitStateBazooka::Update()
{
	// �J�E���g�A�b�v
	m_CountFrame++;
	m_BoostDashCount++;
}

void MobileSuitStateBazooka::OnEnter()
{
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBazooka::OnEnter m_AnimationComponent��nullptr");
		return;
	}

	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("beamrifle", 20);

	// �J�E���g������
	m_CountFrame = 0;
	m_BoostDashCount = MobileSuitStateBazookaData::k_BoostDashInputTime + 1;
	m_IsShotFlag = false;

	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBazooka::OnEnter nullptr");
		return;
	}

	m_RigidBodyComponent->SetDirection(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	m_RigidBodyComponent->SetIsUseGravity(false);


	// �G�̕����Ɍ�����
	RotateToEnemy();
}

void MobileSuitStateBazooka::OnExit()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBazooka::OnEnter nullptr");
		return;
	}
	m_RigidBodyComponent->SetIsUseGravity(true);
}
