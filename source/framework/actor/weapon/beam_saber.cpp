
#include"../../../system/main.h"
#include "beam_saber.h"
#include"../../component/mesh_component/model_component.h"
#include"../../../particle/particle_manager.h"
#include"../../../directx/resource/model_manager.h"
#include"../../actor/mobilesuit/mobilesuit.h"
#include"../../component/collision_component/obb_collision_component.h"
#include"../../component/mesh_component/debug/obb_debug_view_component.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"

namespace BeamSaberData
{
	//! モデルファイル名
	static const char* k_ModelFileName = "asset/model/beamsaber/beamsaber.bin";
	
	static Conv_XM::Vector3f BeamSaber_Handle_OffsetScale = Conv_XM::Vector3f(0.1f, 0.1f, 0.4f);

	//! 弾の色
	static const Conv_XM::Vector4f k_DefaultParticleColor = Conv_XM::Vector4f(1.0f, 0.3f, 0.8f, 1.0f);
	//! 1フレーム中に発生させるパーティクル数
	static constexpr int k_DefaultSpawnNum = 1;
	//! ライフ
	static constexpr float k_DefaultLife = 5.0f;
	//! 速度
	static constexpr float k_ParticleSpeed = 0.025f;
	//! パーティクルサイズ
	static const Conv_XM::Vector2f k_ParticleSize = Conv_XM::Vector2f(0.2f, 0.2f);
	//! パーテイクルの速度
	static const Conv_XM::Vector3f k_ParticleDirection = Conv_XM::Vector3f(0.0f, 1.0f, 0.0f);
	//! 使用エミッタータイプ
	static const CBufferData::EmitterType k_ParticleEmitterType = CBufferData::ET_RECT;

}

namespace KakutoData
{
	//! NKakutou001　最初のポジション
	static const Conv_XM::Vector3f k_NKakutou001PositionFirst = Conv_XM::Vector3f(-0.25f, 1.0f, 0.7f);

	//! NKakutou001　最初の回転
	static const Conv_XM::Vector4f k_NKakutou001QuaternionFirst = Conv_XM::Vector4f::MakeQuaternionFromEuler(0.0f, 0.0f, 0.0f);

	//! NKakutou001　2のポジション
	static const Conv_XM::Vector3f k_NKakutou001PositionSecond = Conv_XM::Vector3f(-0.25f, -1.0f, 0.7f);

	//! NKakutou001　2の回転
	static const Conv_XM::Vector4f k_NKakutou001QuaternionSecond = Conv_XM::Vector4f::MakeQuaternionFromEuler(90.0f, 0.0f, 0.0f);


	//! NKakutou002　最初のポジション
	static const Conv_XM::Vector3f k_NKakutou002PositionFirst = Conv_XM::Vector3f(-0.25f, -1.0f, 0.7f);

	//! NKakutou002　最初の回転
	static const Conv_XM::Vector4f k_NKakutou002QuaternionFirst = Conv_XM::Vector4f::MakeQuaternionFromEuler(90.0f, 0.0f, 0.0f);

	//! NKakutou002　2のポジション
	static const Conv_XM::Vector3f k_NKakutou002PositionSecond = Conv_XM::Vector3f(-0.25f, 1.0f, 0.7f);

	//! NKakutou002　2の回転
	static const Conv_XM::Vector4f k_NKakutou002QuaternionSecond = Conv_XM::Vector4f::MakeQuaternionFromEuler(0.0f, 0.0f, 0.0f);


	//! NKakutou003　最初のポジション
	static const Conv_XM::Vector3f k_NKakutou003PositionFirst = Conv_XM::Vector3f(-0.25f, 1.0f, 0.7f);

	//! NKakutou003　最初の回転
	static const Conv_XM::Vector4f k_NKakutou003QuaternionFirst = Conv_XM::Vector4f::MakeQuaternionFromEuler(0.0f, 0.0f, 30.0f);

	//! NKakutou003　2のポジション
	static const Conv_XM::Vector3f k_NKakutou003PositionSecond = Conv_XM::Vector3f(-0.25f, -1.0f, 0.7f);

	//! NKakutou003　2の回転
	static const Conv_XM::Vector4f k_NKakutou003QuaternionSecond = Conv_XM::Vector4f::MakeQuaternionFromEuler(90.0f, 0.0f, -30.0f);

}

Model* BeamSaber::m_BeamSaber_Handle_Model = nullptr;

void BeamSaber::LoadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();
	m_BeamSaber_Handle_Model = modelManager.Load(BeamSaberData::k_ModelFileName);
}

void BeamSaber::UnloadModel()
{
	ModelManager& modelManager = ModelManager::GetInstance();
	modelManager.Unload(BeamSaberData::k_ModelFileName);
	m_BeamSaber_Handle_Model = nullptr;
}

