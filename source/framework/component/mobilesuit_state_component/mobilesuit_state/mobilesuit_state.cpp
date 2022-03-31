#include"../../../../system/main.h"
#include "mobilesuit_state.h"
#include"../mobilesuit_state_component.h"
#include"../../move_component/rigidbody_component.h"
#include"../../mobilesuit_component/boost_component.h"
#include"../../../actor/actor.h"
#include"../../../actor/mobilesuit/mobilesuit.h"
#include"../../../actor_manager/bullet_manager.h"

namespace MobileSuitStateData
{
	//! �r�[�����C�t�������܂ł̎���
	constexpr int k_BeamRifleShotTime = 12;

	//! �r�[�����C�t�������S�̃t���[��
	constexpr int k_BeamRifleShotAmoutTime = 24;

	//! �U����������ɂȂ�p�x(degree)
	constexpr float k_TurnAroundDegree = 110;
}

MobileSuitState::MobileSuitState(MobileSuitStateComponent * Owner)
	: m_Owner(Owner)
	, m_RigidBodyComponent(nullptr)
	, m_LockOnCameraComponent(nullptr)
	, m_BoostComponent(nullptr)
	, m_AnimationComponent(nullptr)
	, m_MobileSuit(nullptr)
	, m_BeamRifleBulletManager(nullptr)
	, m_CannonBulletManager(nullptr)
	, m_BazookaBulletManager(nullptr)
	, m_EnemyMobileSuit001(nullptr)
	, m_EnemyMobileSuit002(nullptr)
	, m_TargetNum(0)
{

}

bool MobileSuitState::GetData()
{
	if (m_Owner == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_Owner��nullptr");
		return false;
	}

	// ���W�b�h�{�f�B�R���|�[�l���g�擾
	m_RigidBodyComponent = m_Owner->GetRigidBodyComponent();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_RigidBodyComponent��nullptr");
		return false;
	}

	// ���b�N�I���J�����R���|�[�l���g�擾
	m_LockOnCameraComponent = m_Owner->GetLockOnCameraComponent();
	if (m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_RigidBodyComponent��nullptr");
		return false;
	}

	// �u�[�X�g�R���|�[�l���g�擾
	m_BoostComponent = m_Owner->GetBoostComponent();
	if (m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BoostComponent��nullptr");
		return false;
	}

	// �A�j���[�V�����R���|�[�l���g���擾
	m_AnimationComponent = m_Owner->GetAnimationModelComponent();
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_AnimationComponent��nullptr");
		return false;
	}

	// ���r���X�[�c�A�N�^�[�擾
	m_MobileSuit = dynamic_cast<MobileSuit*>(m_Owner->GetOwner());
	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_MobileSuit��nullptr");
		return false;
	}

	// �r�[�����C�t���o���b�g�}�l�[�W���[���擾
	m_BeamRifleBulletManager = m_MobileSuit->GetBeamRifleBulletManager();
	if (m_BeamRifleBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BeamRifleBulletManager��nullptr");
		return false;
	}

	// �L���m���o���b�g�}�l�[�W���[���擾
	m_CannonBulletManager = m_MobileSuit->GetCannonBulletManager();
	if (m_CannonBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_CannonBulletManager��nullptr");
		return false;
	}

	// �o�Y�[�J�o���b�g�}�l�[�W���[���擾
	m_BazookaBulletManager = m_MobileSuit->GetBazookaBulletManager();
	if (m_BazookaBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BazookaBulletManager��nullptr");
		return false;
	}

	// �G�l�~�[�����
	m_EnemyMobileSuit001 = m_MobileSuit->GetEnemyMobileSuit001();
	if (m_EnemyMobileSuit001 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_EnemyMobileSuit001��nullptr");
		return false;
	}

	m_EnemyMobileSuit002 = m_MobileSuit->GetEnemyMobileSuit002();
	if (m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_EnemyMobileSuit002��nullptr");
		return false;
	}

	// �r�[���T�[�x��
	m_BeamSaber = m_MobileSuit->GetBeamSaber();
	if (m_BeamSaber == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BeamSaber��nullptr");
		return false;
	}

	return true;
}

