
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
	//! �I�[�f�B�I�f�[�^
	//! BGM
	static const char* TitleBGM = "asset/sound/BGM/BGM_Title.wav";

	//! ����{�^��SE
	static const char* DecideSE = "asset/sound/SE/Decide001SE.wav";

	//! ���b�V���t�B�[���h�����}�b�v�t�@�C����
	static const char* k_MeshFieldHeightMapFileName = "asset/mapdata/stagedata001.csv";
	//! ���b�V���t�B�[���h�A���t�@�u�����h�t�@�C����
	static const char* k_MeshFieldArufaBlendFileName001 = "asset/mapdata/textureblend001.csv";
	static const char* k_MeshFieldArufaBlendFileName002 = "asset/mapdata/textureblend002.csv";
	static const char* k_MeshFieldArufaBlendFileName003 = "asset/mapdata/textureblend003.csv";
	static const char* k_MeshFieldArufaBlendFileName004 = "asset/mapdata/textureblend004.csv";

	//! �X�J�C�h�[���̃��f����
	static const char* k_SkyDomeModelName = "asset/model/skydome/skydome.bin";
	static const Conv_XM::Vector3f k_SkyDomeScale = Conv_XM::Vector3f(150.0f, 150.0f, 150.0f);
	static const Conv_XM::Vector3f k_SkyDomeEuler = Conv_XM::Vector3f(-90.0f, 0.0f, 0.0f);
	static const Conv_XM::Vector3f k_SkyDomePosition = Conv_XM::Vector3f(0.0f, -10.0f, 0.0f);

	//! �p�[�e�B�N���̃e�N�X�`����
	static const char* k_ParticleTextureName = "asset/texture/foton.png";

	//! �A�j���[�V�������f���t�@�C����
	static const char* k_AnimationModelFileName = "asset/model/robot/guhu_anime.bin";

	//! �A�j���[�V�����t���[��
	static constexpr int k_AmountFrame = 3;

	//! �r�[���T�[�x��001�|�W�V����
	static const Conv_XM::Vector3f k_BeamSaber001Position[k_AmountFrame] = {
		{-1.2f,3.4f,0.5f},
		{-0.55f,2.8f, -0.1f},
		{-0.7f,2.4f,-0.8f},
	};

	//! �r�[���T�[�x��001�p�x
	static const Conv_XM::Vector3f k_BeamSaber001Euler[k_AmountFrame] = {
		{0.0f,0.0f, 10.0f},
		{0.0f,0.0f, -10.0f},
		{10.0f,0.0f, -30.0f},
	};

	//! �r�[���T�[�x��002�|�W�V����
	static const Conv_XM::Vector3f k_BeamSaber002Position[k_AmountFrame] = {
		{1.0f,2.7f,-0.8f},
		{0.6f,2.9f, 0.0f},
	    {0.45f,2.5f, 0.6f},
	};

	//! �r�[���T�[�x��002�p�x
	static const Conv_XM::Vector3f k_BeamSaber002Euler[k_AmountFrame] = {
		{-70.0f,0.0f,0.0f},
		{-70.0f,15.0f,0.0f},
		{-70.0f,30.0f,0.0f},
	};

	//! ���̐�
	static constexpr int k_FireNum = 19;

	//! ���̃|�W�V����
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

	//! �J�E���g���[�g
	static float k_CountRaito = 0.1f;

	//! �J�����A�j���[�V�����f�[�^
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
	// �I�[�f�B�I��ݒ�
	AudioManager& audioManager = AudioManager::GetInstance();
	// �I�[�f�B�I�f�[�^�ǂݍ���
	audioManager.Load(TitleData::TitleBGM);
	audioManager.Load(TitleData::DecideSE);
	// �Đ�
	audioManager.Play(TitleData::TitleBGM, true);

	// �R���W�����f�o�b�O�r���[���\��
	OBBDebugViewComponent::SetIsUseFlag(false);

	// �t�F�[�h�擾
	m_Fade = SceneManager::GetInstance().GetFade();
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_Fade��nullptr");
		return;
	}
	// �t�F�[�h�J�n
	m_Fade->SetFade(Fade::FADE_OUT, 60);

	// �p�[�e�B�N���}�l�[�W���[�擾
	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �p�[�e�B�N����
	particleManager.SetDebugCountView(false);

	// �p�[�e�B�N���̃e�N�X�`���ǂݍ���
	ID3D11ShaderResourceView* particleSRV = nullptr;
	particleSRV = TextureManager::GetInstance().Load(TitleData::k_ParticleTextureName);
	// null�`�F�b�N
	if (particleSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData particleSRV��nullptr");
		return;
	}
	// �p�[�e�B�N�����쐬
	particleManager.CreateParticle(ParticleManagerData::k_BeamParticleName, ParticleManagerData::k_MaxBeamRifleParticleNum, particleSRV);

	// ���̑��A�N�^�[�}�l�[�W���[�쐬
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if (otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("ModeSelectScene::LoadData otherActorManager��nullptr");
		return;
	}

	// �J�������쐬
	Actor* CameraActor = otherActorManager->CreateActor<Actor>();
	if (CameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData CameraActor��nullptr");
		return;
	}
	// �N�H�[�^�j�I���J�����R���|�[�l���g�쐬
	quaternionCameraComponent = CameraActor->CreateComponent<QuaternionCameraComponent>();
	if (quaternionCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData quaternionCameraComponent��nullptr");
		return;
	}
	// �N�H�[�^�j�I���J�����R���|�[�l���g�Ƀf�[�^����
	quaternionCameraComponent->SetPosition(Conv_XM::Vector3f(0.0f, 3.5f, -4.0f));
	quaternionCameraComponent->SetEuler(20.0f, 0.0f, 0.0f);
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManager��nullptr");
		return;
	}
	// �V�[���}�l�[�W���[�ɃJ�����Z�b�g
	m_SceneRenderManager->SetMainCameraComponent(quaternionCameraComponent);

	// ���C�g�J�������쐬
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActor��nullptr");
		return;
	}
	// ���C�g�J�����R���|�[�l���g�쐬
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActor��nullptr");
		return;
	}
	// ���C�g�J�����Ƀp�����[�^�[�ݒ�
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 10.0f, -5.0f));
	lightCamera->SetEuler(60.0f, 0.0f, 0.0f);
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManager��nullptr");
		return;
	}
	// �V�[�������_�[�Ƀ��C�g�J�����o�^
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);

	//�@�f�B���N�V���i�����C�g�쐬
	Actor* directionalLightActor = otherActorManager->CreateActor<Actor>();
	if (directionalLightActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightActor��nullptr");
		return;
	}
	// �f�B���N�V���i�����C�g�R���|�[�l���g�쐬
	directionalLightComponent = directionalLightActor->CreateComponent<DirectionalLightComponent>();
	if (directionalLightComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightComponent��nullptr");
		return;
	}
	// �f�B���N�V���i�����C�g�̃f�[�^�ݒ�
	directionalLightData = {};
	directionalLightData.Direction = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
	directionalLightData.Color = Conv_XM::Vector3f(2.0f, 2.0f, 2.0f);
	directionalLightData.Ambient = Conv_XM::Vector3f(0.4f, 0.4f, 0.4f);
	directionalLightComponent->SetLight(directionalLightData);
	directionalLightComponent->SetEuler(60.0f, 0.0f, 0.0f);

	// ���b�V���t�B�[���h�̃e�N�X�`���ǂݍ���
	MeshFieldComponent::LoadTexture();
	// ���b�V���t�B�[���h�̍쐬
	Actor* meshfieldActor = otherActorManager->CreateActor<Actor>();
	if (meshfieldActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldActor��nullptr");
		return;
	}
	// ���b�V���t�B�[���h�R���|�[�l���g�쐬
	MeshFieldComponent* meshfieldComponent = meshfieldActor->CreateComponent<MeshFieldComponent>();
	if (meshfieldComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldComponent��nullptr");
		return;
	}
	// ���b�V���t�B�[���h�����}�b�v�ǂݍ���
	meshfieldComponent->LoadHeightMapFile(TitleData::k_MeshFieldHeightMapFileName);
	// ���b�V���t�B�[���h�A���t�@�u�����h�t�@�C���ǂݍ���
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName001, 1);
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName002, 2);
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName003, 3);
	meshfieldComponent->LoadTextureBlendFile(TitleData::k_MeshFieldArufaBlendFileName004, 4);
	// ���b�V���t�B�[���h�}�e���A���ݒ�
	CBufferData::Material material = {};
	material.Metaric = 0.3f;
	material.Smoothness = 0.1f;
	meshfieldComponent->SetMaterial(material);
	meshfieldComponent->SetIsTitle(true);

	// ���f���}�l�[�W���[�擾
	ModelManager& modelManager = ModelManager::GetInstance();
	// �X�J�C�h�[�����쐬
	// �X�J�C�h�[�����f���t�@�C���ǂݍ���
	Model* skyDomeModel = modelManager.Load(TitleData::k_SkyDomeModelName);
	if (skyDomeModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeModel��nullptr");
		return;
	}
	// �X�J�C�h�[���p�A�N�^�[�쐬
	Actor* skyDomeActor = otherActorManager->CreateActor<Actor>();
	if (skyDomeActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeActor��nullptr");
		return;
	}
	// �X�J�C�h�[���R���|�[�l���g�쐬
	SkyDomeomponent* skyDomeComponent = skyDomeActor->CreateComponent<SkyDomeomponent>();
	if (skyDomeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeComponent��nullptr");
		return;
	}
	// �X�J�C�h�[���p�����[�^�[�ݒ�
	skyDomeActor->SetScale(TitleData::k_SkyDomeScale);
	skyDomeComponent->SetModel(skyDomeModel);
	Conv_XM::Vector4f skydomeQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(TitleData::k_SkyDomeEuler.x), TitleData::k_SkyDomeEuler.y, TitleData::k_SkyDomeEuler.z);
	skyDomeActor->SetQuaternion(skydomeQuaternion);
	skyDomeActor->SetPosition(TitleData::k_SkyDomePosition);

	//�A�j���[�V�������f���}�l�[�W���[�擾
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	// �A�j�����f���f�[�^�쐬
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

	// �A�j�����f���f�[�^�쐬
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

	// �r�[���T�[�x�����f���ǂݍ���
	BeamSaber::LoadModel();
	// �r�[���T�[�x���쐬
	beamSaber001 = otherActorManager->CreateActor<BeamSaber>();
	beamSaber001->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	beamSaber001->SetPosition(TitleData::k_BeamSaber001Position[0]);
	beamSaber001->SetEuler(TitleData::k_BeamSaber001Euler[0].x, TitleData::k_BeamSaber001Euler[0].y, TitleData::k_BeamSaber001Euler[0].z);

	beamSaber002 = otherActorManager->CreateActor<BeamSaber>();
	beamSaber002->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	beamSaber002->SetPosition(TitleData::k_BeamSaber002Position[0]);
	beamSaber002->SetColor(Conv_XM::Vector4f(0.3f, 1.0f, 0.3f, 1.0f));
	beamSaber002->SetEuler(TitleData::k_BeamSaber002Euler[0].x, TitleData::k_BeamSaber002Euler[0].y, TitleData::k_BeamSaber002Euler[0].z);

	// �p�[�e�B�N���G�~�b�^�[�쐬
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

	// �G�~�b�^�[��o�^
	m_Emitter001.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter001);

	// �p�[�e�B�N���G�~�b�^�[�쐬
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

	// �G�~�b�^�[��o�^
	m_Emitter002.MyIndex = particleManager.RegisterParticleEmitter(m_Emitter002);

	// �^�C�g��UI�e�N�X�`���ǂݍ���
	TitleSceneUI::LoadTexture();
	// �^�C�g��UI�쐬
	otherActorManager->CreateActor<TitleSceneUI>();

	// �t�@�C�A�쐬
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
	// �I�[�f�B�I�j��
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Unload(TitleData::TitleBGM);
	audioManager.Unload(TitleData::DecideSE);

	// ���b�V���t�B�[���h�j��
	MeshFieldComponent::UnloadTexture();

	// �A�j���[�V�����}�l�[�W���[�擾
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();
	// �A�j���[�V�������f���j��
	animationModelManager.Unload(TitleData::k_AnimationModelFileName);

	// �r�[���T�[�x�����f���j��
	BeamSaber::UnloadModel();

	// �^�C�g��UI�e�N�X�`���j��
	TitleSceneUI::UnloadTexture();

	// ���f���}�l�[�W���[�擾
	ModelManager& modelManager = ModelManager::GetInstance();
	// �X�J�C�h�[���j��
	modelManager.Unload(TitleData::k_SkyDomeModelName);
}