BeamSaber::BeamSaber()
	: m_OBBCollisionComponent(nullptr)
	, m_Emitter{}
	, m_ParticleEmitterIndex(-1)
	, m_LandingEffectManager(nullptr)
	, m_Color(BeamSaberData::k_DefaultParticleColor)
	, m_BeamSaberState(STATE_NONE)
	, m_BeamSaberAnimationCount(0)
	, m_BeamSaberMainEffectTime(5.0)
	, m_BeamSaberSubEffectTime(55.0)
{
	m_ActorType = ACTORTYPE_BEAMSABER_NKAKU001;
}

BeamSaber::~BeamSaber()
{
}

void BeamSaber::SetActive(bool flag)
{
	// 入力
	m_Active = flag;
}

void BeamSaber::Reset()
{
	m_BeamSaberAnimationCount = 0;
	m_BeamSaberState = STATE_NONE;
	SetActive(false);
}

void BeamSaber::StartNKakutou001()
{
	m_BeamSaberAnimationCount = 0;
	m_BeamSaberState = STATE_NKAKUTOU001;
	SetActive(true);
	m_ActorType = ACTORTYPE_BEAMSABER_NKAKU001;
}

void BeamSaber::StartNKakutou002()
{
	m_BeamSaberAnimationCount = 0;
	m_BeamSaberState = STATE_NKAKUTOU002;
	SetActive(true);
	m_ActorType = ACTORTYPE_BEAMSABER_NKAKU002;
}

void BeamSaber::StartNKakutou003()
{
	m_BeamSaberAnimationCount = 0;
	m_BeamSaberState = STATE_NKAKUTOU003;
	SetActive(true);
	m_ActorType = ACTORTYPE_BEAMSABER_NKAKU003;
}

void BeamSaber::SetParticleTime(float mainTime, float effectTime)
{
	m_BeamSaberMainEffectTime = mainTime;
	m_BeamSaberSubEffectTime = effectTime;
}

void BeamSaber::LoadActor()
{
	m_ModelComponent = CreateComponent<ModelComponent>();

	m_OBBCollisionComponent = CreateComponent<OBBCollisionComponent>();


	// OBBデバッグビュー作成
	CreateComponent<OBBDebugViewComponent>();
}

void BeamSaber::InitActor()
{
	if (m_ModelComponent == nullptr || m_BeamSaber_Handle_Model == nullptr)
	{
		Logger::GetInstance().SetLog("BeamSaber::InitActor nullptr");
		return;
	}
	m_ModelComponent->SetModel(m_BeamSaber_Handle_Model);
	m_ModelComponent->SetOffsetScale(BeamSaberData::BeamSaber_Handle_OffsetScale);
	Conv_XM::Vector4f q = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f);
	m_ModelComponent->SetOffsetQuaternion(q);
	m_ModelComponent->SetIsShadowInput(true);

	ParticleManager& particleManager = ParticleManager::GetInstance();
	for (int i = 0; i < BeamSaberData::k_MainEmitterNum; i++)
	{
		m_Emitter[i].Color = m_Color;
		m_Emitter[i].EmitterPosition = m_Position;
		m_Emitter[i].EmitterPosition.y += 0.05f * static_cast<float>(i);
		m_Emitter[i].EmitterType = BeamSaberData::k_ParticleEmitterType;
		m_Emitter[i].IsUse = 1;
		m_Emitter[i].MaxSpawnParticlesThisFrame = BeamSaberData::k_DefaultSpawnNum;
		m_Emitter[i].ParticleLifeSpan = m_BeamSaberMainEffectTime;
		m_Emitter[i].Size = BeamSaberData::k_ParticleSize;
		m_Emitter[i].Speed = 0.0f;
		m_Emitter[i].PositionVariance = Conv_XM::Vector3f(0.01f, 0.01f, 0.01f);

		// エミッターを登録
		m_Emitter[i].MyIndex = particleManager.RegisterParticleEmitter(m_Emitter[i]);
	}
	for (int i = BeamSaberData::k_MainEmitterNum; i < BeamSaberData::k_MainEmitterNum * 2; i++)
	{
		m_Emitter[i].Color = m_Color * 0.05f;
		m_Emitter[i].Color.w = 1.0f;
		m_Emitter[i].EmitterPosition = m_Position;
		m_Emitter[i].EmitterPosition.y += 0.05f * static_cast<float>(i - BeamSaberData::k_MainEmitterNum);
		m_Emitter[i].EmitterType = BeamSaberData::k_ParticleEmitterType;
		m_Emitter[i].IsUse = 1;
		m_Emitter[i].MaxSpawnParticlesThisFrame = BeamSaberData::k_DefaultSpawnNum;
		m_Emitter[i].ParticleLifeSpan = m_BeamSaberSubEffectTime;
		m_Emitter[i].Size = BeamSaberData::k_ParticleSize;
		m_Emitter[i].Speed = 0.0f;
		m_Emitter[i].PositionVariance = Conv_XM::Vector3f(0.01f, 0.01f, 0.01f);

		// エミッターを登録
		m_Emitter[i].MyIndex = particleManager.RegisterParticleEmitter(m_Emitter[i]);
	}

	m_OBBCollisionComponent->SetOffsetScale(Conv_XM::Vector3f(0.2f, 1.5f, 0.2f));
	m_OBBCollisionComponent->SetOffsetPosition(Conv_XM::Vector3f(0.0f, 1.2f, 0.0f));


	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* currentScene = sceneManager.GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("BeamSaber::InitActor currentSceneがnullptr");
		return;
	}
	SceneCollisionManager* sceneCollisionManager = currentScene->GetSceneCollisionManager();
	if (sceneCollisionManager)
	{
		//　コリジョンのセット
		sceneCollisionManager->SetBulletCollision(m_OBBCollisionComponent);

		SetActive(false);
	}


}

