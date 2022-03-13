
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
	//! パーティクルのテクスチャ名
	static const char* k_ParticleTextureName = "asset/texture/foton.png";

	//! スカイドームのモデル名
	static const char* k_SkyDomeModelName = "asset/model/skydome/skydome.bin";

	//! メッシュフィールド高さマップファイル名
	static const char* k_MeshFieldHeightMapFileName = "asset/mapdata/stagedata001.csv";
	//! メッシュフィールドアルファブレンドファイル名
	static const char* k_MeshFieldArufaBlendFileName001 = "asset/mapdata/textureblend001.csv";
	static const char* k_MeshFieldArufaBlendFileName002 = "asset/mapdata/textureblend002.csv";
	static const char* k_MeshFieldArufaBlendFileName003 = "asset/mapdata/textureblend003.csv";
	static const char* k_MeshFieldArufaBlendFileName004 = "asset/mapdata/textureblend004.csv";

	//! 経路探索用ノードファイル名
	static const char* k_NodeFileName = "asset/graphdata/stage_graph_node_edge.bin";

	//! ゲームBGM
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
	// フェード取得
	m_Fade = SceneManager::GetInstance().GetFade();
	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_Fadeがnullptr");
		return;
	}
	// フェードアウトを開始
	m_Fade->SetFade(Fade::FADE_OUT, 60);

	// パーティクルマネージャー取得
	ParticleManager& particleManager = ParticleManager::GetInstance();
	// パーティクルデバッグカウンター非表示
	particleManager.SetDebugCountView(false);

	// オーディオを作成
	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Load(GameSceneData::GameBGM);
	// BGM再生
	audioManager.Play(GameSceneData::GameBGM, true);

	// 数字テクスチャの読み込み
	NumberSpriteComponent::LoadNumberTexture();

	// パーティクルのテクスチャ読み込み
	ID3D11ShaderResourceView* particleSRV = nullptr;
	particleSRV = TextureManager::GetInstance().Load(GameSceneData::k_ParticleTextureName);
	if (particleSRV == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData particleSRVがnullptr");
		return;
	}
	// パーティクルを作成
	ParticleManager::GetInstance().CreateParticle(ParticleManagerData::k_BeamParticleName, ParticleManagerData::k_MaxBeamRifleParticleNum, particleSRV);

	// その他アクターマネージャーの作成
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if (otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData otherActorManagerがnullptr");
		return;
	}

	// メッシュフィールドの作成
	Actor* meshfieldActor = otherActorManager->CreateActor<Actor>();
	if (meshfieldActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldActorがnullptr");
		return;
	}
	// メッシュフィールドのテクスチャ読み込み
	MeshFieldComponent::LoadTexture();
	// メッシュフィールドコンポーネント作成
	MeshFieldComponent* meshfieldComponent = meshfieldActor->CreateComponent<MeshFieldComponent>();
	if (meshfieldComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldComponentがnullptr");
		return;
	}
	// メッシュフィールドマテリアル設定
	CBufferData::Material material = {};
	material.Metaric = 0.3f;
	material.Smoothness = 0.1f;
	// メッシュフィールド高さマップ読み込み
	meshfieldComponent->SetMaterial(material);
	// メッシュフィールドアルファブレンドファイル読み込み
	meshfieldComponent->LoadHeightMapFile(GameSceneData::k_MeshFieldHeightMapFileName);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName001, 1);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName002, 2);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName003, 3);
	meshfieldComponent->LoadTextureBlendFile(GameSceneData::k_MeshFieldArufaBlendFileName004, 4);

	if (m_SceneCollisionManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldComponentがnullptr");
		return;
	}
	// シーンコリジョンマネージャーにメッシュフィールドを登録
	m_SceneCollisionManager->SetMeshField(meshfieldComponent);

	//　ディレクショナルライト作成
	Actor* directionalLightActor = otherActorManager->CreateActor<Actor>();
	if (directionalLightActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightActorがnullptr");
		return;
	}
	// ディレクショナルライトコンポーネント作成
	DirectionalLightComponent* directionalLightComponent = directionalLightActor->CreateComponent<DirectionalLightComponent>();
	if (directionalLightComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightComponentがnullptr");
		return;
	}
	// ディレクショナルライトデータ作成
	CBufferData::DirectionalLight directionalLightData = {};
	directionalLightData.Direction = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
	directionalLightData.Color = Conv_XM::Vector3f(2.0f, 2.0f, 2.0f);
	directionalLightData.Ambient = Conv_XM::Vector3f(0.4f, 0.4f, 0.4f);
	directionalLightComponent->SetLight(directionalLightData);
	directionalLightComponent->SetEuler(60.0f, 0.0f, 0.0f);

	// ライトカメラ作成
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraActorがnullptr");
		return;
	}
	// ライトカメラトコンポーネント作成
	LightCameraComponent* lightCameraComponent = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lightCameraComponentがnullptr");
		return;
	}
	// ライトカメラコンポーネントデータ作成
	lightCameraComponent->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	Conv_XM::Vector4f lightq = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(60.0f), 0.0f, 0.0f);
	Conv_XM::Vector3f lightpos = DirectX::XMVector3Rotate(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f), lightq);
	lightCameraComponent->SetPosition(lightpos);
	lightCameraComponent->SetEuler(60.0f, 0.0f, 0.0f);

	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_SceneRenderManagerがnullptr");
		return;
	}
	// シーンレンダーにライトカメラを登録
	m_SceneRenderManager->SetLightCameraComponent(lightCameraComponent);

	// 経路探索用グラフノードを作成
	WeightedGraphLoader* weightGraph = otherActorManager->CreateActor<WeightedGraphLoader>();
	// ファイル読み込み
	weightGraph->Load(GameSceneData::k_NodeFileName);
	
	// モビルスーツマネージャーを作成
	MobileSuitManager* mobileSuitManager = CreateActorManager<MobileSuitManager>();
	if (mobileSuitManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData mobileSuitManagerがnullptr");
		return;
	}

	// モデル読み込み
	Player::LoadModel();
	// サウンド読み込み
	MobileSuit::LoadSound();
	Player::LoadSound();
	// プレイヤー作成
	Player* player = mobileSuitManager->CreateMobileSuitActor<Player>();
	player->SetPosition(Conv_XM::Vector3f(-30.0f, 15.0f, -30.0f));
	if (player == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData playerがnullptr");
		return;
	}

	// ライトカメラコンポーネントの追従ターゲットにプレイヤーを登録
	lightCameraComponent->SetChaseTarget(player);

	// 味方作成
	// モデル読み込み
	Ally::LoadModel();
	Ally* ally = mobileSuitManager->CreateMobileSuitActor<Ally>();
	ally->SetPosition(Conv_XM::Vector3f(-35.0f, 15.0f, -35.0f));
	if (ally == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData allyがnullptr");
		return;
	}
	// 経路探索用ノードを設定
	ally->SetWeightedGraph(weightGraph->GetWeightGraph());

	// エネミー作成
	// モデル読み込み
	Enemy::LoadModel();
	// 一人目のエネミーを作成
	Enemy* enemy001 = mobileSuitManager->CreateMobileSuitActor<Enemy>();
	enemy001->SetPosition(Conv_XM::Vector3f(30.0f, 15.0f, 30.0f));
	if (enemy001 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001がnullptr");
		return;
	}
	// 経路探索用ノードを設定
	enemy001->SetWeightedGraph(weightGraph->GetWeightGraph());
	// エネミー番号を設定
	enemy001->SetEnemyNumber(Enemy::ENEMY001);

	// 二人目のエネミーを作成
	Enemy* enemy002 = mobileSuitManager->CreateMobileSuitActor<Enemy>();
	enemy002->SetPosition(Conv_XM::Vector3f(35.0f, 15.0f, 35.0f));
	if (enemy002 == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy002がnullptr");
		return;
	}
	// 経路探索用ノードを設定
	enemy002->SetWeightedGraph(weightGraph->GetWeightGraph());
	// エネミー番号を設定
	enemy001->SetEnemyNumber(Enemy::ENEMY002);

	// プレイヤーにエネミーをセット
	player->SetEnemy(enemy001, enemy002);

	// バレットマネージャーを作成
	BulletManager* bulletManager = CreateActorManager<BulletManager>();
	if (bulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData bulletManagerがnullptr");
		return;
	}
	// バレットを作成
	bulletManager->CreateBulletActors<Bullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// プレイヤーにバレットマネージャーを登録
	player->SetBeamRifleBulletManager(bulletManager);

	// キャノンを作成
	BulletManager* cannonManager = CreateActorManager<BulletManager>();
	if (cannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData cannonManagerがnullptr");
		return;
	}
	cannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// プレイヤーにバレットマネージャーを登録
	player->SetCannonBulletManager(cannonManager);

	// UIを作成
	// テクスチャ読み込み
	BoostGaugeUI::LoadTexture();
	// ブーストゲージUIを作成
	BoostGaugeUI* m_BoostUI = otherActorManager->CreateActor<BoostGaugeUI>();
	if (m_BoostUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_BoostUIがnullptr");
		return;
	}
	// ブーストUIにプレイヤーをセット
	m_BoostUI->SetMobileSuit(player);

	// 覚醒ゲージUI作成
	ArousalGaugeUI* m_ArousalGaugeUI = otherActorManager->CreateActor<ArousalGaugeUI>();
	if (m_ArousalGaugeUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData m_ArousalGaugeUIがnullptr");
		return;
	}
	// 覚醒UIにプレイヤーをセット
	m_ArousalGaugeUI->SetMobileSuit(player);

	// タイムリミットUIを作成
	TimeLimitUI* timeLimitUI = otherActorManager->CreateActor<TimeLimitUI>();

	// ライフUIを作成
	LifeUI* lifeUI = otherActorManager->CreateActor<LifeUI>();
	if (lifeUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData lifeUIがnullptr");
		return;
	}
	// ライフUIにプレイヤーをセット
	lifeUI->SetMobileSuit(player);

	// バレット残弾数表示UIを作成
	RemaingBulletsUI* remainigBulletsUI = otherActorManager->CreateActor<RemaingBulletsUI>();
	if (remainigBulletsUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData remainigBulletsUIがnullptr");
		return;
	}
	// 残弾数UIにバレットマネージャーをセット
	remainigBulletsUI->SetBulletManager(bulletManager);

	// バレット残弾数表示UIを作成
	RemaingBulletsUI* remainigCannonUI = otherActorManager->CreateActor<RemaingBulletsUI>();
	if (remainigCannonUI == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData remainigBulletsUIがnullptr");
		return;
	}
	// 残弾数UIにキャノンマネージャーをセット
	remainigCannonUI->SetBulletManager(cannonManager);
	remainigCannonUI->SetLeftPosition(Conv_XM::Vector2f(850.0f, 400.0f));

	// スカイドームを作成
	ModelManager& modelManager = ModelManager::GetInstance();
	Model* skyDomeModel = modelManager.Load(GameSceneData::k_SkyDomeModelName);
	if (skyDomeModel == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeModelがnullptr");
		return;
	}
	// スカイドーム表示用アクターを作成
	Actor* skyDomeActor = otherActorManager->CreateActor<Actor>();
	if (skyDomeActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeActorがnullptr");
		return;
	}
	// スカイドーム表示用コンポーネントを作成
	SkyDomeomponent* skyDomeComponent = skyDomeActor->CreateComponent<SkyDomeomponent>();
	if (skyDomeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData skyDomeComponentがnullptr");
		return;
	}
	// スカイドームパラメーター設定
	skyDomeActor->SetScale(Conv_XM::Vector3f(150.0f, 150.0f, 150.0f));
	skyDomeComponent->SetModel(skyDomeModel);
	Conv_XM::Vector4f skydomeQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);
	skyDomeActor->SetQuaternion(skydomeQuaternion);
	skyDomeActor->SetPosition(Conv_XM::Vector3f(0.0f, -10.0f, 0.0f));

	// モビルスーツ情報UIを作成
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

	// モビルスーツが爆発した時のエフェクトマネージャーを作成
	EffectManager* mobileSuitExplosionEffectManager = CreateActorManager<EffectManager>();
	mobileSuitExplosionEffectManager->CreateEffectActors<MobileSuitExplosionEffect>(10);

	// モビルスーツに爆発エフェクトマネージャーを設定
	player->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);
	ally->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);
	enemy001->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);
	enemy002->SetMobileSuitExplosionEffectManager(mobileSuitExplosionEffectManager);

	// ビームライフルが着弾した時のエフェクトマネージャーを作成
	EffectManager* beamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	beamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);

	// バレットマネージャーにエフェクトマネージャーを設定
	bulletManager->SetLandingEffectManager(beamriflleLandingEffectManager);

	// サイドセブンのモデル読み込み
	SideSevenObject::LoadModel();
	otherActorManager->CreateActor<SideSevenObject>();


	// エネミー001用バレットマネージャーを作成
	BulletManager* enemy001BulletManager = CreateActorManager<BulletManager>();
	if (enemy001BulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManagerがnullptr");
		return;
	}
	// エネミー001用バレットを作成
	enemy001BulletManager->CreateBulletActors<EnemyBullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// エネミー001にバレットマネージャー設定
	enemy001->SetBeamRifleBulletManager(enemy001BulletManager);

	// エネミー001用バレットの着弾用エフェクトマネージャーを作成
	EffectManager* enemy001BBeamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	enemy001BBeamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);
	enemy001BulletManager->SetLandingEffectManager(enemy001BBeamriflleLandingEffectManager);

	// エネミー001用キャノンのバレットマネージャー作成
	BulletManager* enemy001CannonManager = CreateActorManager<BulletManager>();
	if (enemy001CannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManagerがnullptr");
		return;
	}
	// キャノン作成
	enemy001CannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// エネミー001にキャノンのバレットマネージャー設定
	enemy001->SetCannonBulletManager(enemy001CannonManager);

	// エネミー002用のバレットマネージャーを作成
	BulletManager* enemy002BulletManager = CreateActorManager<BulletManager>();
	if (enemy002BulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy002BulletManagerがnullptr");
		return;
	}
	// バレットを作成
	enemy002BulletManager->CreateBulletActors<EnemyBullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// エネミー002用のバレットマネージャーを設定
	enemy002->SetBeamRifleBulletManager(enemy002BulletManager);

	// エネミー002用バレットの着弾エフェクトマネージャーを作成
	EffectManager* enemy002BeamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	// エフェクトを作成
	enemy002BeamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);
	// エネミー002用バレットに着弾エフェクトマネージャーを設定
	enemy002BulletManager->SetLandingEffectManager(enemy002BeamriflleLandingEffectManager);

	// エネミー002用キャノンのバレットマネージャーを作成
	BulletManager* enemy002CannonManager = CreateActorManager<BulletManager>();
	if (enemy002CannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManagerがnullptr");
		return;
	}
	// エネミー002用キャノンを作成
	enemy002CannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// エネミー002にキャノンのバレットマネージャーを設定
	enemy002->SetCannonBulletManager(enemy002CannonManager);

	// エネミーの敵を設定
	enemy001->SetEnemy(player, ally);
	enemy002->SetEnemy(player, ally);

	// 味方用バレットマネージャーを作成
	BulletManager* allyBulletManager = CreateActorManager<BulletManager>();
	if (allyBulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy002BulletManagerがnullptr");
		return;
	}
	// 味方用バレットを作成
	allyBulletManager->CreateBulletActors<Bullet>(PlayerData::k_MaxBeamRifleInstanceNum, PlayerData::k_MaxBeamRifleNum);
	// 味方にバレットマネージャーを設定
	ally->SetBeamRifleBulletManager(allyBulletManager);

	// 味方用バレットの着弾時のエフェクトマネージャーを作成
	EffectManager* allyBeamriflleLandingEffectManager = CreateActorManager<EffectManager>();
	// 味方用バレットの着弾時のエフェクトを作成
	allyBeamriflleLandingEffectManager->CreateEffectActors<BeanRifleLandingEffect>(20);
	// 味方用バレットに着弾時のエフェクトを設定
	allyBulletManager->SetLandingEffectManager(allyBeamriflleLandingEffectManager);

	// 味方用キャノンのバレットマネージャーを作成
	BulletManager* allyCannonManager = CreateActorManager<BulletManager>();
	if (allyCannonManager == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData enemy001BulletManagerがnullptr");
		return;
	}
	// 味方用キャノンを作成
	allyCannonManager->CreateBulletActors<Cannon>(PlayerData::k_MaxCannonInstanceNum, PlayerData::k_MaxCannonNum);
	// 味方用キャノンのバレットマネージャーを設定
	ally->SetCannonBulletManager(allyCannonManager);

	// 味方の敵を設定
	ally->SetEnemy(enemy001, enemy002);

	// ビームサーベル作成
	BeamSaber::LoadModel();
	BeamSaber* playerBeamSaber = otherActorManager->CreateActor<BeamSaber>();
	BeamSaber* allyBeamSaber = otherActorManager->CreateActor<BeamSaber>();
	BeamSaber* enemy001BeamSaber = otherActorManager->CreateActor<BeamSaber>();
	BeamSaber* enemy002BeamSaber = otherActorManager->CreateActor<BeamSaber>();

	// ビームサーベルの使用するパーティクルを設定
	playerBeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	allyBeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	enemy001BeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);
	enemy002BeamSaber->SetUseParticleName(ParticleManagerData::k_BeamParticleName);

	// ビームサーベルのパーティクル時間を設定
	playerBeamSaber->SetParticleTime(2.0f, 2.0f);
	allyBeamSaber->SetParticleTime(2.0f, 2.0f);
	enemy001BeamSaber->SetParticleTime(2.0f, 2.0f);
	enemy002BeamSaber->SetParticleTime(2.0f, 2.0f);

	// モビルスーツにビームサーベルを設定
	player->SetBeamSaber(playerBeamSaber);
	ally->SetBeamSaber(allyBeamSaber);
	enemy001->SetBeamSaber(enemy001BeamSaber);
	enemy002->SetBeamSaber(enemy002BeamSaber);

	// ビームサーベルの所有者を設定
	playerBeamSaber->SetOwnerMobileSuit(player);
	allyBeamSaber->SetOwnerMobileSuit(ally);
	enemy001BeamSaber->SetOwnerMobileSuit(enemy001);
	enemy002BeamSaber->SetOwnerMobileSuit(enemy002);

	// その他UIのテクスチャ読み込み
	OtherUI::LoadTexture();
	// その他UIを作成
	otherActorManager->CreateActor<OtherUI>();

	// コストマネージャーのテクスチャ読み込み
	CostManager::LoadTexture();
	// コストマネージャーを作成
	CostManager* costManager = otherActorManager->CreateActor<CostManager>();

	// 各種モビルスーツにコストマネージャーを設定
	player->SetCostManager(costManager);
	ally->SetCostManager(costManager);
	enemy001->SetCostManager(costManager);
	enemy002->SetCostManager(costManager);

	// ゲームマネージャーのテクスチャ読み込み
	GameManager::LoadTexture();
	// ゲームマネージャーを作成
	m_GameManager = otherActorManager->CreateActor<GameManager>();
	// ゲームマネージャーのパラメーター設定
	m_GameManager->SetParameter(timeLimitUI, costManager, player, ally, enemy001, enemy002);

	// メタAIを作成
	MetaAI* metaAI = otherActorManager->CreateActor<MetaAI>();
	// メタAIにモビルスーツを設定
	metaAI->SetMobileSuit(player, ally, enemy001, enemy002);


	// テクスチャ読み込み
	Tree::LoadTexture();
	otherActorManager->CreateActor<Tree>();
}

