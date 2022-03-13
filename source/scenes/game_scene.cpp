
#include"../system/main.h"
#include "game_scene.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/scene_function/scene_collision_manager.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../framework/actor_manager/mobilesuit_manager.h"
#include"../framework/actor_manager/bullet_manager.h"
#include"../framework/actor_manager/effect_manager.h"
#include"../framework/actor/mobilesuit/player.h"
#include"../framework/actor/mobilesuit/enemy.h"
#include"../framework/actor/mobilesuit/ally.h"
#include"../framework/actor/bullet/bullet.h"
#include"../framework/actor/bullet/enemy_bullet.h"
#include"../framework/actor/bullet/cannon.h"
#include"../framework/actor/ui/arousal_gauge_ui.h"
#include"../framework/actor/ui/boost_gauge_ui.h"
#include"../framework/actor/ui/life_ui.h"
#include"../framework/actor/ui/remaining_bullets_ui.h"
#include"../framework/actor/ui/timelimit_ui.h"
#include"../framework/actor/ui/game_scene/mobilesuit_infomation_ui.h"
#include"../framework/actor/ui/other_ui.h"
#include"../framework/actor/gamemanager/cost_manager.h"
#include"../framework/actor/gamemanager/game_manager.h"
#include"../framework/actor/effect/mobilesuit_explosion_effect.h"
#include"../framework/actor/effect/beamrifle_landing_effect.h"
#include"../framework/actor/stage/sideseven_object.h"
#include"../framework/actor/stage/tree.h"
#include"../framework/actor/weapon/beam_saber.h"
#include"../framework/component/sprite_component/number_sprite_component.h"
#include"../framework/component/mesh_component/mesh_field_component.h"
#include"../framework/component/mesh_component/skydome_component.h"
#include"../framework/component/mesh_component/debug/obb_debug_view_component.h"
#include"../framework/component/mesh_component/debug/aabb_debug_view_component.h"
#include"../framework/component/light_component/directional_light_component.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../directx/resource/texture_manager.h"
#include"../directx/resource/model_manager.h"
#include"../particle/particle_manager.h"
#include"../system/fade.h"
#include"../system/myimgui.h"

#include"../ai/bfs.h"
#include"../ai/gbfs.h"
#include"../framework/actor/ai/weighted_graph_node_loder.h"
#include"../framework/actor/ai/meta_ai/meta_ai.h"
#include"../system/audio_manager.h"

namespace GameSceneData
{
	//! �p�[�e�B�N���̃e�N�X�`����
	static const char* k_ParticleTextureName = "asset/texture/foton.png";

	//! �X�J�C�h�[���̃��f����
	static const char* k_SkyDomeModelName = "asset/model/skydome/skydome.bin";

	//! ���b�V���t�B�[���h�����}�b�v�t�@�C����
	static const char* k_MeshFieldHeightMapFileName = "asset/mapdata/stagedata001.csv";
	//! ���b�V���t�B�[���h�A���t�@�u�����h�t�@�C����
	static const char* k_MeshFieldArufaBlendFileName001 = "asset/mapdata/textureblend001.csv";
	static const char* k_MeshFieldArufaBlendFileName002 = "asset/mapdata/textureblend002.csv";
	static const char* k_MeshFieldArufaBlendFileName003 = "asset/mapdata/textureblend003.csv";
	static const char* k_MeshFieldArufaBlendFileName004 = "asset/mapdata/textureblend004.csv";

	//! �o�H�T���p�m�[�h�t�@�C����
	static const char* k_NodeFileName = "asset/graphdata/stage_graph_node_edge.bin";

	//! �Q�[��BGM
	static const char* GameBGM = "asset/sound/BGM/BGM_Game.wav";
}

GameScene::GameScene()
	: m_IsSceneChange(false)
	, m_Fade(nullptr)
	, m_IsUseDebugCollisionViewFlag(false)
	, m_GameManager(nullptr)
	, m_TimeLimitUI(nullptr)
{
}

GameScene::~GameScene()
{
}