void TitleScene::SetData()
{
}

void TitleScene::UpdateScene()
{
	// �A�j���[�V�����J�E���g
	int animationCount = static_cast<int>(m_Frame * TitleData::k_CountRaito);

	// �A�j���[�V�������[�g
	float animationRaito = static_cast<float>(m_Frame) * TitleData::k_CountRaito;

	// �A�j���[�V�����f�[�^�v�Z
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

	// �A�j���[�V�����f�[�^����
	beamSaber001->SetEuler(euler001.x, euler001.y, euler001.z);
	beamSaber001->SetPosition(pos001);
	beamSaber002->SetEuler(euler002.x, euler002.y, euler002.z);
	beamSaber002->SetPosition(pos002);


	// ���͊Ǘ�
	InputManager& inputManager = InputManager::GetInstance();
	
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fade��nullptr");
		return;
	}

	// ����{�^������������
	if (inputManager.GetIsTrigger(InputManager::IN_DECIDE) == true)
	{
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
		AudioManager::GetInstance().Play(TitleData::DecideSE, false);
		AudioManager::GetInstance().Stop(TitleData::TitleBGM);
	}

	// �t�F�[�h���I������V�[����؂�ւ���
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		SceneManager::GetInstance().SceneChange(SceneManager::SN_MODESELECT_SCENE);
	}

	// �J�E���g�A�b�v
	m_Frame++;
}

void TitleScene::CreateSceneFunction()
{
	// �V�[�������_�[�쐬
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void TitleScene::UnloadSceneFunction()
{
	// �V�[�������_�[�j��
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
