
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
	// アクター独自の読み込み処理
	LoadActor();

	// コンポーネントの読み込み処理
	LoadComponents();
}

void Actor::Init()
{
	// アクター独自の初期化処理
	InitActor();

	// コンポーネントの初期化処理
	InitComponents();
}

void Actor::Uninit()
{
	// コンポーネントの終了処理
	UninitComponents();

	// アクター独自の終了処理
	UninitActor();
}

void Actor::ProcessInput()
{
	// アクター独自の入力処理
	ProcessInputActor();

	// コンポーネントの入力処理
	ProcessInputComponents();

	//更新フラグが立っているなら座標更新
	if (m_RecomputeTransform)
	{
		ComputeWorldTransform();
	}
}

void Actor::Update()
{
	// アクター独自の入力処理
	UpdateActor();

	// コンポーネントの入力処理
	UpdateComponents();

	//更新フラグが立っているなら座標更新
	if (m_RecomputeTransform)
	{
		ComputeWorldTransform();
	}
}

void Actor::ComputeWorldTransform()
{
	// 座標更新フラグを降ろす
	m_RecomputeTransform = false;

	Conv_XM::Matrix4x4f mtxS, mtxR, mtxT;
	mtxS = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	mtxR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);
	mtxT = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_WorldMatrix = (DirectX::XMMATRIX)mtxS * (DirectX::XMMATRIX)mtxR * (DirectX::XMMATRIX)mtxT;

	// コンポーネントにトランスフォームを更新したことを伝える
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
	// コンポーネント入力処理
	for (auto* const component : m_Components)
	{
		// nullチェック
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::LoadComponents componentがnullptr");
			continue;
		}
		component->Load();
	}
}

void Actor::InitComponents()
{
	// コンポーネント入力処理
	for (auto* const component : m_Components)
	{
		// nullチェック
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::InitComponent componentがnullptr");
			continue;
		}
		component->Init();
	}
}

void Actor::UninitComponents()
{
	// コンポーネント終了処理
	for (auto* const component : m_Components)
	{
		// nullチェック
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::UninitComponent componentがnullptr");
			continue;
		}
		component->Uninit();
	}

	// コンポーネント削除
	for (auto* component : m_Components)
	{
		// nullチェック
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::UninitComponent componentがnullptr");
			continue;
		}
		delete component;
		component = nullptr;
	}
	//中身を空にする
	m_Components.clear();
}

void Actor::ProcessInputComponents()
{
	// コンポーネント入力処理
	for (auto* const component : m_Components)
	{
		// nullチェック
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::ProcessInputComponent componentがnullptr");
			continue;
		}
		component->ProcessInput();
	}
}

void Actor::UpdateComponents()
{
	// コンポーネント更新処理
	for (auto* const component : m_Components)
	{
		// nullチェック
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::UpdateComponents componentがnullptr");
			continue;
		}
		component->Update();
	}
}

void Actor::AddComponent(Component * component)
{
	//更新番号を取得
	int MyUpdateOrder = component->GetUpdateOrder();

	//走査し、更新順になるように挿入する
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
