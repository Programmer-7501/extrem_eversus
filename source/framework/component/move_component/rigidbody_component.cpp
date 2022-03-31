
#include"../../../system/main.h"
#include "rigidbody_component.h"
#include"../../../system/input_manager.h"
#include"../../actor/actor.h"

namespace RigidBodyData
{
	//! �f�t�H���g�̏d��
	constexpr float k_DefaultGravity = 0.025f;
	//! �ő呬�x
	const Conv_XM::Vector3f k_MaxSpeed = Conv_XM::Vector3f(10.0f, 10.0f, 10.0f);
	//! �f�t�H���g�̒n�㖀�C
	constexpr float k_DefaultGroundFriction = 0.2f;
	//! �f�t�H���g�̋󒆖��C
	constexpr float k_DefaultAirFriction = 0.01f;
}

RigidBodyComponent::RigidBodyComponent(Actor * owner)
	: Component(owner)
	, m_IsGround(true)
	, m_IsUseGravity(true)
	, m_Gravity(RigidBodyData::k_DefaultGravity)
	, m_GroundFriction(RigidBodyData::k_DefaultGroundFriction)
	, m_AirFriction(RigidBodyData::k_DefaultAirFriction)
	, m_Direction(0.0f,0.0f,0.0f)
	, m_IsObjectGround(false)
	, m_IsMobileSuit(false)
{

}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Update()
{
	// null�`�F�b�N
	if (m_Owner == nullptr)
	{
		Logger::GetInstance().SetLog("RigidBodyComponent::ProcessInput m_OwnerComponent��nullptr");
		return;
	}
	// �|�W�V�����擾
	Conv_XM::Vector3f position = m_Owner->GetPosition();

	// �d�́A���C��������
	if (m_IsGround == false && m_IsUseGravity == true)
	{
		m_Direction.y -= m_Gravity;
		m_Direction.x *= (1.0f - m_AirFriction);
		m_Direction.z *= (1.0f - m_AirFriction);
	}
	else
	{
		m_Direction.x *= (1.0f - m_GroundFriction);
		m_Direction.z *= (1.0f - m_GroundFriction);
	}

	// ���x���N�����v����
	m_Direction = DirectX::XMVectorClamp(m_Direction, -(RigidBodyData::k_MaxSpeed), RigidBodyData::k_MaxSpeed);

	position += m_Direction;

	// ���r���X�[�c�Ȃ��ʂ���o�Ȃ��悤�ɃN�����v����
	if(m_IsMobileSuit == true)
	{
		position.x = Math::Clamp(position.x, GameData::k_StageLeftLimit, GameData::k_StageRightLimit);
		position.z = Math::Clamp(position.z, GameData::k_StageBackLimit, GameData::k_StageFrontLimit);
	}

	// �|�W�V������ݒ肷��
	m_Owner->SetPosition(position);
}