void GameScene::UnloadData()
{
	// テクスチャ破棄
	Tree::UnloadTexture();

	// テクスチャ破棄
	GameManager::UnloadTexture();

	// テクスチャ破棄
	CostManager::UnloadTexture();

	// テクスチャ破棄
	OtherUI::UnloadTexture();

	// モデル破棄
	BeamSaber::UnloadModel();

	// モデル破棄
	SideSevenObject::UnloadModel();

	// テクスチャ破棄
	NumberSpriteComponent::UnloadNumberTexture();

	// テクスチャ破棄
	MobileSuitInfomationUI::UnloadTexture();

	// テクスチャ破棄
	BoostGaugeUI::LoadTexture();

	// モデル破棄
	Enemy::UnloadModel();

	// モデル破棄
	Ally::UnloadModel();

	// サウンド破棄
	Player::UnloadSound();

	// モデル破棄
	Player::UnloadModel();

	// サウンド破棄
	MobileSuit::UnloadSound();

	// テクスチャ破棄
	MeshFieldComponent::UnloadTexture();
}

void GameScene::SetData()
{

}

void GameScene::UpdateScene()
{
	ImGui::Begin(MyImGuiData::k_DebugWindowName);
	ImGui::Text(u8"キーボードの場合");
	ImGui::Text(u8"WASD:移動 Spaceキー:ジャンプ Rキーメイン射撃");
	ImGui::Text(u8"Enterキー : ロック切り替え");

	ImGui::Text(u8"コントローラーの場合");
	ImGui::Text(u8"十字パッド:移動 Aボタン:ジャンプ Xボタンメイン射撃");
	ImGui::Text(u8"Bボタン : ロック切り替え");

	ImGui::Text(u8"ジャンプボタン素早く2連打:ブーストダッシュ");


	ImGui::Checkbox("DebugCollisionView", &m_IsUseDebugCollisionViewFlag);
	ImGui::End();

	// デバッグコリジョンビューを表示するかどうか
	OBBDebugViewComponent::SetIsUseFlag(m_IsUseDebugCollisionViewFlag);
	AABBDebugViewComponent::SetIsUseFlag(m_IsUseDebugCollisionViewFlag);

	if (m_Fade == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::UpdateScene m_Fadeがnullptr");
		return;
	}

	// フェードし終えたらシーンを切り替える
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
	// シーンレンダー作成
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
	// コリジョンマネージャー作成
	if (m_SceneCollisionManager == nullptr)
	{
		m_SceneCollisionManager = new SceneCollisionManager();
		m_SceneCollisionManager->Init();
	}
}

void GameScene::UnloadSceneFunction()
{
	// シーンコリジョンマネージャー破棄
	if (m_SceneCollisionManager)
	{
		m_SceneCollisionManager->Uninit();
		delete m_SceneCollisionManager;
		m_SceneCollisionManager = nullptr;
	}
	// シーンレンダー破棄
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
