
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
	//! �t�B�[���h�̃e�N�X�`���t�@�C���l�[��
	static const char* k_FieldTextureFileName = "asset/texture/s_grass.jpg";

	//! �X�J�C�h�[���̃��f���̃t�@�C���l�[��
	static const char* k_SkydomeModelFileName = "asset/model/skydome/skydome.bin";

	//! �X�t�B�A�̃��f���̃t�@�C���l�[��
	static const char* k_SphereModelFileName = "asset/model/sphere/sphere.bin";

	//! �p�[�e�B�N���e�N�X�`���l�[��
	static const char* k_ParticleTextureFileName = "asset/texture/foton.png";

	//! �f��BGM
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
	// �I�[�f�B�I���擾
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Load(DemoSceneData::k_DemoBGMFileName);
	audioManager.Play(DemoSceneData::k_DemoBGMFileName, true);

	// �t�F�[�h������
	m_Fade = SceneManager::GetInstance().GetFade();
	if (m_Fade)
	{
		m_Fade->SetFade(Fade::FADE_OUT, 60 * 1);
	}

	ParticleManager::GetInstance().SetDebugCountView(true);

	// �O���b�h���쐬
	OtherActorManager* actorManager = CreateActorManager<OtherActorManager>();
	if (actorManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData actorManager��nullptr");
		return;
	}

	Actor* grid = actorManager->CreateActor<Actor>();
	if (grid == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData grid��nullptr");
		return;
	}
	grid->CreateComponent<GridComponent>();

	// �t�B�[���h���쐬
	Actor* fieldActor = actorManager->CreateActor<Actor>();
	if (fieldActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData fieldActor��nullptr");
		return;
	}

	//! �t�B�[���h�e�N�X�`���ǂݍ���
	ID3D11ShaderResourceView* fieldSRV = TextureManager::GetInstance().Load(DemoSceneData::k_FieldTextureFileName);
	if (fieldSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData fieldSRV��nullptr");
		return;
	}
	fieldActor->SetScale(Conv_XM::Vector3f(100.0f, 1.0f, 100.0f));
	FieldComponent* fieldComponent = fieldActor->CreateComponent<FieldComponent>();
	if (fieldComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData fieldComponent��nullptr");
		return;
	}
	fieldComponent->SetSRV(fieldSRV);

	// �f�o�b�O�J�������쐬
	DebugCamera* debugCamera =  actorManager->CreateActor<DebugCamera>();
	if (debugCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData debugCamera��nullptr");
		return;
	}
	debugCamera->SetPosition(Conv_XM::Vector3f(0.0f, 5.0f, -10.0f));

	// ���C�g�J�������쐬
	Actor* lightCameraActor = actorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActor��nullptr");
		return;
	}
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActor��nullptr");
		return;
	}
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	lightCamera->SetEuler(90.0f, 0.0f, 0.0f);

	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManager��nullptr");
		return;
	}
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);

	// �f�B���N�V���i�����C�g
	Actor* dirLightActor = actorManager->CreateActor<Actor>();
	if (dirLightActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData dirLightActor��nullptr");
		return;
	}
	m_DirectionalLight = dirLightActor->CreateComponent<DirectionalLightComponent>();
	if (m_DirectionalLight == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_DirectionalLight��nullptr");
		return;
	}
	m_DirectionalLightData.Ambient = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
	m_DirectionalLightData.Color = Conv_XM::Vector3f(2.0f, 2.0f, 2.0f);
	m_DirectionalLightData.Direction = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
	m_DirectionalLight->SetLight(m_DirectionalLightData);

	// �X�t�B�A���쐬
	Model* sphereModel = ModelManager::GetInstance().Load(DemoSceneData::k_SphereModelFileName);
	if (sphereModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel��nullptr");
		return;
	}
	Actor* sphereModelActor001 = actorManager->CreateActor<Actor>();
	if (sphereModelActor001 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor001��nullptr");
		return;
	}
	sphereModelActor001->SetPosition(Conv_XM::Vector3f(-10.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel001 = sphereModelActor001->CreateComponent<ModelComponent>();
	if (sphereModel001 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel001��nullptr");
		return;
	}
	sphereModel001->SetModel(sphereModel);

	Actor* sphereModelActor002 = actorManager->CreateActor<Actor>();
	if (sphereModelActor002 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor002��nullptr");
		return;
	}
	sphereModelActor002->SetPosition(Conv_XM::Vector3f(-5.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel002 = sphereModelActor002->CreateComponent<ModelComponent>();
	if (sphereModel002 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel002��nullptr");
		return;
	}
	sphereModel002->SetModel(sphereModel);

	Actor* sphereModelActor003 = actorManager->CreateActor<Actor>();
	if (sphereModelActor003 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor003��nullptr");
		return;
	}
	sphereModelActor003->SetPosition(Conv_XM::Vector3f(0.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel003 = sphereModelActor003->CreateComponent<ModelComponent>();
	if (sphereModel003 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel003��nullptr");
		return;
	}
	sphereModel003->SetModel(sphereModel);

	Actor* sphereModelActor004 = actorManager->CreateActor<Actor>();
	if (sphereModelActor004 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor004��nullptr");
		return;
	}
	sphereModelActor004->SetPosition(Conv_XM::Vector3f(5.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel004 = sphereModelActor004->CreateComponent<ModelComponent>();
	if (sphereModel004 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel004��nullptr");
		return;
	}
	sphereModel004->SetModel(sphereModel);

	Actor* sphereModelActor005 = actorManager->CreateActor<Actor>();
	if (sphereModelActor005 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor005��nullptr");
		return;
	}
	sphereModelActor005->SetPosition(Conv_XM::Vector3f(10.0f, 2.0f, 5.0f));
	ModelComponent* sphereModel005 = sphereModelActor005->CreateComponent<ModelComponent>();
	if (sphereModel005 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModel005��nullptr");
		return;
	}
	sphereModel005->SetModel(sphereModel);

	Actor* sphereModelActor = actorManager->CreateActor<Actor>();
	if (sphereModelActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData sphereModelActor��nullptr");
		return;
	}
	sphereModelActor->SetPosition(Conv_XM::Vector3f(0.0f, 2.0f, 2.0f));
	m_SphereModel = sphereModelActor->CreateComponent<ModelComponent>();
	if (m_SphereModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SphereModel��nullptr");
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

	// �p�[�e�B�N���Z�b�g
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

	// �X�J�C�h�[���쐬
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
	// ImGui����
	ImGui::Begin(MyImGuiData::k_DebugWindowName);

	// �V�[���؂�ւ��p
	bool isPush = ImGui::Button("ModeSelectScene");

	ImGui::Text(u8"�Q�[���p�b�h�ړ�");
	ImGui::Text(u8"�\���L�[ �O�㍶�E�ړ�");
	ImGui::Text(u8"RB�ŏ㏸LB�ŉ��~");
	ImGui::Text(u8"X,Y,A,B ���_�ړ�");

	ImGui::Text(u8"�J�����ړ�");
	ImGui::Text(u8"WASDQE�L�[ �O�㍶�E�㉺�ړ�");
	ImGui::Text(u8"���L�[ ���_�ړ�");

	ImGui::Text(u8"���������V�F�[�_�[�̃f���V�[���ł��B���̋��̂̓��^���b�N�ƃX���[�X�l�X��ς��Ă��܂�");
	ImGui::Text(u8"������1�Ԗڂ̓��^���b�N:1.0f�X���[�X�l�X1.0f");
	ImGui::Text(u8"������2�Ԗڂ̓��^���b�N:0.0f�X���[�X�l�X1.0f");
	ImGui::Text(u8"������3�Ԗڂ̓��^���b�N:1.0f�X���[�X�l�X0.0f");
	ImGui::Text(u8"������4�Ԗڂ̓��^���b�N:0.5f�X���[�X�l�X0.5f");
	ImGui::Text(u8"������5�Ԗڂ̓��^���b�N:0.0f�X���[�X�l�X0.0f");
	ImGui::Text(u8"��Ԏ�O�̋��͈̂ȉ��̃p�����[�^�ɂ��ω����܂�");

	// �}�e���A��
	ImGui::SliderFloat("Metaric", &m_Metaric, 0.0f, 1.0f);
	ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.0f, 1.0f);

	// �f�B���N�V���i�����C�g
	ImGui::SliderFloat3("Ambient", m_Ambient, 0.0f, 1.0f);
	ImGui::SliderFloat3("Color", m_Color, 0.0f, 3.0f);
	ImGui::SliderFloat3("Direction", m_Direction, -180.0f, 180.0f);

	ImGui::Text(u8"GPU�p�[�e�B�N���ł��B10���p�[�e�B�N���قǏo���Ă���܂�");
	ImGui::Text(u8"�O�̃p�[�e�B�N���͓���̃h���[�R�[���ŕ`�悳��Ă���܂�");
	ImGui::Checkbox("ParticleSpawn", &m_ParticleUse);

	ImGui::End();

	// �}�e���A���Z�b�g
	m_Material.Metaric = m_Metaric;
	m_Material.Smoothness = m_Smoothness;


	if (m_SphereModel == nullptr)
	{
		Logger::GetInstance().SetLog("DemoScene::UpdateScene m_SphereModel��nullptr");
		return;
	}
	m_SphereModel->SetMaterial(m_Material);

	if (m_DirectionalLight == nullptr)
	{
		Logger::GetInstance().SetLog("DemoScene::UpdateScene m_DirectionalLight��nullptr");
		return;
	}

	// �f�B���N�V���i�����C�g�Z�b�g
	m_DirectionalLightData.Ambient = Conv_XM::Vector3f(m_Ambient[0], m_Ambient[1], m_Ambient[2]);
	m_DirectionalLightData.Color = Conv_XM::Vector3f(m_Color[0], m_Color[1], m_Color[2]);
	m_DirectionalLightData.Direction = Conv_XM::Vector3f(m_Direction[0], m_Direction[1], m_Direction[2]);
	m_DirectionalLight->SetLight(m_DirectionalLightData);
	m_DirectionalLight->SetEuler(m_Direction[0], m_Direction[1], m_Direction[2]);

	directionalLight->SetLightParameter(m_DirectionalLightData);
	directionalLight->SetEuler(m_Direction[0], m_Direction[1], m_Direction[2]);

	// GPU�p�[�e�B�N���X�|�[��
	if (m_ParticleUse == true)
	{
		ParticleManager::GetInstance().SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter001.MyIndex);
		ParticleManager::GetInstance().SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter002.MyIndex);
		ParticleManager::GetInstance().SpawnParticle(ParticleManagerData::k_BeamParticleName, m_Emitter003.MyIndex);
	}

	// �t�F�[�h�A�V�[���؂�ւ�
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fade��nullptr");
		return;
	}

	if (isPush == true)
	{
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
	}

	// �t�F�[�h���I������V�[����؂�ւ���
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		SceneManager::GetInstance().SceneChange(SceneManager::SN_MODESELECT_SCENE);

		AudioManager::GetInstance().Stop(DemoSceneData::k_DemoBGMFileName);
	}
}

void DemoScene::CreateSceneFunction()
{
	// �V�[�������_�[�쐬
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