void GameScene::LoadData()
{
	// �t�F�[�h�擾
	m_Fade = SceneManager::GetInstance().GetFade();
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_Fade��nullptr");
		return;
	}
	// �t�F�[�h�A�E�g���J�n
	m_Fade->SetFade(Fade::FADE_OUT, 60);

	// �p�[�e�B�N���}�l�[�W���[�擾
	ParticleManager& particleManager = ParticleManager::GetInstance();
	// �p�[�e�B�N���f�o�b�O�J�E���^�[��\��
	particleManager.SetDebugCountView(false);

	// �I�[�f�B�I���쐬
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Load(GameSceneData::GameBGM);
	// BGM�Đ�
	audioManager.Play(GameSceneData::GameBGM, true);

	// �����e�N�X�`���̓ǂݍ���
	NumberSpriteComponent::LoadNumberTexture();

	// �p�[�e�B�N���̃e�N�X�`���ǂݍ���
	ID3D11ShaderResourceView* particleSRV = nullptr;
	particleSRV = TextureManager::GetInstance().Load(GameSceneData::k_ParticleTextureName);
	if (particleSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData particleSRV��nullptr");
		return;
	}
	// �p�[�e�B�N�����쐬
	ParticleManager::GetInstance().CreateParticle(ParticleManagerData::k_BeamParticleName, ParticleManagerData::k_MaxBeamRifleParticleNum, particleSRV);

	// ���̑��A�N�^�[�}�l�[�W���[�̍쐬
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if (otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData otherActorManager��nullptr");
		return;
	}

	// ���b�V���t�B�[���h�̍쐬
	Actor* meshfieldActor = otherActorManager->CreateActor<Actor>();
	if (meshfieldActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldActor��nullptr");
		return;
	}
	// ���b�V���t�B�[���h�̃e�N�X�`���ǂݍ���
	MeshFieldComponent::LoadTexture();
	// ���b�V���t�B�[���h�R���|�[�l���g�쐬
	MeshFieldComponent* meshfieldComponent = meshfieldActor->CreateComponent<MeshFieldComponent>();
	if (meshfieldComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldComponent��nullptr");
		return;
	}
	// ���b�V���t�B�[���h�}�e���A���ݒ�
	CBufferData::Material material = {};
	material.Metaric = 0.3f;
	material.Smoothness = 0.1f;
	// ���b�V���t�B�[���h�����}�b�v�ǂݍ���
	meshfieldComponent->SetMaterial(material);
	// ���b�V���t�B�[���h�A���t�@�u�����h�t�@�C���ǂݍ���
	meshfieldComponent->LoadHeightMapFile(GameSceneData::k_MeshFieldHeightMapFileName);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName001, 1);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName002, 2);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName003, 3);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName004, 4);

	if (m_SceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldComponent��nullptr");
		return;
	}
	// �V�[���R���W�����}�l�[�W���[�Ƀ��b�V���t�B�[���h��o�^
	m_SceneCollisionManager->SetMeshField(meshfieldComponent);

	//�@�f�B���N�V���i�����C�g�쐬
	Actor* directionalLightActor = otherActorManager->CreateActor<Actor>();
	if (directionalLightActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightActor��nullptr");
		return;
	}
	// �f�B���N�V���i�����C�g�R���|�[�l���g�쐬
	DirectionalLightComponent* directionalLightComponent = directionalLightActor->CreateComponent<DirectionalLightComponent>();
	if (directionalLightComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightComponent��nullptr");
		return;
	}
	// �f�B���N�V���i�����C�g�f�[�^�쐬
	CBufferData::DirectionalLight directionalLightData = {};
	directionalLightData.Direction = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
	directionalLightData.Color = Conv_XM::Vector3f(2.0f, 2.0f, 2.0f);
	directionalLightData.Ambient = Conv_XM::Vector3f(0.4f, 0.4f, 0.4f);
	directionalLightComponent->SetLight(directionalLightData);
	directionalLightComponent->SetEuler(60.0f, 0.0f, 0.0f);

	// ���C�g�J�����쐬
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActor��nullptr");
		return;
	}
	// ���C�g�J�����g�R���|�[�l���g�쐬
	LightCameraComponent* lightCameraComponent = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraComponent��nullptr");
		return;
	}
	// ���C�g�J�����R���|�[�l���g�f�[�^�쐬
	lightCameraComponent->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	Conv_XM::Vector4f lightq = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(60.0f), 0.0f, 0.0f);
	Conv_XM::Vector3f lightpos = DirectX::XMVector3Rotate(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f), lightq);
	lightCameraComponent->SetPosition(lightpos);
	lightCameraComponent->SetEuler(60.0f, 0.0f, 0.0f);

	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManager��nullptr");
		return;
	}
	// �V�[�������_�[�Ƀ��C�g�J������o�^
	m_SceneRenderManager->SetLightCameraComponent(lightCameraComponent);

	// �o�H�T���p�O���t�m�[�h���쐬
	WeightedGraphLoader* weightGraph = otherActorManager->CreateActor<WeightedGraphLoader>();
	// �t�@�C���ǂݍ���
	weightGraph->Load(GameSceneData::k_NodeFileName);
	
	// ���r���X�[�c�}�l�[�W���[���쐬
	MobileSuitManager* mobileSuitManager = CreateActorManager<MobileSuitManager>();
	if (mobileSuitManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData mobileSuitManager��nullptr");
		return;
	}

	// ���f���ǂݍ���
	Player::LoadModel();
	// �T�E���h�ǂݍ���
	MobileSuit::LoadSound();
	Player::LoadSound();
	// �v���C���[�쐬
	Player* player = mobileSuitManager->CreateMobileSuitActor<Player>();
	player->SetPosition(Conv_XM::Vector3f(-30.0f, 15.0f, -30.0f));
	if (player == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData player��nullptr");
		return;
	}

	// ���C�g�J�����R���|�[�l���g�̒Ǐ]�^�[�Q�b�g�Ƀv���C���[��o�^
	lightCameraComponent->SetChaseTarget(player);

	// �����쐬
	// ���f���ǂݍ���
	Ally::LoadModel();
	Ally* ally = mobileSuitManager->CreateMobileSuitActor<Ally>();
	ally->SetPosition(Conv_XM::Vector3f(-35.0f, 15.0f, -35.0f));
	if (ally == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData ally��nullptr");
		return;
	}
	// �o�H�T���p�m�[�h��ݒ�
	ally->SetWeightedGraph(weightGraph->GetWeightGraph());

	// �G�l�~�[�쐬
	// ���f���ǂݍ���
	Enemy::LoadModel();
	// ��l�ڂ̃G�l�~�[���쐬
	Enemy* enemy001 = mobileSuitManager->CreateMobileSuitActor<Enemy>();
	enemy001->SetPosition(Conv_XM::Vector3f(30.0f, 15.0f, 30.0f));
	if (enemy001 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001��nullptr");
		return;
	}
	// �o�H�T���p�m�[�h��ݒ�
	enemy001->SetWeightedGraph(weightGraph->GetWeightGraph());
	// �G�l�~�[�ԍ���ݒ�
	enemy001->SetEnemyNumber(Enemy::ENEMY001);

	// ��l�ڂ̃G�l�~�[���쐬
	Enemy* enemy002 = mobileSuitManager->CreateMobileSuitActor<Enemy>();
	enemy002->SetPosition(Conv_XM::Vector3f(35.0f, 15.0f, 35.0f));
	if (enemy002 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy002��nullptr");
		return;
	}
	// �o�H�T���p�m�[�h��ݒ�
	enemy002->SetWeightedGraph(weightGraph->GetWeightGraph());
	// �G�l�~�[�ԍ���ݒ�
	enemy001->SetEnemyNumber(Enemy::ENEMY002);

	// �v���C���[�ɃG�l�~�[���Z�b�g
	player->SetEnemy(enemy001, enemy002);

	// �o���b�g�}�l�[�W���[���쐬
	BulletManager* bulletManager = CreateActorManager<BulletManager>();
	if (bulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData bulletManager��nullptr");
		return;
	}
	// �o���b�g���쐬
	bulletManager->CreateBulletActors<Bullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// �v���C���[�Ƀo���b�g�}�l�[�W���[��o�^
	player->SetBeamRifleBulletManager(bulletManager);

	// �L���m�����쐬
	BulletManager* cannonManager = CreateActorManager<BulletManager>();
	if (cannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData cannonManager��nullptr");
		return;
	}
	cannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// �v���C���[�Ƀo���b�g�}�l�[�W���[��o�^
	player->SetCannonBulletManager(cannonManager);

	// UI���쐬
	// �e�N�X�`���ǂݍ���
	BoostGaugeUI::LoadTexture();
	// �u�[�X�g�Q�[�WUI���쐬
	BoostGaugeUI* m_BoostUI = otherActorManager->CreateActor<BoostGaugeUI>();
	if (m_BoostUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_BoostUI��nullptr");
		return;
	}
	// �u�[�X�gUI�Ƀv���C���[���Z�b�g
	m_BoostUI->SetMobileSuit(player);

	// �o���Q�[�WUI�쐬
	ArousalGaugeUI* m_ArousalGaugeUI = otherActorManager->CreateActor<ArousalGaugeUI>();
	if (m_ArousalGaugeUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_ArousalGaugeUI��nullptr");
		return;
	}
	// �o��UI�Ƀv���C���[���Z�b�g
	m_ArousalGaugeUI->SetMobileSuit(player);

	// �^�C�����~�b�gUI���쐬
	TimeLimitUI* timeLimitUI = otherActorManager->CreateActor<TimeLimitUI>();

	// ���C�tUI���쐬
	LifeUI* lifeUI = otherActorManager->CreateActor<LifeUI>();
	if (lifeUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lifeUI��nullptr");
		return;
	}
	// ���C�tUI�Ƀv���C���[���Z�b�g
	lifeUI->SetMobileSuit(player);

	// �o���b�g�c�e���\��UI���쐬
	RemaingBulletsUI* remainigBulletsUI = otherActorManager->CreateActor<RemaingBulletsUI>();
	if (remainigBulletsUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData remainigBulletsUI��nullptr");
		return;
	}
	// �c�e��UI�Ƀo���b�g�}�l�[�W���[���Z�b�g
	remainigBulletsUI->SetBulletManager(bulletManager);

	// �o���b�g�c�e���\��UI���쐬
	RemaingBulletsUI* remainigCannonUI = otherActorManager->CreateActor<RemaingBulletsUI>();
	if (remainigCannonUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData remainigBulletsUI��nullptr");
		return;
	}
	// �c�e��UI�ɃL���m���}�l�[�W���[���Z�b�g
	remainigCannonUI->SetBulletManager(cannonManager);
	remainigCannonUI->SetLeftPosition(Conv_XM::Vector2f(850.0f, 400.0f));

	// �X�J�C�h�[�����쐬
	ModelManager& modelManager = ModelManager::GetInstance();
	Model* skyDomeModel = modelManager.Load(GameSceneData::k_SkyDomeModelName);
	if (skyDomeModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeModel��nullptr");
		return;
	}
	// �X�J�C�h�[���\���p�A�N�^�[���쐬
	Actor* skyDomeActor = otherActorManager->CreateActor<Actor>();
	if (skyDomeActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeActor��nullptr");
		return;
	}
	// �X�J�C�h�[���\���p�R���|�[�l���g���쐬
	SkyDomeomponent* skyDomeComponent = skyDomeActor->CreateComponent<SkyDomeomponent>();
	if (skyDomeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeComponent��nullptr");
		return;
	}
	// �X�J�C�h�[���p�����[�^�[�ݒ�
	skyDomeActor->SetScale(Conv_XM::Vector3f(150.0f, 150.0f, 150.0f));
	skyDomeComponent->SetModel(skyDomeModel);
	Conv_XM::Vector4f skydomeQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);
	skyDomeActor->SetQuaternion(skydomeQuaternion);
	skyDomeActor->SetPosition(Conv_XM::Vector3f(0.0f, -10.0f, 0.0f));

	// ���r���X�[�c���UI���쐬
	MobileSuitInfomationUI::LoadTexture();
	MobileSuitInfomationUI* enemy001InfoUI = otherActorManager->CreateActor<MobileSuitInfomationUI>();
	enemy001InfoUI->SetParentActor(enemy001);
	enemy001InfoUI->SetMobileSuitName(MobileSuitInfomationUI::ENEMY001);

	MobileSuitInfomationUI* enemy002InfoUI = otherActorManager->CreateActor<MobileSuitInfomationUI>();
	enemy002InfoUI->SetParentActor(enemy002);
	enemy001InfoUI->SetMobileSuitName(MobileSuitInfomationUI::ENEMY002);

	MobileSuitInfomationUI* allyInfoUI = otherActorManager->CreateActor<MobileSuitInfomationUI>();
	allyInfoUI->SetParentActor(ally);
	allyInfoUI->SetMobileSuitName(MobileSuitInfomationUI::ALLY);

	// ���r���X�[�c�������������̃G�t�F�N�g�}�l�[�W���[���쐬
	EffectManager* mobileSuitExplosionEffectManager = CreateActorManager<EffectManager>();
	mobileSuitExplosionEffectManager->CreateEffectActors<MobileSuitExplosionEffect>(10);

	// ���r���X�[�c�ɔ����G�t�F�N�g�}�l�[�W���[��ݒ�
	player->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);
	ally->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);
	enemy001->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);
	enemy002->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);

	// �r�[�����C�t�������e�������̃G�t�F�N�g�}�l�[�W���[���쐬
	EffectManager* beamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	beamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);

	// �o���b�g�}�l�[�W���[�ɃG�t�F�N�g�}�l�[�W���[��ݒ�
	bulletManager->SetLandingEffectManager(beamriflleLandingEffectManager);

	// �T�C�h�Z�u���̃��f���ǂݍ���
	SideSevenObject::LoadModel();
	otherActorManager->CreateActor<SideSevenObject>();


	// �G�l�~�[001�p�o���b�g�}�l�[�W���[���쐬
	BulletManager* enemy001BulletManager = CreateActorManager<BulletManager>();
	if (enemy001BulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManager��nullptr");
		return;
	}
	// �G�l�~�[001�p�o���b�g���쐬
	enemy001BulletManager->CreateBulletActors<EnemyBullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// �G�l�~�[001�Ƀo���b�g�}�l�[�W���[�ݒ�
	enemy001->SetBeamRifleBulletManager(enemy001BulletManager);

	// �G�l�~�[001�p�o���b�g�̒��e�p�G�t�F�N�g�}�l�[�W���[���쐬
	EffectManager* enemy001BBeamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	enemy001BBeamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);
	enemy001BulletManager->SetLandingEffectManager(enemy001BBeamriflleLandingEffectManager);

	// �G�l�~�[001�p�L���m���̃o���b�g�}�l�[�W���[�쐬
	BulletManager* enemy001CannonManager = CreateActorManager<BulletManager>();
	if (enemy001CannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManager��nullptr");
		return;
	}
	// �L���m���쐬
	enemy001CannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// �G�l�~�[001�ɃL���m���̃o���b�g�}�l�[�W���[�ݒ�
	enemy001->SetCannonBulletManager(enemy001CannonManager);

	// �G�l�~�[002�p�̃o���b�g�}�l�[�W���[���쐬
	BulletManager* enemy002BulletManager = CreateActorManager<BulletManager>();
	if (enemy002BulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy002BulletManager��nullptr");
		return;
	}
	// �o���b�g���쐬
	enemy002BulletManager->CreateBulletActors<EnemyBullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// �G�l�~�[002�p�̃o���b�g�}�l�[�W���[��ݒ�
	enemy002->SetBeamRifleBulletManager(enemy002BulletManager);

	// �G�l�~�[002�p�o���b�g�̒��e�G�t�F�N�g�}�l�[�W���[���쐬
	EffectManager* enemy002BeamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	// �G�t�F�N�g���쐬
	enemy002BeamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);
	// �G�l�~�[002�p�o���b�g�ɒ��e�G�t�F�N�g�}�l�[�W���[��ݒ�
	enemy002BulletManager->SetLandingEffectManager(enemy002BeamriflleLandingEffectManager);

	// �G�l�~�[002�p�L���m���̃o���b�g�}�l�[�W���[���쐬
	BulletManager* enemy002CannonManager = CreateActorManager<BulletManager>();
	if (enemy002CannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManager��nullptr");
		return;
	}
	// �G�l�~�[002�p�L���m�����쐬
	enemy002CannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// �G�l�~�[002�ɃL���m���̃o���b�g�}�l�[�W���[��ݒ�
	enemy002->SetCannonBulletManager(enemy002CannonManager);

	// �G�l�~�[�̓G��ݒ�
	enemy001->SetEnemy(player, ally);
	enemy002->SetEnemy(player, ally);

	// �����p�o���b�g�}�l�[�W���[���쐬
	BulletManager* allyBulletManager = CreateActorManager<BulletManager>();
	if (allyBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy002BulletManager��nullptr");
		return;
	}
	// �����p�o���b�g���쐬
	allyBulletManager->CreateBulletActors<Bullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// �����Ƀo���b�g�}�l�[�W���[��ݒ�
	ally->SetBeamRifleBulletManager(allyBulletManager);

	// �����p�o���b�g�̒��e���̃G�t�F�N�g�}�l�[�W���[���쐬
	EffectManager* allyBeamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	// �����p�o���b�g�̒��e���̃G�t�F�N�g���쐬
	allyBeamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);
	// �����p�o���b�g�ɒ��e���̃G�t�F�N�g��ݒ�
	allyBulletManager->SetLandingEffectManager(allyBeamriflleLandingEffectManager);

	// �����p�L���m���̃o���b�g�}�l�[�W���[���쐬
	BulletManager* allyCannonManager = CreateActorManager<BulletManager>();
	if (allyCannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManager��nullptr");
		return;
	}
	// �����p�L���m�����쐬
	allyCannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// �����p�L���m���̃o���b�g�}�l�[�W���[��ݒ�
	ally->SetCannonBulletManager(allyCannonManager);

	// �����̓G��ݒ�
	ally->SetEnemy(enemy001, enemy002);

	// �r�[���T�[�x���쐬
	BeamSaber::LoadModel();
	BeamSaber* playerBeamSaber = otherActorManager->CreateActor<BeamSaber>();
	BeamSaber* allyBeamSaber = otherActorManager->CreateActor<BeamSaber>();
	BeamSaber* enemy001BeamSaber = otherActorManager->CreateActor<BeamSaber>();
	BeamSaber* enemy002BeamSaber = otherActorManager->CreateActor<BeamSaber>();

	// �r�[���T�[�x���̎g�p����p�[�e�B�N����ݒ�
	playerBeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	allyBeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	enemy001BeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	enemy002BeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);

	// �r�[���T�[�x���̃p�[�e�B�N�����Ԃ�ݒ�
	playerBeamSaber->SetParticleTime(2.0f, 2.0f);
	allyBeamSaber->SetParticleTime(2.0f, 2.0f);
	enemy001BeamSaber->SetParticleTime(2.0f, 2.0f);
	enemy002BeamSaber->SetParticleTime(2.0f, 2.0f);

	// ���r���X�[�c�Ƀr�[���T�[�x����ݒ�
	player->SetBeamSaber(playerBeamSaber);
	ally->SetBeamSaber(allyBeamSaber);
	enemy001->SetBeamSaber(enemy001BeamSaber);
	enemy002->SetBeamSaber(enemy002BeamSaber);

	// �r�[���T�[�x���̏��L�҂�ݒ�
	playerBeamSaber->SetOwnerMobileSuit(player);
	allyBeamSaber->SetOwnerMobileSuit(ally);
	enemy001BeamSaber->SetOwnerMobileSuit(enemy001);
	enemy002BeamSaber->SetOwnerMobileSuit(enemy002);

	// ���̑�UI�̃e�N�X�`���ǂݍ���
	OtherUI::LoadTexture();
	// ���̑�UI���쐬
	otherActorManager->CreateActor<OtherUI>();

	// �R�X�g�}�l�[�W���[�̃e�N�X�`���ǂݍ���
	CostManager::LoadTexture();
	// �R�X�g�}�l�[�W���[���쐬
	CostManager* costManager = otherActorManager->CreateActor<CostManager>();

	// �e�탂�r���X�[�c�ɃR�X�g�}�l�[�W���[��ݒ�
	player->SetCostManager(costManager);
	ally->SetCostManager(costManager);
	enemy001->SetCostManager(costManager);
	enemy002->SetCostManager(costManager);

	// �Q�[���}�l�[�W���[�̃e�N�X�`���ǂݍ���
	GameManager::LoadTexture();
	// �Q�[���}�l�[�W���[���쐬
	m_GameManager = otherActorManager->CreateActor<GameManager>();
	// �Q�[���}�l�[�W���[�̃p�����[�^�[�ݒ�
	m_GameManager->SetParameter(timeLimitUI, costManager, player, ally, enemy001, enemy002);

	// ���^AI���쐬
	MetaAI* metaAI = otherActorManager->CreateActor<MetaAI>();
	// ���^AI�Ƀ��r���X�[�c��ݒ�
	metaAI->SetMobileSuit(player, ally, enemy001, enemy002);


	// �e�N�X�`���ǂݍ���
	Tree::LoadTexture();
	otherActorManager->CreateActor<Tree>();
}

