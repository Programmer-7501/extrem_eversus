
#include"../system/main.h"
#include "demo_scene.h"
#include"../framework/component/mesh_component/grid_component.h"
#include"../framework/component/mesh_component/skydome_component.h"
#include"../framework/component/mesh_component/field_component.h"
#include"../framework/component/cameracomponent/quaternion_camera_component.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../framework/component/light_component/directional_light_component.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../framework/actor/camera/debug_camera.h"
#include"../framework/actor/light/directional_light.h"
#include"../directx/resource/model_manager.h"
#include"../directx/resource/texture_manager.h"
#include"../framework/component/mesh_component/model_component.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../particle/particle_manager.h"
#include"../framework/scene/scene_manager.h"
#include"../system/fade.h"
#include"../system/myimgui.h"
#include"../system/audio_manager.h"

namespace DemoSceneData
{
	//! フィールドのテクスチャファイルネーム
	static const char* k_FieldTextureFileName = "asset/texture/s_grass.jpg";

	//! スカイドームのモデルのファイルネーム
	static const char* k_SkydomeModelFileName = "asset/model/skydome/skydome.bin";

	//! スフィアのモデルのファイルネーム
	static const char* k_SphereModelFileName = "asset/model/sphere/sphere.bin";

	//! パーティクルテクスチャネーム
	static const char* k_ParticleTextureFileName = "asset/texture/foton.png";

	//! デモBGM
	static const char* k_DemoBGMFileName = "asset/sound/BGM/DemoBGM.wav";
}

DemoScene::DemoScene()
	: m_Material{ {0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f},0.5f,0.5f,{0.0f,0.0f} }
	, m_SphereModel(nullptr)
	, m_Metaric(0.5f)
	, m_Smoothness(0.5f)
	, m_DirectionalLight(nullptr)
	, m_DirectionalLightData{{0.5f, 0.5f, 0.5f}, 0.0f, {2.0f, 2.0f, 2.0f},0.0f,{0.0f, -1.0f, 0.0f}, 0.0f}
	, m_Ambient{0.5f, 0.5f, 0.5f}
	, m_Color{2.0f, 2.0f, 2.0f}
	, m_Direction{0.0f, -1.0f, 0.0f}
	, m_MaxParticleNum(131072)
	, m_ParticleUse(true)
	, m_Emitter001{}
	, m_Fade(nullptr)
	, m_IsSceneChange(false)
{
}

DemoScene::~DemoScene()
{
}

DirectionalLight* directionalLight;

