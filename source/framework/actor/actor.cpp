
#include"../../system/main.h"
#include "actor.h"

Actor::Actor()
	: m_ActorType(ACTORTYPE_NONE)
	, m_Active(true)
	, m_WorldMatrix(DirectX::XMMatrixIdentity())
	, m_Position(0.0f,0.0f,0.0f)
	, m_Scale(1.0f,1.0f,1.0f)
	, m_Quaternion(0.0f, 0.0f, 0.0f, 1.0f)
	, m_RecomputeTransform(true)
{
}

Actor::~Actor()
{
}

void Actor::LoadData()
{
	// �A�N�^�[�Ǝ��̓ǂݍ��ݏ���
	LoadActor();

	// �R���|�[�l���g�̓ǂݍ��ݏ���
	LoadComponents();
}

void Actor::Init()
{
	// �A�N�^�[�Ǝ��̏���������
	InitActor();

	// �R���|�[�l���g�̏���������
	InitComponents();
}

void Actor::Uninit()
{
	// �R���|�[�l���g�̏I������
	UninitComponents();

	// �A�N�^�[�Ǝ��̏I������
	UninitActor();
}

void Actor::ProcessInput()
{
	// �A�N�^�[�Ǝ��̓��͏���
	ProcessInputActor();

	// �R���|�[�l���g�̓��͏���
	ProcessInputComponents();

	//�X�V�t���O�������Ă���Ȃ���W�X�V
	if (m_RecomputeTransform)
	{
		ComputeWorldTransform();
	}
}

void Actor::Update()
{
	// �A�N�^�[�Ǝ��̓��͏���
	UpdateActor();

	// �R���|�[�l���g�̓��͏���
	UpdateComponents();

	//�X�V�t���O�������Ă���Ȃ���W�X�V
	if (m_RecomputeTransform)
	{
		ComputeWorldTransform();
	}
}

void Actor::ComputeWorldTransform()
{
	// ���W�X�V�t���O���~�낷
	m_RecomputeTransform = false;

	Conv_XM::Matrix4x4f mtxS, mtxR, mtxT;
	mtxS = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	mtxR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);
	mtxT = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_WorldMatrix = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;

	// �R���|�[�l���g�Ƀg�����X�t�H�[�����X�V�������Ƃ�`����
	for (auto comp : m_Components)
	{
		comp->OnUpdateWorldTransform();
	}
}

void Actor::CollisionDetected(Actor * coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);
}

void Actor::SetEuler(float x, float y, float z)
{
	float rad_X, rad_Y, rad_Z;
	rad_X = DirectX::XMConvertToRadians(x);
	rad_Y = DirectX::XMConvertToRadians(y);
	rad_Z = DirectX::XMConvertToRadians(z);
	m_Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rad_X, rad_Y, rad_Z);
	m_RecomputeTransform = true;
}

void Actor::SetEuler(const Conv_XM::Vector3f & euler)
{
	float rad_X, rad_Y, rad_Z;
	rad_X = DirectX::XMConvertToRadians(euler.x);
	rad_Y = DirectX::XMConvertToRadians(euler.y);
	rad_Z = DirectX::XMConvertToRadians(euler.z);
	m_Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rad_X, rad_Y, rad_Z);
	m_RecomputeTransform = true;
}

Conv_XM::Vector3f Actor::GetForward() const
{
	Conv_XM::Vector3f ret;
	ret = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldForward, m_Quaternion);
	return ret;
}

Conv_XM::Vector3f Actor::GetRight() const
{
	Conv_XM::Vector3f ret;
	ret = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldRight, m_Quaternion);
	return ret;
}

Conv_XM::Vector3f Actor::GetUp() const
{
	Conv_XM::Vector3f ret;
	ret = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldUp, m_Quaternion);
	return ret;
}

void Actor::LoadActor()
{
}

void Actor::InitActor()
{
}

void Actor::UninitActor()
{
}

void Actor::UpdateActor()
{
}

void Actor::ProcessInputActor()
{
}

void Actor::LoadComponents()
{
	// �R���|�[�l���g���͏���
	for (auto* const component : m_Components)
	{
		// null�`�F�b�N
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::LoadComponents component��nullptr");
			continue;
		}
		component->Load();
	}
}

void Actor::InitComponents()
{
	// �R���|�[�l���g���͏���
	for (auto* const component : m_Components)
	{
		// null�`�F�b�N
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::InitComponent component��nullptr");
			continue;
		}
		component->Init();
	}
}

void Actor::UninitComponents()
{
	// �R���|�[�l���g�I������
	for (auto* const component : m_Components)
	{
		// null�`�F�b�N
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::UninitComponent component��nullptr");
			continue;
		}
		component->Uninit();
	}

	// �R���|�[�l���g�폜
	for (auto* component : m_Components)
	{
		// null�`�F�b�N
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::UninitComponent component��nullptr");
			continue;
		}
		delete component;
		component = nullptr;
	}
	//���g����ɂ���
	m_Components.clear();
}

void Actor::ProcessInputComponents()
{
	// �R���|�[�l���g���͏���
	for (auto* const component : m_Components)
	{
		// null�`�F�b�N
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::ProcessInputComponent component��nullptr");
			continue;
		}
		component->ProcessInput();
	}
}

void Actor::UpdateComponents()
{
	// �R���|�[�l���g�X�V����
	for (auto* const component : m_Components)
	{
		// null�`�F�b�N
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::UpdateComponents component��nullptr");
			continue;
		}
		component->Update();
	}
}

void Actor::AddComponent(Component * component)
{
	//�X�V�ԍ����擾
	int MyUpdateOrder = component->GetUpdateOrder();

	//�������A�X�V���ɂȂ�悤�ɑ}������
	auto&& iter = m_Components.begin();
	for (; iter != m_Components.end(); iter++)
	{
		if (MyUpdateOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	m_Components.insert(iter, component);
}
