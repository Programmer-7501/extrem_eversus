
#include"../../../system/main.h"
#include "bullet.h"
#include"../../component/move_component/rigidbody_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../../particle/particle_manager.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"

namespace BulletData
{
	//! デフォルトの速度
	static constexpr float k_DefaultSpeed = 0.75f;

	//! デフォルトの終了フレーム
	static constexpr int k_DefaultFinishFrame = 300;

	//! 最大回転量
	static constexpr float k_DefaultMaxRotate = 0.25f;
}

Bullet::Bullet()
	: m_RigidBodyComponent(nullptr)
	, m_OBBCollisionComponent(nullptr)
	, m_Speed(BulletData::k_DefaultSpeed)
	, m_FrameCount(0)
	, m_FinishFrame(BulletData::k_DefaultFinishFrame)
	, m_TargetMobileSuit(nullptr)
	, m_Direction(0.0f,0.0f,0.0f)
	, m_TargetFlag(false)
	, m_RotationLimit(BulletData::k_DefaultMaxRotate)
	, m_LandingEffectManager(nullptr)
{
	m_ActorType = Actor::ACTORTYPE_PLAYER_BULLET;
}

Bullet::~Bullet()
{
}

void Bullet::SetDirection(const Conv_XM::Vector3f & direction)
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_RigidBodyComponentがnullptr");
		return;
	}

	m_Direction = DirectX::XMVector3Normalize(direction);

	m_RigidBodyComponent->SetDirection(m_Direction * m_Speed);
}

void Bullet::SetActive(bool flag)
{
	// 入力
	m_Active = flag;

	if (m_Active == false)
	{
		m_FrameCount = 0;
		m_TargetFlag = false;
		m_TargetMobileSuit = nullptr;
	}
}

void Bullet::LoadActor()
{
	// リジッドボディコンポーネント作成
	m_RigidBodyComponent = CreateComponent<RigidBodyComponent>();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_RigidBodyComponentがnullptr");
		return;
	}

	// OBBコリジョンコンポーネント作成
	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();
	if (m_OBBCollisionComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor m_OBBCollisionComponentがnullptr");
		return;
	}
	m_OBBCollisionComponent->SetOffsetScale(Conv_XM::Vector3f(0.5f, 0.5f, 0.5f));

	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* currentScene = sceneManager.GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor currentSceneがnullptr");
		return;
	}
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor sceneCollisionManagerがnullptr");
		return;
	}
	//　コリジョンのセット
	sceneCollisionManager->SetBulletCollision(m_OBBCollisionComponent);

	// OBBデバッグビュー作成
	CreateComponent<OBBDebugViewComponent>();
}

void Bullet::InitActor()
{
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("Bullet::InitActor nullptr");
		return;
	}

	m_RigidBodyComponent->SetGravity(0.0f);
	m_RigidBodyComponent->SetAirFriction(0.0f);
	m_RigidBodyComponent->SetIsGround(false);
}

void Bullet::ProcessInputActor()
{
	// 追跡フラグが立っているなら追う
	if (m_TargetFlag == true)
	{
		TargetChase();
	}
}

void Bullet::UpdateActor()
{
	// フレームカウントが終了フレームを超えてたら
	if (m_FinishFrame < m_FrameCount)
	{
		// 非アクティブ状態にする
		SetActive(false);
	}
	//　カウントアップ
	m_FrameCount++;
}

void Bullet::CollisionDetected(class Actor* coleObj)
{
	UNREFERENCED_PARAMETER(coleObj);
}

void Bullet::TargetChase()
{
	if (m_TargetFlag == false || m_TargetMobileSuit == nullptr)
	{
		return;
	}

	// ターゲットのポジション取得
	Conv_XM::Vector3f targetPos = m_TargetMobileSuit->GetPosition();

	// 自分からターゲットの単位ベクトル取得
	Conv_XM::Vector3f myPosToTargetVec = targetPos - m_Position;
	myPosToTargetVec = DirectX::XMVector3Normalize(myPosToTargetVec);

	// 速度の単位ベクトル取得
	Conv_XM::Vector3f velDir;
	velDir = DirectX::XMVector3Normalize(m_Direction);

	//0ベクトルかどうかエラーチェック
	float error = Conv_XM::Vector3f::LengthSq(myPosToTargetVec);
	if (error < Math::k_Epsilon)
	{
		return;
	}

	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(velDir, myPosToTargetVec);

	if (dot < 0.0f)
	{
		m_TargetFlag = false;
	}
	else
	{
		Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(velDir, myPosToTargetVec);
		float angle = xmangle.x;

		Conv_XM::Vector4f NextVecQuaternion;

		// 同じ方角なら
		if (dot > 0.9999f)
		{
			NextVecQuaternion = DirectX::XMQuaternionIdentity();
		}
		else
		{
			if (fabsf(angle) > DirectX::XMConvertToRadians(m_RotationLimit))
			{
				angle = DirectX::XMConvertToRadians(m_RotationLimit);
			}
			//外積を求め回転軸を求める
			Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(velDir, myPosToTargetVec);

			// 次のベクトルとアクターのベクトルが同一方向なら
			if (Conv_XM::Vector3f::LengthSq(axis) < Math::k_Epsilon)
			{
				NextVecQuaternion = DirectX::XMQuaternionIdentity();
			}
			else
			{
				//クォータニオンを求める
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
			}
		}

		m_Direction = DirectX::XMVector3Rotate(m_Direction, NextVecQuaternion);

		m_Direction = DirectX::XMVector3Normalize(m_Direction);

		// リジッドボディに速度を入力する
		m_RigidBodyComponent->SetDirection(m_Direction * m_Speed);
	}
}