void DemoScene::LoadData()
{
	// オーディオを取得
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Load(DemoSceneData::k_DemoBGMFileName);
	audioManager.Play(DemoSceneData::k_DemoBGMFileName, true);

	// フェードさせる
	m_Fade = SceneManager::GetInstance().GetFade();
	if (m_Fade)
	{
		m_Fade->SetFade(Fade::FADE_OUT, 60 * 1);
	}

	ParticleManager::GetInstance().SetDebugCountView(true);

	// グリッドを作成
	OtherActorManager* actorManager = CreateActorManager<OtherActorManager>();
	if (actorManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData actorManagerがnullptr");
		return;
	}

	Actor* grid = actorManager->CreateActor<Actor>();
	if (grid == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData gridがnullptr");
		return;
	}
	grid->CreateComponent<GridComponent>();

	// フィールドを作成
	Actor* fieldActor = actorManager->CreateActor<Actor>();
	if (fieldActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData fieldActorがnullptr");
		return;
	}

	//! フィールドテクスチャ読み込み
	ID3D11ShaderResourceView* fieldSRV = TextureManager::GetInstance().Load(DemoSceneData::k_FieldTextureFileName);
	if (fieldSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData fieldSRVがnullptr");
		return;
	}
	fieldActor->SetScale(Conv_XM::Vector3f(100.0f, 1.0f, 100.0f));
	FieldComponent* fieldComponent = fieldActor->CreateComponent<FieldComponent>();
	if (fieldComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData fieldComponentがnullptr");
		return;
	}
	fieldComponent->SetSRV(fieldSRV);

	// デバッグカメラを作成
	DebugCamera* debugCamera =  actorManager->CreateActor<DebugCamera>();
	if (debugCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData debugCameraがnullptr");
		return;
	}
	debugCamera->SetPosition(Conv_XM::Vector3f(0.0f, 5.0f, -10.0f));

	// ライトカメラを作成
	Actor* lightCameraActor = actorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActorがnullptr");
		return;
	}
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActorがnullptr");
		return;
	}
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	lightCamera->SetEuler(90.0f, 0.0f, 0.0f);

	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManagerがnullptr");
		return;
	}
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);

	// ディレクショナルライト
	Actor* dirLightActor = actorManager->CreateActor<Actor>();
	if (dirLightActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData dirLightActorがnullptr");
		return;
	}
	m_DirectionalLight = dirLightActor->CreateComponent<DirectionalLightComponent>();
	if (m_DirectionalLight == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_DirectionalLightがnullptr");
		return;
	}
	m_DirectionalLightData.Ambient = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
	m_DirectionalLightData.Color = Conv_XM::Vector3f(2.0f, 2.0f, 2.0f);
	m_DirectionalLightData.Direction = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
	m_DirectionalLight->SetLight(m_DirectionalLightData);

	// スフィアを作成
	Model* sphereModel = ModelManager::GetInstance().Load(DemoSceneData::k_SphereModelFileName);
	if (sphereModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelがnullptr");
		return;
	}
	Actor* sphereModelActor001 = actorManager->CreateActor<Actor>();
	if (sphereModelActor001 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor001がnullptr");
		return;
	}
	sphereModelActor001->SetPosition(Conv_XM::Vector3f(-10.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel001 = sphereModelActor001->CreateComponent<ModelComponent>();
	if (sphereModel001 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel001がnullptr");
		return;
	}
	sphereModel001->SetModel(sphereModel);

	Actor* sphereModelActor002 = actorManager->CreateActor<Actor>();
	if (sphereModelActor002 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor002がnullptr");
		return;
	}
	sphereModelActor002->SetPosition(Conv_XM::Vector3f(-5.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel002 = sphereModelActor002->CreateComponent<ModelComponent>();
	if (sphereModel002 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel002がnullptr");
		return;
	}
	sphereModel002->SetModel(sphereModel);

	Actor* sphereModelActor003 = actorManager->CreateActor<Actor>();
	if (sphereModelActor003 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor003がnullptr");
		return;
	}
	sphereModelActor003->SetPosition(Conv_XM::Vector3f(0.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel003 = sphereModelActor003->CreateComponent<ModelComponent>();
	if (sphereModel003 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel003がnullptr");
		return;
	}
	sphereModel003->SetModel(sphereModel);

	Actor* sphereModelActor004 = actorManager->CreateActor<Actor>();
	if (sphereModelActor004 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor004がnullptr");
		return;
	}
	sphereModelActor004->SetPosition(Conv_XM::Vector3f(5.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel004 = sphereModelActor004->CreateComponent<ModelComponent>();
	if (sphereModel004 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel004がnullptr");
		return;
	}
	sphereModel004->SetModel(sphereModel);

	Actor* sphereModelActor005 = actorManager->CreateActor<Actor>();
	if (sphereModelActor005 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor005がnullptr");
		return;
	}
	sphereModelActor005->SetPosition(Conv_XM::Vector3f(10.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel005 = sphereModelActor005->CreateComponent<ModelComponent>();
	if (sphereModel005 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel005がnullptr");
		return;
	}
	sphereModel005->SetModel(sphereModel);

	Actor* sphereModelActor = actorManager->CreateActor<Actor>();
	if (sphereModelActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActorがnullptr");
		return;
	}
	sphereModelActor->SetPosition(Conv_XM::Vector3f(0.0f, 2.0f, 2.0f));
	m_SphereModel = sphereModelActor->CreateComponent<ModelComponent>();
	if (m_SphereModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SphereModelがnullptr");
		return;
	}
	m_SphereModel->SetModel(sphereModel);

	CBufferData::Material material = {};
	material.Metaric = 1.0f;
	material.Smoothness = 1.0f;
	sphereModel001->SetMaterial(material);
	material.Metaric = 0.0f;
	material.Smoothness = 1.0f;
	sphereModel002->SetMaterial(material);
	material.Metaric = 1.0f;
	material.Smoothness = 0.0f;
	sphereModel003->SetMaterial(material);
	material.Metaric = 0.5f;
	material.Smoothness = 0.5f;
	sphereModel004->SetMaterial(material);
	material.Metaric = 0.0f;
	material.Smoothness = 0.0f;
	sphereModel005->SetMaterial(material);

	// パーティクルセット
	ID3D11ShaderResourceView* particleSRV = nullptr;
	particleSRV = TextureManager::GetInstance().Load(DemoSceneData::k_ParticleTextureFileName);
	ParticleManager::GetInstance().CreateParticle(ParticleManagerData::k_BeamParticleName, m_MaxParticleNum, particleSRV);

	m_Emitter001.Color = Conv_XM::Vector4f(1.0f, 0.0f, 0.5f, 1.0f);
	m_Emitter001.EmitterPosition = Conv_XM::Vector3f(0.0f, 0.0f, 0.0f);
	m_Emitter001.EmitterType = CBufferData::ET_DEMO;
	m_Emitter001.IsUse = 1;
	m_Emitter001.MaxSpawnParticlesThisFrame = 700;
	m_Emitter001.ParticleLifeSpan = 40;
	m_Emitter001.ParticleVelocity = Conv_XM::Vector3f(0.0f, 0.5f, 0.0f);
	m_Emitter001.VelocityVariance = Conv_XM::Vector3f(0.2f, 0.1f, 0.2f);
	m_Emitter001.Gravity = 0.015f;
	m_Emitter001.Size = Conv_XM::Vector2f(0.1f, 0.1f);
	m_Emitter001.SizeOverLifeTime = 0;
	m_Emitter001.PositionVariance = Conv_XM::Vector3f(0.0f, 0.0f, 0.0f);
	m_Emitter001.MyIndex = ParticleManager::GetInstance().RegisterParticleEmitter(m_Emitter001);

	m_Emitter002 = m_Emitter001;
	m_Emitter002.Color = Conv_XM::Vector4f(0.0f, 1.0f, 0.4f, 1.0f);
	m_Emitter002.Gravity = 0.02f;
	m_Emitter002.ParticleVelocity = Conv_XM::Vector3f(0.0f, 0.4f, 0.0f);
	m_Emitter002.VelocityVariance = Conv_XM::Vector3f(0.2f, 0.1f, 0.2f);
	m_Emitter002.EmitterPosition = Conv_XM::Vector3f(5.0f, 0.0f, 0.0f);
	m_Emitter002.MaxSpawnParticlesThisFrame = 800;
	m_Emitter002.ParticleLifeSpan = 30;
	m_Emitter002.MyIndex = ParticleManager::GetInstance().RegisterParticleEmitter(m_Emitter002);

	m_Emitter003 = m_Emitter001;
	m_Emitter003.Color = Conv_XM::Vector4f(1.0f, 0.5f, 0.0f, 1.0f);
	m_Emitter003.Gravity = 0.01f;
	m_Emitter003.ParticleVelocity = Conv_XM::Vector3f(0.0f, 0.6f, 0.0f);
	m_Emitter003.VelocityVariance = Conv_XM::Vector3f(0.2f, 0.1f, 0.2f);
	m_Emitter003.EmitterPosition = Conv_XM::Vector3f(-5.0f, 0.0f, 0.0f);
	m_Emitter003.MaxSpawnParticlesThisFrame = 600;
	m_Emitter003.ParticleLifeSpan = 100;
	m_Emitter003.MyIndex = ParticleManager::GetInstance().RegisterParticleEmitter(m_Emitter003);

	// スカイドーム作成
	Model* skydomeModel = ModelManager::GetInstance().Load(DemoSceneData::k_SkydomeModelFileName);
	Actor* skydomeActor = actorManager->CreateActor<Actor>();
	SkyDomeomponent* skydome = skydomeActor->CreateComponent<SkyDomeomponent>();
	skydomeActor->SetScale(Conv_XM::Vector3f(50.0f, 50.0f, 50.0f));
	skydome->SetModel(skydomeModel);
	Conv_XM::Vector4f skydomeq = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);
	skydomeActor->SetQuaternion(skydomeq);
	skydomeActor->SetPosition(Conv_XM::Vector3f(0.0f, -10.0f, 0.0f));


	directionalLight = actorManager->CreateActor<DirectionalLight>();
}

void DemoScene::UnloadData()
{
	TextureManager::GetInstance().Unload(DemoSceneData::k_FieldTextureFileName);
	TextureManager::GetInstance().Unload(DemoSceneData::k_ParticleTextureFileName);
	ModelManager::GetInstance().Unload(DemoSceneData::k_SphereModelFileName);
}

void DemoScene::SetData()
{
}

void DemoScene::UpdateScene()
{
	// ImGui入力
	ImGui::Begin(MyImGuiData::k_DebugWindowName);

	// シーン切り替え用
	bool isPush = ImGui::Button("ModeSelectScene");

	ImGui::Text(u8"ゲームパッド移動");
	ImGui::Text(u8"十字キー 前後左右移動");
	ImGui::Text(u8"RBで上昇LBで下降");
	ImGui::Text(u8"X,Y,A,B 視点移動");

	ImGui::Text(u8"カメラ移動");
	ImGui::Text(u8"WASDQEキー 前後左右上下移動");
	ImGui::Text(u8"矢印キー 視点移動");

	ImGui::Text(u8"実装したシェーダーのデモシーンです。この球体はメタリックとスムースネスを変えています");
	ImGui::Text(u8"左から1番目はメタリック:1.0fスムースネス1.0f");
	ImGui::Text(u8"左から2番目はメタリック:0.0fスムースネス1.0f");
	ImGui::Text(u8"左から3番目はメタリック:1.0fスムースネス0.0f");
	ImGui::Text(u8"左から4番目はメタリック:0.5fスムースネス0.5f");
	ImGui::Text(u8"左から5番目はメタリック:0.0fスムースネス0.0f");
	ImGui::Text(u8"一番手前の球体は以下のパラメータにより変化します");

	// マテリアル
	ImGui::SliderFloat("Metaric", &m_Metaric, 0.0f, 1.0f);
	ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.0f, 1.0f);

	// ディレクショナルライト
	ImGui::SliderFloat3("Ambient", m_Ambient, 0.0f, 1.0f);
	ImGui::SliderFloat3("Color", m_Color, 0.0f, 3.0f);
	ImGui::SliderFloat3("Direction", m_Direction, -180.0f, 180.0f);

	ImGui::Text(u8"GPUパーティクルです。10万パーティクルほど出しております");
	ImGui::Text(u8"三つのパーティクルは同一のドローコールで描画されております");
	ImGui::Checkbox("ParticleSpawn", &m_ParticleUse);

	ImGui::End();

	// マテリアルセット
	m_Material.Metaric = m_Metaric;
	m_Material.Smoothness = m_Smoothness;


	if (m_SphereModel == nullptr)
	{
		Logger::GetInstance().SetLog("DemoScene::UpdateScene m_SphereModelがnullptr");
		return;
	}
	m_SphereModel->SetMaterial(m_Material);

	if (m_DirectionalLight == nullptr)
	{
		Logger::GetInstance().SetLog("DemoScene::UpdateScene m_DirectionalLightがnullptr");
		return;
	}

	// ディレクショナルライトセット
	m_DirectionalLightData.Ambient = Conv_XM::Vector3f(m_Ambient[0], m_Ambient[1], m_Ambient[2]);
	m_DirectionalLightData.Color = Conv_XM::Vector3f(m_Color[0], m_Color[1], m_Color[2]);
	m_DirectionalLightData.Direction = Conv_XM::Vector3f(m_Direction[0], m_Direction[1], m_Direction[2]);
	m_DirectionalLight->SetLight(m_DirectionalLightData);
	m_DirectionalLight->SetEuler(m_Direction[0], m_Direction[1], m_Direction[2]);

	directionalLight->SetLightParameter(m_DirectionalLightData);
	directionalLight->SetEuler(m_Direction[0], m_Direction[1], m_Direction[2]);

	// GPUパーティクルスポーン
	if (m_ParticleUse == true)
	{
		ParticleManager::GetInstance().SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter001.MyIndex);
		ParticleManager::GetInstance().SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter002.MyIndex);
		ParticleManager::GetInstance().SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter003.MyIndex);
	}

	// フェード、シーン切り替え
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fadeがnullptr");
		return;
	}

	if (isPush == true)
	{
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
	}

	// フェードし終えたらシーンを切り替える
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		SceneManager::GetInstance().SceneChange(SceneManager::SN_MODESELECT_SCENE);

		AudioManager::GetInstance().Stop(DemoSceneData::k_DemoBGMFileName);
	}
}

void DemoScene::CreateSceneFunction()
{
	// シーンレンダー作成
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void DemoScene::UnloadSceneFunction()
{
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
