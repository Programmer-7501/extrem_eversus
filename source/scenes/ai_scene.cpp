
#include"../system/main.h"
#include "ai_scene.h"
#include"../framework/scene/scene_manager.h"
#include"../framework/scene_function/scene_render_manager.h"
#include"../framework/actor/ui/game_clear_scene/game_clear_scene_ui.h"
#include"../framework/actor_manager/other_actor_manager.h"
#include"../system/fade.h"
#include"../framework/component/cameracomponent/light_camera_component.h"
#include"../framework/actor/camera/debug_camera.h"
#include"../particle/particle_manager.h"
#include"../framework/actor/ai/ai_test.h"
#include"../framework/actor/ai/ai_node.h"
#include"../framework/actor/ai/ai_goal_node.h"
#include"../framework/component/mesh_component/mesh_field_component.h"
#include"../framework/component/mesh_component/debug/debug_number_billboard_component.h"
#include"../framework/component/mesh_component/debug/aabb_debug_view_component.h"
#include"../framework/component/light_component/directional_light_component.h"
#include"../framework/actor/stage/sideseven_object.h"

#include"../framework/actor/ai/gbfs/gbfs_goal_node.h"
#include"../framework/actor/ai/gbfs/gbfs_node.h"
#include"../framework/actor/ai/gbfs/gbfs_test.h"

#include"../framework/actor/ai/astar/astar_goal_node.h"
#include"../framework/actor/ai/astar/astar_node.h"
#include"../framework/actor/ai/astar/astar_test.h"
#include"../framework/actor/ai/weighted_graph_node_maker.h"
#include"../framework/actor/ai/weighted_graph_node_loder.h"

#include"../system/myimgui.h"

/**
* @namespace AISceneData
* @brief AIシーン情報
*/
namespace AISceneData
{
	//! フェードイン時間
	static constexpr int k_FadeInTime = 60;

	//! フェードアウト時間
	static constexpr int k_FadeOutTime = 60;
}


AIScene::AIScene()
	: m_IsSceneChange(false)
	, m_Fade(nullptr)
{
}

AIScene::~AIScene()
{
}