void GameScene::UnloadData()
{
	// �e�N�X�`���j��
	Tree::UnloadTexture();

	// �e�N�X�`���j��
	GameManager::UnloadTexture();

	// �e�N�X�`���j��
	CostManager::UnloadTexture();

	// �e�N�X�`���j��
	OtherUI::UnloadTexture();

	// ���f���j��
	BeamSaber::UnloadModel();

	// ���f���j��
	SideSevenObject::UnloadModel();

	// �e�N�X�`���j��
	NumberSpriteComponent::UnloadNumberTexture();

	// �e�N�X�`���j��
	MobileSuitInfomationUI::UnloadTexture();

	// �e�N�X�`���j��
	BoostGaugeUI::LoadTexture();

	// ���f���j��
	Enemy::UnloadModel();

	// ���f���j��
	Ally::UnloadModel();

	// �T�E���h�j��
	Player::UnloadSound();

	// ���f���j��
	Player::UnloadModel();

	// �T�E���h�j��
	MobileSuit::UnloadSound();

	// �e�N�X�`���j��
	MeshFieldComponent::UnloadTexture();
}

void GameScene::SetData()
{

}

void GameScene::UpdateScene()
{
	ImGui::Begin(MyImGuiData::k_DebugWindowName);
	ImGui::Text(u8"�L�[�{�[�h�̏ꍇ");
	ImGui::Text(u8"WASD:�ړ� Space�L�[:�W�����v R�L�[���C���ˌ�");
	ImGui::Text(u8"Enter�L�[ : ���b�N�؂�ւ�");

	ImGui::Text(u8"�R���g���[���[�̏ꍇ");
	ImGui::Text(u8"�\���p�b�h:�ړ� A�{�^��:�W�����v X�{�^�����C���ˌ�");
	ImGui::Text(u8"B�{�^�� : ���b�N�؂�ւ�");

	ImGui::Text(u8"�W�����v�{�^���f����2�A��:�u�[�X�g�_�b�V��");


	ImGui::Checkbox("DebugCollisionView", &m_IsUseDebugCollisionViewFlag);
	ImGui::End();

	// �f�o�b�O�R���W�����r���[��\�����邩�ǂ���
	OBBDebugViewComponent::SetIsUseFlag(m_IsUseDebugCollisionViewFlag);
	AABBDebugViewComponent::SetIsUseFlag(m_IsUseDebugCollisionViewFlag);

	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fade��nullptr");
		return;
	}

	// �t�F�[�h���I������V�[����؂�ւ���
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		AudioManager& audioManager = AudioManager::GetInstance();
		audioManager.Stop(GameSceneData::GameBGM);

		switch (m_GameManager->GetSceneChange())
		{
		case GameManager::SCENE_CHANGE_NONE:
			break;
		case GameManager::SCENE_CHANGE_WIN:
			SceneManager::GetInstance().SceneChange(SceneManager::SN_GAME_CLEAR_SCENE);
			break;
		case GameManager::SCENE_CHANGE_LOSE:
			SceneManager::GetInstance().SceneChange(SceneManager::SN_GAME_OVER_SCENE);
			break;
		default:
			break;
		}
	}

	if (m_IsSceneChange == true)
	{
		return;
	}

	switch (m_GameManager->GetSceneChange())
	{
	case GameManager::SCENE_CHANGE_NONE:
		break;
	case GameManager::SCENE_CHANGE_WIN:
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
		break;
	case GameManager::SCENE_CHANGE_LOSE:
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
		break;
	default:
		break;
	}
}

void GameScene::CreateSceneFunction()
{
	// �V�[�������_�[�쐬
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
	// �R���W�����}�l�[�W���[�쐬
	if (m_SceneCollisionManager == nullptr)
	{
		m_SceneCollisionManager = new SceneCollisionManager();
		m_SceneCollisionManager->Init();
	}
}

void GameScene::UnloadSceneFunction()
{
	// �V�[���R���W�����}�l�[�W���[�j��
	if (m_SceneCollisionManager)
	{
		m_SceneCollisionManager->Uninit();
		delete m_SceneCollisionManager;
		m_SceneCollisionManager = nullptr;
	}
	// �V�[�������_�[�j��
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
