
#include"../system/main.h"
#include "title_scene.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/actor/ui/title_scene/title_scene_ui.h"
#include"../framework/actor/weapon/beam_saber.h"
#include"../framework/actor/fire/fire.h"
#include"../framework/actor/stage/sideseven_object.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../framework/component/audio_component/audio_component.h"
#include"../framework/component/mesh_component/mesh_field_component.h"
#include"../framework/component/mesh_component/animation_model_component.h"
#include"../framework/component/mesh_component/model_component.h"
#include"../framework/component/mesh_component/instancing_model_component.h"
#include"../framework/component/mesh_component/debug/obb_debug_view_component.h"
#include"../system/fade.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../framework/actor/camera/debug_camera.h"
#include"../particle/particle_manager.h"
#include"../system/audio_manager.h"
#include"../directx/resource/model_manager.h"
#include"../framework/component/mesh_component/skydome_component.h"
#include"../framework/component/light_component/directional_light_component.h"
#include"../directx/resource/texture_manager.h"
#include"../directx/resource/animation_model_manager.h"

namespace TitleData
{
	//! オーディオデータ
	//! BGM
	static const char* TitleBGM = "asset/sound/BGM/BGM_Title.wav";

	//! 決定ボタンSE
	static const char* DecideSE = "asset/sound/SE/Decide001SE.wav";

	//! メッシュフィールド高さマップファイル名
	static const char* k_MeshFieldHeightMapFileName = "asset/mapdata/stagedata001.csv";
	//! メッシュフィールドアルファブレンドファイル名
	static const char* k_MeshFieldArufaBlendFileName001 = "asset/mapdata/textureblend001.csv";
	static const char* k_MeshFieldArufaBlendFileName002 = "asset/mapdata/textureblend002.csv";
	static const char* k_MeshFieldArufaBlendFileName003 = "asset/mapdata/textureblend003.csv";
	static const char* k_MeshFieldArufaBlendFileName004 = "asset/mapdata/textureblend004.csv";

	//! スカイドームのモデル名
	static const char* k_SkyDomeModelName = "asset/model/skydome/skydome.bin";
	static const Conv_XM::Vector3f k_SkyDomeScale = Conv_XM::Vector3f(150.0f, 150.0f, 150.0f);
	static const Conv_XM::Vector3f k_SkyDomeEuler = Conv_XM::Vector3f(-90.0f, 0.0f, 0.0f);
	static const Conv_XM::Vector3f k_SkyDomePosition = Conv_XM::Vector3f(0.0f, -10.0f, 0.0f);

	//! パーティクルのテクスチャ名
	static const char* k_ParticleTextureName = "asset/texture/foton.png";

	//! アニメーションモデルファイル名
	static const char* k_AnimationModelFileName = "asset/model/robot/guhu_anime.bin";

	//! アニメーションフレーム
	static constexpr int k_AmountFrame = 3;

	//! ビームサーベル001ポジション
	static const Conv_XM::Vector3f k_BeamSaber001Position[k_AmountFrame] = {
		{-1.2f,3.4f,0.5f},
		{-0.55f,2.8f, -0.1f},
		{-0.7f,2.4f,-0.8f},
	};

	//! ビームサーベル001角度
	static const Conv_XM::Vector3f k_BeamSaber001Euler[k_AmountFrame] = {
		{0.0f,0.0f, 10.0f},
		{0.0f,0.0f, -10.0f},
		{10.0f,0.0f, -30.0f},
	};

	//! ビームサーベル002ポジション
	static const Conv_XM::Vector3f k_BeamSaber002Position[k_AmountFrame] = {
		{1.0f,2.7f,-0.8f},
		{0.6f,2.9f, 0.0f},
	    {0.45f,2.5f, 0.6f},
	};

	//! ビームサーベル002角度
	static const Conv_XM::Vector3f k_BeamSaber002Euler[k_AmountFrame] = {
		{-70.0f,0.0f,0.0f},
		{-70.0f,15.0f,0.0f},
		{-70.0f,30.0f,0.0f},
	};