void MobileSuitState::RotationActor(const Conv_XM::Vector3f & cameraForward, const Conv_XM::Vector3f & actorForward, Direction direction, float rotationLimit)
{
	Conv_XM::Vector4f q1;
	switch (direction)
	{
	case MobileSuitState::FORWARD:
		// ��]�����Ȃ�
		q1 = DirectX::XMQuaternionIdentity();
		break;
	case MobileSuitState::BACK:
		// y���ō���90�x��]�������N�H�[�^�j�I�����쐬
		q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(180.0f));
		break;
	case MobileSuitState::RIGHT:
		// y���ŉE��90�x��]�������N�H�[�^�j�I�����쐬
		q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
		break;
	case MobileSuitState::LEFT:
		// y���ō���90�x��]�������N�H�[�^�j�I�����쐬
		q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(-90.0f));
		break;
	default:
		break;
	}

	//���K������
	Conv_XM::Vector3f nextVec = DirectX::XMVector3Rotate(cameraForward, q1);
	nextVec = DirectX::XMVector3Normalize(nextVec);

	//0�x�N�g�����ǂ����G���[�`�F�b�N
	float error = Conv_XM::Vector3f::LengthSq(nextVec);
	if (error < Math::k_Epsilon)
	{
		return;
	}

	//���ςƊp�x���Z�o
	float dot = Conv_XM::Vector3f::Dot(actorForward, nextVec);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(actorForward, nextVec);
	float angle = xmangle.x;

	Conv_XM::Vector4f NextVecQuaternion;

	// �������p�Ȃ�
	if (dot > 0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionIdentity();
	}
	else
	{
		if (rotationLimit < Math::k_Epsilon)
		{
			if (dot < -0.9999f)
			{
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis((DirectX::XMVECTOR)Conv_XM::Vector3f::WorldUp, DirectX::XM_PI);
			}
			else
			{
				//�O�ς����߉�]�������߂�
				Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

				//�N�H�[�^�j�I�������߂�
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
			}
		}
		else
		{
			if (fabsf(angle) > DirectX::XMConvertToRadians(rotationLimit))
			{
				angle = DirectX::XMConvertToRadians(rotationLimit);
			}
			//�O�ς����߉�]�������߂�
			Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

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
	}

	//���߂��N�H�[�^�j�I������������
	Conv_XM::Vector4f myQuaternion = m_MobileSuit->GetQuaternion();
	Conv_XM::Vector4f q = DirectX::XMQuaternionMultiply(myQuaternion, NextVecQuaternion);
	m_MobileSuit->SetQuaternion(q);
}

void MobileSuitState::RotationActor(const Conv_XM::Vector3f & cameraForward, const Conv_XM::Vector3f & actorForward, const Conv_XM::Vector2f & direction, float rotationLimit)
{
	Conv_XM::Vector4f q1;

	Conv_XM::Vector2f len = DirectX::XMVector2Length(direction);
	if (len.x <= Math::k_Epsilon)
	{
		return;
	}
	// �O�x�N�g���ƖڕW�x�N�g���̂Ȃ��p���Z�o
	float cos = Conv_XM::Vector2f::Dot(Conv_XM::Vector2f::Forward, direction);
	float rad;
	if (direction.x > 0)
	{
		rad = acosf(cos);
	}
	else
	{
		rad = -acosf(cos);
	}


	q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, rad);

	//���K������
	Conv_XM::Vector3f nextVec = DirectX::XMVector3Rotate(cameraForward, q1);
	nextVec = DirectX::XMVector3Normalize(nextVec);

	//0�x�N�g�����ǂ����G���[�`�F�b�N
	float error = Conv_XM::Vector3f::LengthSq(nextVec);
	if (error < Math::k_Epsilon)
	{
		return;
	}

	//���ςƊp�x���Z�o
	float dot = Conv_XM::Vector3f::Dot(actorForward, nextVec);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(actorForward, nextVec);
	float angle = xmangle.x;

	Conv_XM::Vector4f NextVecQuaternion;

	// �������p�Ȃ�
	if (dot > 0.9999f)
	{
		NextVecQuaternion = DirectX::XMQuaternionIdentity();
	}
	else
	{
		if (rotationLimit < Math::k_Epsilon)
		{
			if (dot < -0.9999f)
			{
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis((DirectX::XMVECTOR)Conv_XM::Vector3f::WorldUp, DirectX::XM_PI);
			}
			else
			{
				//�O�ς����߉�]�������߂�
				Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

				//�N�H�[�^�j�I�������߂�
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
			}
		}
		else
		{
			if (fabsf(angle) > DirectX::XMConvertToRadians(rotationLimit))
			{
				angle = DirectX::XMConvertToRadians(rotationLimit);
			}
			//�O�ς����߉�]�������߂�
			Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

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
	}

	//���߂��N�H�[�^�j�I������������
	Conv_XM::Vector4f myQuaternion = m_MobileSuit->GetQuaternion();
	Conv_XM::Vector4f q = DirectX::XMQuaternionMultiply(myQuaternion, NextVecQuaternion);
	m_MobileSuit->SetQuaternion(q);
}

void MobileSuitState::RotateToEnemy()
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