void BeamSaber::ProcessInputActor()
{
	if (m_Active == false)
	{
		return;
	}

	if (m_OwnerMobileSuit == nullptr)
	{

	}
	else
	{
		Conv_XM::Vector3f resPos = Conv_XM::Vector3f::Zero;
		Conv_XM::Vector4f resQ;
		float t = 0.0f;
		// 所有モビルスーツのポジション取得
		Conv_XM::Vector3f ownerPos = m_OwnerMobileSuit->GetPosition();
		Conv_XM::Vector4f ownerQuaternion = m_OwnerMobileSuit->GetQuaternion();

		Conv_XM::Vector3f offsetPos = Conv_XM::Vector3f::Zero;
		switch (m_BeamSaberState)
		{
		case BeamSaber::STATE_NONE:
			resPos = ownerPos;
			//SetActive(false);
			break;
		case BeamSaber::STATE_NKAKUTOU001:
			t = static_cast<float>(m_BeamSaberAnimationCount) / 80.0f;
			resQ = DirectX::XMQuaternionSlerp(KakutoData::k_NKakutou001QuaternionFirst, KakutoData::k_NKakutou001QuaternionSecond, t);
			resQ = DirectX::XMQuaternionMultiply(resQ, ownerQuaternion);

			SetQuaternion(resQ);

			Conv_XM::Vector3f::Linear(&offsetPos, KakutoData::k_NKakutou001PositionFirst, KakutoData::k_NKakutou001PositionSecond, t);
			offsetPos = DirectX::XMVector3Rotate(offsetPos, ownerQuaternion);
			resPos = ownerPos + offsetPos;
			break;
		case BeamSaber::STATE_NKAKUTOU002:
			t = static_cast<float>(m_BeamSaberAnimationCount) / 80.0f;
			resQ = DirectX::XMQuaternionSlerp(KakutoData::k_NKakutou002QuaternionFirst, KakutoData::k_NKakutou002QuaternionSecond, t);
			resQ = DirectX::XMQuaternionMultiply(resQ, ownerQuaternion);

			SetQuaternion(resQ);

			Conv_XM::Vector3f::Linear(&offsetPos, KakutoData::k_NKakutou002PositionFirst, KakutoData::k_NKakutou002PositionSecond, t);
			offsetPos = DirectX::XMVector3Rotate(offsetPos, ownerQuaternion);
			resPos = ownerPos + offsetPos;
			break;
		case BeamSaber::STATE_NKAKUTOU003:
			t = static_cast<float>(m_BeamSaberAnimationCount) / 80.0f;
			resQ = DirectX::XMQuaternionSlerp(KakutoData::k_NKakutou003QuaternionFirst, KakutoData::k_NKakutou003QuaternionSecond, t);
			resQ = DirectX::XMQuaternionMultiply(resQ, ownerQuaternion);

			SetQuaternion(resQ);

			Conv_XM::Vector3f::Linear(&offsetPos, KakutoData::k_NKakutou003PositionFirst, KakutoData::k_NKakutou003PositionSecond, t);
			offsetPos = DirectX::XMVector3Rotate(offsetPos, ownerQuaternion);
			resPos = ownerPos + offsetPos;
			break;
		default:
			break;
		}

		// ポジション入力
		SetPosition(resPos);

		// カウントアップ
		m_BeamSaberAnimationCount++;
	}

	Conv_XM::Vector3f stanvec = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldUp, m_Quaternion);
	for (int i = 0; i < BeamSaberData::k_MainEmitterNum; i++)
	{
		m_Emitter[i].EmitterPosition = m_Position;
		m_Emitter[i].EmitterPosition += (stanvec * static_cast<float>(i) * 0.05f);
		ParticleManager& particleManager = ParticleManager::GetInstance();
		particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter[i].MyIndex, m_Emitter[i]);
		particleManager.SpawnParticle(m_UseParticleName, m_Emitter[i].MyIndex);
	}
	for (int i = BeamSaberData::k_MainEmitterNum; i < BeamSaberData::k_MainEmitterNum * 2; i++)
	{
		m_Emitter[i].EmitterPosition = m_Position;
		m_Emitter[i].EmitterPosition += (stanvec * static_cast<float>(i - BeamSaberData::k_MainEmitterNum) * 0.05f);
		ParticleManager& particleManager = ParticleManager::GetInstance();
		particleManager.UpdateParticleEmitterConstantBuffer(m_Emitter[i].MyIndex, m_Emitter[i]);
		particleManager.SpawnParticle(m_UseParticleName, m_Emitter[i].MyIndex);
	}

}