	//! 炎の数
	static constexpr int k_FireNum = 19;

	//! 炎のポジション
	static const Conv_XM::Vector3f k_FireEuler[k_FireNum] = {
		{ 0.0f, 0.0f, 0.0f},
		{ 0.0f, 10.0f, 0.0f},
		{ 0.0f, 20.0f, 0.0f},
		{ 0.0f, 30.0f, 0.0f},
		{ 0.0f, 40.0f, 0.0f},
		{ 0.0f, 50.0f, 0.0f},
		{ 0.0f, 60.0f, 0.0f},
		{ 0.0f, 70.0f, 0.0f},
		{ 0.0f, 80.0f, 0.0f},
		{ 0.0f, 90.0f, 0.0f},
		{ 0.0f, -10.0f, 0.0f},
		{ 0.0f, -20.0f, 0.0f},
		{ 0.0f, -30.0f, 0.0f},
		{ 0.0f, -40.0f, 0.0f},
		{ 0.0f, -50.0f, 0.0f},
		{ 0.0f, -60.0f, 0.0f},
		{ 0.0f, -70.0f, 0.0f},
		{ 0.0f, -80.0f, 0.0f},
		{ 0.0f, -90.0f, 0.0f},
	};

	//! カウントレート
	static float k_CountRaito = 0.1f;

	//! カメラアニメーションデータ
	static constexpr int k_CameraFrame = 2;
	static Conv_XM::Vector3f k_CameraPos[k_CameraFrame] =
	{
		{0.0f, 3.5f, -40.0f},
		{0.0f, 3.5f, -4.0f},
	};
	static Conv_XM::Vector3f k_CameraEuler[k_CameraFrame] =
	{
		{80.0f, 0.0f, 0.0f},
		{20.0f, 0.0f, 0.0f},
	};
}

