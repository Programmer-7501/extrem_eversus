
#include"../../../../system/main.h"
#include "mobilesuit_state_nkakutou_first.h"
#include"../../move_component/rigidbody_component.h"
#include"../mobilesuit_state_component.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor/weapon/beam_saber.h"
#include"../../mesh_component/animation_model_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../cameracomponent/lockon_camera_component.h"

namespace MobileSuitStateNKakutouFirstData
{
	//! �U���S�̃t���[��
	static constexpr int k_AmountFrame = 60;

	//! �f�t�H���g�̃u�[�X�g���x
	static constexpr float k_DefaultBoostSpeed = 0.15f;

	//! �f�t�H���g�̃u�[�X�g�g�p��
	static constexpr float k_DefaultBoostUseNum = 0.1f;
}

MobileSuitStateNKakutouFirst::MobileSuitStateNKakutouFirst(MobileSuitStateComponent * ownerComponent)
	:MobileSuitState(ownerComponent)
	, m_CountFrame(0)
	, m_BoostSpeed(MobileSuitStateNKakutouFirstData::k_DefaultBoostSpeed)
	, m_UseBoostNum(MobileSuitStateNKakutouFirstData::k_DefaultBoostUseNum)
	, m_IsGoNextKakutou(false)
{
}

void MobileSuitStateNKakutouFirst::Init()
{
	if (GetData() == false)
	{
		Logger::GetInstance().SetLog("MobileSuitStateNKakutouFirst::Init GetData�����s");
		return;
	}
}

void MobileSuitStateNKakutouFirst::ProcessInput()
{
	if (m_Owner == nullptr || m_RigidBodyComponent == nullptr || m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateWalk::ProcessInput nullptr");
		return;
	}

	// �G�̕���������
	RotateToEnemy();

	Conv_XM::Vector3f enemyPosition, dif;
	if (m_MobileSuit->GetTargetNumber() == 0)
	{
		enemyPosition = m_EnemyMobileSuit001->GetPosition();
	}
	else
	{
		enemyPosition = m_EnemyMobileSuit002->GetPosition();
	}

	Conv_XM::Vector3f myPosition = m_MobileSuit->GetPosition();

	dif = enemyPosition - myPosition;
	dif = DirectX::XMVector3Normalize(dif);

	m_RigidBodyComponent->SetVelocity(dif * m_BoostSpeed);
	// �u�[�X�g�������
	m_BoostComponent->UseBoost(m_UseBoostNum);

	// �S�̃t���[���𒴂�����
	if (m_CountFrame >= MobileSuitStateNKakutouFirstData::k_AmountFrame)
	{
		if (m_IsGoNextKakutou == true)
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateNKakutouSecond");
		}
		else
		{
			m_Owner->ChangeMobileSuitState("MobileSuitStateAirIdle");
		}
	}


	if (m_Owner->GetIsAI() == true)
	{

	}
	else
	{
		InputManager& inputManager = InputManager::GetInstance();

		if(inputManager.GetIsTrigger(InputManager::IN_KAKUTOU) == true)
		{
			m_IsGoNextKakutou = true;
		}
	}

}

void MobileSuitStateNKakutouFirst::Update()
{
	// �J�E���g�A�b�v
	m_CountFrame++;
}

void MobileSuitStateNKakutouFirst::OnEnter()
{
	if (m_AnimationComponent == nullptr || m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateNKakutouFirst::OnEnter nullptr");
		return;
	}
	m_AnimationComponent->SetCurrentAnimationBlendToNextAnimation("nkaku_01", 5);

	m_CountFrame = 0;

	m_RigidBodyComponent->SetVelocity(Conv_XM::Vector3f(0.0f, 0.1f, 0.0f));
	m_RigidBodyComponent->SetIsGround(false);
	m_RigidBodyComponent->SetIsUseGravity(false);

	// �G�̕����Ɍ�����
	RotateToEnemy();

	// �t���O������
	m_IsGoNextKakutou = false;

	m_BeamSaber->StartNKakutou001();
}

void MobileSuitStateNKakutouFirst::OnExit()
{
	m_RigidBodyComponent->SetIsUseGravity(true);

	m_BeamSaber->Reset();
}

void MobileSuitStateNKakutouFirst::RotateToEnemy()
{
	if (m_MobileSuit == nullptr || m_EnemyMobileSuit001 == nullptr || m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateBeamRifleShot::OnExit nullptr");
		return;
	}

	Conv_XM::Vector3f enemyPosition, dif;
	if (m_MobileSuit->GetTargetNumber() == 0)
	{
		enemyPosition = m_EnemyMobileSuit001->GetPosition();
	}
	else
	{
		enemyPosition = m_EnemyMobileSuit002->GetPosition();
	}
	enemyPosition.y = 0.0f;

	Conv_XM::Vector3f myPosition = m_MobileSuit->GetPosition();
	myPosition.y = 0.0f;

	dif = enemyPosition - myPosition;
	dif = DirectX::XMVector3Normalize(dif);


	//���ςƊp�x���Z�o
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, dif);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(Conv_XM::Vector3f::WorldForward, dif);
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
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, dif);

		//�N�H�[�^�j�I�������߂�
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	//���߂��N�H�[�^�j�I������������
	m_MobileSuit->SetQuaternion(NextVecQuaternion);
}