void AIScene::LoadData()
{
	// AABBのデバッグコリジョンビューを消す
	AABBDebugViewComponent::SetIsUseFlag(false);

	// パーティクルのデバッグカウントを消す
	ParticleManager::GetInstance().SetDebugCountView(false);

	// シーンマネージャー取得
	SceneManager& sceneManager = SceneManager::GetInstance();

	// フェード取得
	m_Fade = sceneManager.GetFade();
	// フェード開始
	m_Fade->SetFade(Fade::FADE_OUT, AISceneData::k_FadeOutTime);


	// アクターマネージャー作成
	OtherActorManager* otherActorManager = CreateActorManager<OtherActorManager>();
	if (otherActorManager == nullptr)
	{
		Logger::GetInstance().SetLog("AIScene::LoadData otherActorManagerがnullptr");
		return;
	}

	// デバッグカメラを作成
	DebugCamera* debugCamera = otherActorManager->CreateActor<DebugCamera>();
	if (debugCamera == nullptr)
	{
		Logger::GetInstance().SetLog("AIScene::LoadData debugCameraがnullptr");
		return;
	}
	debugCamera->SetPosition(Conv_XM::Vector3f(0.0f, 5.0f, -10.0f));

	// ライトカメラを作成
	Actor* lightCameraActor = otherActorManager->CreateActor<Actor>();
	if (lightCameraActor == nullptr)
	{
		Logger::GetInstance().SetLog("AIScene::LoadData lightCameraActorがnullptr");
		return;
	}
	LightCameraComponent* lightCamera = lightCameraActor->CreateComponent<LightCameraComponent>();
	if (lightCamera == nullptr)
	{
		Logger::GetInstance().SetLog("AIScene::LoadData lightCameraActorがnullptr");
		return;
	}
	lightCamera->SetPosition(Conv_XM::Vector3f(0.0f, 20.0f, 0.0f));
	lightCamera->SetTarget(Conv_XM::Vector3f(0.0f, 0.0f, 0.0f));
	lightCamera->SetUp(Conv_XM::Vector3f(0.0f, 0.0f, 1.0f));
	if (m_SceneRenderManager == nullptr)
	{
		Logger::GetInstance().SetLog("AIScene::LoadData m_SceneRenderManagerがnullptr");
		return;
	}
	m_SceneRenderManager->SetLightCameraComponent(lightCamera);

	WeightedGraphLoader* weightGraph = otherActorManager->CreateActor<WeightedGraphLoader>();
	weightGraph->Load("asset/graphdata/stage_graph_node_edge.bin");

	AStarNode::LoadModel();
	AStarNode* aStarNode = otherActorManager->CreateActor<AStarNode>();

	AStarTest::LoadModel();
	AStarTest* aStarTest = otherActorManager->CreateActor<AStarTest>();

	AStarGoalNode::LoadModel();
	AStarGoalNode* aStarGoalNode = otherActorManager->CreateActor<AStarGoalNode>();

	aStarNode->SetWeightedGraph(weightGraph->GetWeightGraph());
	aStarTest->SetWeightedGraph(weightGraph->GetWeightGraph());

	aStarGoalNode->SetAStarTest(aStarTest);

	// メッシュフィールドの作成
	Actor* meshfieldActor = otherActorManager->CreateActor<Actor>();
	if (meshfieldActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldActorがnullptr");
		return;
	}
	MeshFieldComponent::LoadTexture();
	MeshFieldComponent* meshfieldComponent = meshfieldActor->CreateComponent<MeshFieldComponent>();
	if (meshfieldComponent == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData meshfieldComponentがnullptr");
		return;
	}
	meshfieldComponent->LoadHeightMapFile("asset/mapdata/stagedata001.csv");
	meshfieldComponent->LoadTextureBlendFile("asset/mapdata/textureblend001.csv", 1);
	meshfieldComponent->LoadTextureBlendFile("asset/mapdata/textureblend002.csv", 2);
	meshfieldComponent->LoadTextureBlendFile("asset/mapdata/textureblend003.csv", 3);
	meshfieldComponent->LoadTextureBlendFile("asset/mapdata/textureblend004.csv", 4);
	CBufferData::Material material = {};
	material.Metaric = 0.3f;
	material.Smoothness = 0.1f;
	meshfieldComponent->SetMaterial(material);


	// ディレクショナルライト
	Actor* dirLightActor = otherActorManager->CreateActor<Actor>();
	if (dirLightActor == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData dirLightActorがnullptr");
		return;
	}
	DirectionalLightComponent* directionalLight = dirLightActor->CreateComponent<DirectionalLightComponent>();
	if (directionalLight == nullptr)
	{
		Logger::GetInstance().SetLog("GameScene::LoadData directionalLightがnullptr");
		return;
	}
	CBufferData::DirectionalLight lightData = {};
	lightData.Ambient = Conv_XM::Vector3f(0.5f, 0.5f, 0.5f);
	lightData.Color = Conv_XM::Vector3f(2.0f, 2.0f, 2.0f);
	lightData.Direction = Conv_XM::Vector3f(0.0f, -1.0f, 0.0f);
	directionalLight->SetLight(lightData);

	SideSevenObject::LoadModel();
	otherActorManager->CreateActor<SideSevenObject>();
}

void AIScene::UnloadData()
{
	AStarNode::UnloadModel();

	AStarTest::UnloadModel();

	AStarGoalNode::UnloadModel();

	SideSevenObject::UnloadModel();
}

void AIScene::SetData()
{
}

void AIScene::UpdateScene()
{
	// ImGui入力
	ImGui::Begin(MyImGuiData::k_DebugWindowName);

	// シーン切り替え用
	bool isPush = ImGui::Button("ModeSelectScene");

	ImGui::End();

	if (isPush == true)
	{
		m_Fade->SetFade(Fade::FADE_IN, 60);
		m_IsSceneChange = true;
	}

	// フェードし終えたらシーンを切り替える
	if (m_IsSceneChange == true && m_Fade->GetActive() == false)
	{
		SceneManager::GetInstance().SceneChange(SceneManager::SN_MODESELECT_SCENE);
	}
}

void AIScene::CreateSceneFunction()
{
	// シーンレンダー作成
	if (m_SceneRenderManager == nullptr)
	{
		m_SceneRenderManager = new SceneRenderManager();
		m_SceneRenderManager->Init();
	}
}

void AIScene::UnloadSceneFunction()
{
	if (m_SceneRenderManager)
	{
		m_SceneRenderManager->Uninit();
		delete m_SceneRenderManager;
		m_SceneRenderManager = nullptr;
	}
}