TitleScene::TitleScene()
	:m_IsSceneChange(false)
	, m_Fade(nullptr)
	, m_Emitter001{}
	, m_Emitter002{}
	, m_Frame(0)
	, beamSaber001(nullptr)
	, beamSaber002(nullptr)
	, animeModelComp001(nullptr)
	, animeModelComp002(nullptr)
	, directionalLightComponent(nullptr)
	, directionalLightData{}
	, quaternionCameraComponent(nullptr)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::LoadData()
{
	// オーディオを設定
	AudioManager& audioManager = AudioManager::GetInstance();
	// オーディオデータ読み込み
	audioManager.Load(TitleData::TitleBGM);
	audioManager.Load(TitleData::DecideSE);
	// 再生
	audioManager.Play(TitleData::TitleBGM, true);

	// コリジョンデバッグビューを非表示
	OBBDebugViewComponent::SetIsUseFlag(false);

	// フェード取得
	m_Fade = SceneManager::GetInstance().GetFade();
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_Fadeがnullptr");
		return;
	}
	// フェード開始
	m_Fade->SetFade(Fade::FADE_OUT, 60);

	// パーティクルマネージャー取得
	ParticleManager& particleManager = ParticleManager::GetInstance();
	// パーティクルの
	particleManager.SetDebugCountView(false);

	// パーティクルのテクスチャ読み込み
	ID3D11ShaderResourceView* particleSRV = nullptr;
	particleSRV = TextureManager::GetInstance().Load(TitleData::k_ParticleTextureName);
	// nullチェック
	if (particleSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData particleSRVがnullptr");
		return;
	}
	// パーティクルを作成
	particleManager.CreateParticle(ParticleManagerData::k_BeamParticleName, ParticleManagerData::k_MaxBeamRifleParticleNum, particleSRV);

	// その他アクターマネージャー作成
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if (otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData otherActorManagerがnullptr");
		return;
	}

	// カメラを作成
	Actor* CameraActor = otherActorManager->CreateActor<Actor>();
	if (CameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData CameraActorがnullptr");
		return;
	}
	// クォータニオンカメラコンポーネント作成
	quaternionCameraComponent = CameraActor->CreateComponent<QuaternionCameraComponent>();
	if (quaternionCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData quaternionCameraComponentがnullptr");
		return;
	}
	// クォータニオンカメラコンポーネントにデータ入力
	quaternionCameraComponent->SetPosition(Conv_XM::Vector3f(0.0f, 3.5f, -4.0f));
	quaternionCameraComponent->SetEuler(20.0f, 0.0f, 0.0f);
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManagerがnullptr");
		return;
	}
	// シーンマネージャーにカメラセット
	m_SceneRenderManager->SetMainCameraComponent(quaternionCameraComponent);

	// ライトカメラを作成
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActorがnullptr");
		return;
	}
	// ライトカメラコンポーネント作成
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActorがnullptr");
		return;
	}
	// ライトカメラにパラメーター設定
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 10.0f, -5.0f));
	lightCamera->SetEuler(60.0f, 0.0f, 0.0f);
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManagerがnullptr");
		return;
	}
	// シーンレンダーにライトカメラ登録
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);

	//　ディレクショナルライト作成
	Actor* directionalLightActor = otherActorManager->CreateActor<Actor>();
	if (directionalLightActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightActorがnullptr");
		return;
	}
	// ディレクショナルライトコンポーネント作成
	directionalLightComponent = directionalLightActor->CreateComponent<DirectionalLightComponent>();
	if (directionalLightComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightComponentがnullptr");
		return;
	}
	// ディレクショナルライトのデータ設定
	directionalLightData = {};
	directionalLightData.Direction = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
	directionalLightData.Color = Conv_XM::Vector3f(2.0f, 2.0f, 2.0f);
	directionalLightData.Ambient = Conv_XM::Vector3f(0.4f, 0.4f, 0.4f);
	directionalLightComponent->SetLight(directionalLightData);
	directionalLightComponent->SetEuler(60.0f, 0.0f, 0.0f);

	// メッシュフィールドのテクスチャ読み込み
	MeshFieldComponent::LoadTexture();
	// メッシュフィールドの作成
	Actor* meshfieldActor = otherActorManager->CreateActor<Actor>();
	if (meshfieldActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldActorがnullptr");
		return;
	}
	// メッシュフィールドコンポーネント作成
	MeshFieldComponent* meshfieldComponent = meshfieldActor->CreateComponent<MeshFieldComponent>();
	if (meshfieldComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldComponentがnullptr");
		return;
	}
	// メッシュフィールド高さマップ読み込み
	meshfieldComponent->LoadHeightMapFile(TitleData::k_MeshFieldHeightMapFileName);
	// メッシュフィールドアルファブレンドファイル読み込み
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName001, 1);
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName002, 2);
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName003, 3);
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName004, 4);
	// メッシュフィールドマテリアル設定
	CBufferData::Material material = {};
	material.Metaric = 0.3f;
	material.Smoothness = 0.1f;
	meshfieldComponent->SetMaterial(material);
	meshfieldComponent->SetIsTitle(true);

	// モデルマネージャー取得
	ModelManager& modelManager = ModelManager::GetInstance();
	// スカイドームを作成
	// スカイドームモデルファイル読み込み
	Model* skyDomeModel = modelManager.Load(TitleData::k_SkyDomeModelName);
	if (skyDomeModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeModelがnullptr");
		return;
	}
	// スカイドーム用アクター作成
	Actor* skyDomeActor = otherActorManager->CreateActor<Actor>();
	if (skyDomeActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeActorがnullptr");
		return;
	}
	// スカイドームコンポーネント作成
	SkyDomeomponent* skyDomeComponent = skyDomeActor->CreateComponent<SkyDomeomponent>();
	if (skyDomeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeComponentがnullptr");
		return;
	}
	// スカイドームパラメーター設定
	skyDomeActor->SetScale(TitleData::k_SkyDomeScale);
	skyDomeComponent->SetModel(skyDomeModel);
	Conv_XM::Vector4f skydomeQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(TitleData::k_SkyDomeEuler.x), TitleData::k_SkyDomeEuler.y, TitleData::k_SkyDomeEuler.z);
	skyDomeActor->SetQuaternion(skydomeQuaternion);
	skyDomeActor->SetPosition(TitleData::k_SkyDomePosition);

	//アニメーションモデルマネージャー取得
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	// アニメモデルデータ作成
	Actor* animeModel001 = otherActorManager->CreateActor<Actor>();
	animeModel001->SetPosition(Conv_XM::Vector3f(-2.0f, 3.7f, 0.0f));
	AnimationModel* animeModel = animationModelManager.Load(TitleData::k_AnimationModelFileName);
	animeModelComp001 = animeModel001->CreateComponent<AnimationModelComponent>();
	animeModelComp001->SetAnimationModel(animeModel);
	Conv_XM::Vector4f modelOffsetQ001 = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(90.0f), 0.0f);
	animeModelComp001->SetOffsetQuaternion(modelOffsetQ001);
	animeModelComp001->SetOffsetScale(Conv_XM::Vector3f(0.4f, 0.4f, 0.4f));
	animeModelComp001->SetAnimation("attack");
	animeModelComp001->SetIsShadowInput(true);
	material.Metaric = 0.0f;
	material.Smoothness = 1.0f;
	animeModelComp001->SetMaterial(material);
	animeModelComp001->SetCurrentFrame(15);
	animeModelComp001->SetCounetSpeed(TitleData::k_CountRaito);

	// アニメモデルデータ作成
	Actor* animeModel002 = otherActorManager->CreateActor<Actor>();
	animeModel002->SetPosition(Conv_XM::Vector3f(2.0f, 3.7f, 0.0f));
	animeModelComp002 = animeModel002->CreateComponent<AnimationModelComponent>();
	animeModelComp002->SetAnimationModel(animeModel);
	Conv_XM::Vector4f modelOffsetQ002 = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), 0.0f);
	animeModelComp002->SetOffsetQuaternion(modelOffsetQ002);
	animeModelComp002->SetOffsetScale(Conv_XM::Vector3f(0.4f, 0.4f, 0.4f));
	animeModelComp002->SetAnimation("attack001");
	animeModelComp002->SetIsShadowInput(true);
	animeModelComp002->SetMaterial(material);
	animeModelComp002->SetCurrentFrame(15);
	animeModelComp002->SetCounetSpeed(TitleData::k_CountRaito);

	// ビームサーベルモデル読み込み
	BeamSaber::LoadModel();
	// ビームサーベル作成
	beamSaber001 = otherActorManager->CreateActor<BeamSaber>();
	beamSaber001->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	beamSaber001->SetPosition(TitleData::k_BeamSaber001Position[0]);
	beamSaber001->SetEuler(TitleData::k_BeamSaber001Euler[0].x, TitleData::k_BeamSaber001Euler[0].y, TitleData::k_BeamSaber001Euler[0].z);

	beamSaber002 = otherActorManager->CreateActor<BeamSaber>();
	beamSaber002->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	beamSaber002->SetPosition(TitleData::k_BeamSaber002Position[0]);
	beamSaber002->SetColor(Conv_XM::Vector4f(0.3f, 1.0f, 0.3f, 1.0f));
	beamSaber002->SetEuler(TitleData::k_BeamSaber002Euler[0].x, TitleData::k_BeamSaber002Euler[0].y, TitleData::k_BeamSaber002Euler[0].z);

	// パーティクルエミッター作成
	m_Emitter001.Color = Conv_XM::Vector4f(1.0f,0.3f,0.3f,1.0f);
	m_Emitter001.EmitterPosition = Conv_XM::Vector3f(0.0f, 3.0f, 0.0f);
	m_Emitter001.EmitterType = CBufferData::ET_SPHERE;
	m_Emitter001.IsUse = 1;
	m_Emitter001.MaxSpawnParticlesThisFrame = 1;
	m_Emitter001.ParticleLifeSpan = 400;
	m_Emitter001.Size = Conv_XM::Vector2f(0.8f, 0.8f);
	m_Emitter001.Speed = 0.025f;
	m_Emitter001.PositionVariance = Conv_XM::Vector3f(0.01f, 0.01f, 0.01f);
	m_Emitter001.SizeOverLifeTime = 1;

	// エミッターを登録
	m_Emitter001.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter001);

	// パーティクルエミッター作成
	m_Emitter002.Color = Conv_XM::Vector4f(0.3f, 1.0f, 0.3f, 1.0f);
	m_Emitter002.EmitterPosition = Conv_XM::Vector3f(0.0f, 3.0f, 0.0f);
	m_Emitter002.EmitterType = CBufferData::ET_SPHERE;
	m_Emitter002.IsUse = 1;
	m_Emitter002.MaxSpawnParticlesThisFrame = 1;
	m_Emitter002.ParticleLifeSpan = 400;
	m_Emitter002.Size = Conv_XM::Vector2f(0.8f, 0.8f);
	m_Emitter002.Speed = 0.025f;
	m_Emitter002.PositionVariance = Conv_XM::Vector3f(0.01f, 0.01f, 0.01f);
	m_Emitter002.SizeOverLifeTime = 1;

	// エミッターを登録
	m_Emitter002.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter002);

	// タイトルUIテクスチャ読み込み
	TitleSceneUI::LoadTexture();
	// タイトルUI作成
	otherActorManager->CreateActor<TitleSceneUI>();

	// ファイア作成
	Conv_XM::Vector3f firepos, firestanpos;
	Conv_XM::Vector4f fire_q;
	firestanpos = Conv_XM::Vector3f(0.0f, 0.0f, 5.0f);
	for (int i = 0; i < TitleData::k_FireNum; i++)
	{
		Fire* fire;
		fire = otherActorManager->CreateActor<Fire>();
		
		fire_q = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(TitleData::k_FireEuler[i].x), DirectX::XMConvertToRadians(TitleData::k_FireEuler[i].y), DirectX::XMConvertToRadians(TitleData::k_FireEuler[i].z));
		firepos = DirectX::XMVector3Rotate(firestanpos, fire_q);

		fire->SetPosition(firepos);
	}
}

