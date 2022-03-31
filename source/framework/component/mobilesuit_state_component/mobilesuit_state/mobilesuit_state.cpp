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
	//! ビームライフルを撃つまでの時間
	constexpr int k_BeamRifleShotTime = 12;

	//! ビームライフルを撃つ全体フレーム
	constexpr int k_BeamRifleShotAmoutTime = 24;

	//! 振り向き撃ちになる角度(degree)
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
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_Ownerがnullptr");
		return false;
	}

	// リジッドボディコンポーネント取得
	m_RigidBodyComponent = m_Owner->GetRigidBodyComponent();
	if (m_RigidBodyComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_RigidBodyComponentがnullptr");
		return false;
	}

	// ロックオンカメラコンポーネント取得
	m_LockOnCameraComponent = m_Owner->GetLockOnCameraComponent();
	if (m_LockOnCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_RigidBodyComponentがnullptr");
		return false;
	}

	// ブーストコンポーネント取得
	m_BoostComponent = m_Owner->GetBoostComponent();
	if (m_BoostComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BoostComponentがnullptr");
		return false;
	}

	// アニメーションコンポーネントを取得
	m_AnimationComponent = m_Owner->GetAnimationModelComponent();
	if (m_AnimationComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_AnimationComponentがnullptr");
		return false;
	}

	// モビルスーツアクター取得
	m_MobileSuit = dynamic_cast<MobileSuit*>(m_Owner->GetOwner());
	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_MobileSuitがnullptr");
		return false;
	}

	// ビームライフルバレットマネージャーを取得
	m_BeamRifleBulletManager = m_MobileSuit->GetBeamRifleBulletManager();
	if (m_BeamRifleBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BeamRifleBulletManagerがnullptr");
		return false;
	}

	// キャノンバレットマネージャーを取得
	m_CannonBulletManager = m_MobileSuit->GetCannonBulletManager();
	if (m_CannonBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_CannonBulletManagerがnullptr");
		return false;
	}

	// バズーカバレットマネージャーを取得
	m_BazookaBulletManager = m_MobileSuit->GetBazookaBulletManager();
	if (m_BazookaBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BazookaBulletManagerがnullptr");
		return false;
	}

	// エネミーを入力
	m_EnemyMobileSuit001 = m_MobileSuit->GetEnemyMobileSuit001();
	if (m_EnemyMobileSuit001 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_EnemyMobileSuit001がnullptr");
		return false;
	}

	m_EnemyMobileSuit002 = m_MobileSuit->GetEnemyMobileSuit002();
	if (m_EnemyMobileSuit002 == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_EnemyMobileSuit002がnullptr");
		return false;
	}

	// ビームサーベル
	m_BeamSaber = m_MobileSuit->GetBeamSaber();
	if (m_BeamSaber == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitState::GetData m_BeamSaberがnullptr");
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
		// 回転させない
		q1 = DirectX::XMQuaternionIdentity();
		break;
	case MobileSuitState::BACK:
		// y軸で左に90度回転させたクォータニオンを作成
		q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(180.0f));
		break;
	case MobileSuitState::RIGHT:
		// y軸で右に90度回転させたクォータニオンを作成
		q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(90.0f));
		break;
	case MobileSuitState::LEFT:
		// y軸で左に90度回転させたクォータニオンを作成
		q1 = DirectX::XMQuaternionRotationAxis(Conv_XM::Vector3f::WorldUp, DirectX::XMConvertToRadians(-90.0f));
		break;
	default:
		break;
	}

	//正規化する
	Conv_XM::Vector3f nextVec = DirectX::XMVector3Rotate(cameraForward, q1);
	nextVec = DirectX::XMVector3Normalize(nextVec);

	//0ベクトルかどうかエラーチェック
	float error = Conv_XM::Vector3f::LengthSq(nextVec);
	if (error < Math::k_Epsilon)
	{
		return;
	}

	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(actorForward, nextVec);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(actorForward, nextVec);
	float angle = xmangle.x;

	Conv_XM::Vector4f NextVecQuaternion;

	// 同じ方角なら
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
				//外積を求め回転軸を求める
				Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

				//クォータニオンを求める
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
			}
		}
		else
		{
			if (fabsf(angle) > DirectX::XMConvertToRadians(rotationLimit))
			{
				angle = DirectX::XMConvertToRadians(rotationLimit);
			}
			//外積を求め回転軸を求める
			Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

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
	}

	//求めたクォータニオンを合成する
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
	// 前ベクトルと目標ベクトルのなす角を算出
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

	//正規化する
	Conv_XM::Vector3f nextVec = DirectX::XMVector3Rotate(cameraForward, q1);
	nextVec = DirectX::XMVector3Normalize(nextVec);

	//0ベクトルかどうかエラーチェック
	float error = Conv_XM::Vector3f::LengthSq(nextVec);
	if (error < Math::k_Epsilon)
	{
		return;
	}

	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(actorForward, nextVec);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(actorForward, nextVec);
	float angle = xmangle.x;

	Conv_XM::Vector4f NextVecQuaternion;

	// 同じ方角なら
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
				//外積を求め回転軸を求める
				Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

				//クォータニオンを求める
				NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
			}
		}
		else
		{
			if (fabsf(angle) > DirectX::XMConvertToRadians(rotationLimit))
			{
				angle = DirectX::XMConvertToRadians(rotationLimit);
			}
			//外積を求め回転軸を求める
			Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(actorForward, nextVec);

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
	}

	//求めたクォータニオンを合成する
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


	//内積と角度を算出
	float dot = Conv_XM::Vector3f::Dot(Conv_XM::Vector3f::WorldForward, dif);
	Conv_XM::Vector3f xmangle = DirectX::XMVector3AngleBetweenNormals(Conv_XM::Vector3f::WorldForward, dif);
	float angle = xmangle.x;

	Conv_XM::Vector4f NextVecQuaternion;

	// 同じ方角なら
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
		//外積を求め回転軸を求める
		Conv_XM::Vector3f axis = Conv_XM::Vector3f::Cross(Conv_XM::Vector3f::WorldForward, dif);

		//クォータニオンを求める
		NextVecQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);
	}

	//求めたクォータニオンを合成する
	m_MobileSuit->SetQuaternion(NextVecQuaternion);
}