void TitleScene::UnloadData()
{
	// オーディオ破棄
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Unload(TitleData::TitleBGM);
	audioManager.Unload(TitleData::DecideSE);

	// メッシュフィールド破棄
	MeshFieldComponent::UnloadTexture();

	// アニメーションマネージャー取得
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	// アニメーションモデル破棄
	animationModelManager.Unload(TitleData::k_AnimationModelFileName);

	// ビームサーベルモデル破棄
	BeamSaber::UnloadModel();

	// タイトルUIテクスチャ破棄
	TitleSceneUI::UnloadTexture();

	// モデルマネージャー取得
	ModelManager& modelManager = ModelManager::GetInstance();
	// スカイドーム破棄
	modelManager.Unload(TitleData::k_SkyDomeModelName);
}

void TitleScene::SetData()
{
}

void TitleScene::UpdateScene()
{
	// アニメーションカウント
	int animationCount = static_cast<int>(m_Frame * TitleData::k_CountRaito);

	// アニメーションレート
	float animationRaito = static_cast<float>(m_Frame) * TitleData::k_CountRaito;

	// アニメーションデータ計算
	float t;
	Conv_XM::Vector3f pos001, euler001, pos002, euler002, cameraPos, cameraEuler;

	if (animationRaito <= 6)
	{
		t = animationRaito / 6.0f;
		Conv_XM::Vector3f::Linear(&cameraPos, TitleData::k_CameraPos[0], TitleData::k_CameraPos[1], t);
		Conv_XM::Vector3f::Linear(&cameraEuler, TitleData::k_CameraEuler[0], TitleData::k_CameraEuler[1], t);
		
		quaternionCameraComponent->SetPosition(cameraPos);
		quaternionCameraComponent->SetEuler(cameraEuler.x, cameraEuler.y, cameraEuler.z);

		animeModelComp001->StopModel(true, 15);
		animeModelComp002->StopModel(true, 15);
		if (animationRaito == 6)
		{
			animeModelComp001->StopModel(false, 15);
			animeModelComp002->StopModel(false, 15);
			animeModelComp001->SetCurrentFrame(15);
			animeModelComp002->SetCurrentFrame(15);
		}
		pos001 = TitleData::k_BeamSaber001Position[0];
		pos002 = TitleData::k_BeamSaber002Position[0];
		euler001 = TitleData::k_BeamSaber001Euler[0];
		euler002 = TitleData::k_BeamSaber002Euler[0];
	}
	else if (animationCount < 22 + 6)
	{
		t = (animationCount - 6.0f) / 22.0f;
		Conv_XM::Vector3f::Linear(&pos001, TitleData::k_BeamSaber001Position[0], TitleData::k_BeamSaber001Position[1], t);
		Conv_XM::Vector3f::Linear(&euler001, TitleData::k_BeamSaber001Euler[0], TitleData::k_BeamSaber001Euler[1], t);
		Conv_XM::Vector3f::Linear(&pos002, TitleData::k_BeamSaber002Position[0], TitleData::k_BeamSaber002Position[1], t);
		Conv_XM::Vector3f::Linear(&euler002, TitleData::k_BeamSaber002Euler[0], TitleData::k_BeamSaber002Euler[1], t);
	}
	else if (animationCount <= 44 + 6)
	{
		t = (animationCount - 22 - 6) / 22.0f;
		Conv_XM::Vector3f::Linear(&pos001, TitleData::k_BeamSaber001Position[1], TitleData::k_BeamSaber001Position[2], t);
		Conv_XM::Vector3f::Linear(&euler001, TitleData::k_BeamSaber001Euler[1], TitleData::k_BeamSaber001Euler[2], t);
		Conv_XM::Vector3f::Linear(&pos002, TitleData::k_BeamSaber002Position[1], TitleData::k_BeamSaber002Position[2], t);
		Conv_XM::Vector3f::Linear(&euler002, TitleData::k_BeamSaber002Euler[1], TitleData::k_BeamSaber002Euler[2], t);
	}
	else
	{
		pos001 = TitleData::k_BeamSaber001Position[2];
		euler001 = TitleData::k_BeamSaber001Euler[2];

		pos002 = TitleData::k_BeamSaber002Position[2];
		euler002 = TitleData::k_BeamSaber002Euler[2];

		animeModelComp001->StopModel(true, 60);
		animeModelComp002->StopModel(true, 60);

		ParticleManager& particleManager = ParticleManager::GetInstance();

		particleManager.SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter001.MyIndex);
		particleManager.SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter002.MyIndex);


	}

	// アニメーションデータ入力
	beamSaber001->SetEuler(euler001.x, euler001.y, euler001.z);
	beamSaber001->SetPosition(pos001);
	beamSaber002->SetEuler(euler002.x, euler002.y, euler002.z);
	beamSaber002->SetPosition(pos002);


	// 入力管理
	InputManager& inputManager = InputManager::GetInstance();
	
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fadeがnullptr");
		return;
	}

	// 決定ボタンを押したら
	if (inputManager.GetIsTrigger(InputManager::IN_DECIDE) == true)
	{
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
		AudioManager::GetInstance().Play(TitleData::DecideSE, false);
		AudioManager::GetInstance().Stop(TitleData::TitleBGM);
	}

	// フェードし終えたらシーンを切り替える
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		SceneManager::GetInstance().SceneChange(SceneManager::SN_MODESELECT_SCENE);
	}

	// カウントアップ
	m_Frame++;
}

void TitleScene::CreateSceneFunction()
{
	// シーンレンダー作成
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void TitleScene::UnloadSceneFunction()
{
	// シーンレンダー破棄
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
