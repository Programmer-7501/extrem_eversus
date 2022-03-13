
#include"../../system/main.h"
#include "scene_render_manager.h"
#include"../component/sprite_component/sprite_component.h"
#include"../component/cameracomponent/camera_component.h"
#include"../component/mesh_component/mesh_component.h"
#include"../../directx/renderer.h"
#include"../../function/time_measurement.h"
#include"../../particle/particle_manager.h"

SceneRenderManager::SceneRenderManager()
	: m_MainCameraComponent(nullptr)
	, m_LightCameraComponent(nullptr)
{
}

SceneRenderManager::~SceneRenderManager()
{
}

void SceneRenderManager::Init()
{
	m_TimeMeasure = new TimeMeasurement();
}

void SceneRenderManager::Uninit()
{
	delete m_TimeMeasure;
}

void SceneRenderManager::Draw() const
{
	if (m_TimeMeasure == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::Draw m_TimeMeasureがnullptr");
		return;
	}
	// 計測開始
	m_TimeMeasure->Start();

	// 3D描画
	Draw3D();

	// 2D描画
	Draw2D();

	// 計測終了
	m_TimeMeasure->End();
}

void SceneRenderManager::AddSpriteComponent(SpriteComponent * spriteComponent)
{
	// nullチェック
	if (spriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::AddSpriteComponent spriteComponentがnullptr");
		return;
	}
	//挿入する場所を見つける
	int myDrawOrder = spriteComponent->GetDrawOrder();
	auto&& iter = m_SpriteComponents.begin();
	for (; iter != m_SpriteComponents.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	// イテレーターの前のポジションに挿入
	m_SpriteComponents.insert(iter, spriteComponent);
}

void SceneRenderManager::AddMeshComponent(MeshComponent * meshComponent)
{
	// nullチェック
	if (meshComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::AddMeshComponent meshComponentがnullptr");
		return;
	}

	// 影入力するかどうか
	if (meshComponent->GetIsShadowInput() == true)
	{
		m_ShadowInputMeshComponents.emplace_back(meshComponent);
	}

	// レンダリング方法を判別
	switch (meshComponent->GetRenderingWay())
	{
	case MeshComponent::RENDERING_DEFERRED:
		// モデルコンポーネント列に登録
		m_DeferredRenderingMeshComponents.emplace_back(meshComponent);
		break;
	case MeshComponent::RENDERING_FORWARD:
		// モデルコンポーネント列に登録
		m_ForwardRenderingMeshComponents.emplace_back(meshComponent);
		break;
	default:
		break;
	}

}

void SceneRenderManager::Draw3D() const
{
	// レンダラーの取得
	Renderer& renderer = Renderer::GetInstance();

	// シャドウマップ入力開始
	renderer.ShadowInputStart();

	// シャドウマップ入力
	InputShadow();

	// シャドウマップ入力終了
	renderer.ShadowInputEnd();


	// GBuffer入力開始
	renderer.GBufferInputStart();

	// GBuffer入力
	InputGBuffer();

	// GBuffer入力終了
	renderer.GBufferInputEnd();


	// ディファードレンダリング
	renderer.ExcuteDeferredRendering();


	// フォワードレンダリング開始
	renderer.ForwardRenderingStart();

	// フォワードレンダリング
	FowardRenderingExcute();

	// フォワードレンダリング終了
	renderer.ForwardRenderingEnd();


	// レンダリング結果出力
	renderer.ExcuteOutPutRenderingResult();
}

void SceneRenderManager::Draw2D() const
{
	// 2Dパラメーターに変更
	Renderer::GetInstance().SetWorldViewProjection2D();

	// スプライトコンポーネントの描画
	for (auto* const spriteComponent : m_SpriteComponents)
	{
		// nullチェック
		if (spriteComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::Draw2D spriteComponentがnullptr");
			continue;
		}
		spriteComponent->Draw();
	}
}

void SceneRenderManager::InputShadow() const
{
	// nullチェック
	if (m_LightCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::InputShadow m_LightCameraComponentがnullptr");
		return;
	}
	// ビュープロジェクション行列登録
	m_LightCameraComponent->SetCameraData();
	
	// モデルの描画
	for (auto* const meshComponent : m_ShadowInputMeshComponents)
	{
		// nullチェック
		if (meshComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::InputShadow meshComponentがnullptr");
			continue;
		}
		meshComponent->ShadowDraw();
	}
}

void SceneRenderManager::InputGBuffer() const
{
	// nullチェック
	if (m_MainCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::InputGBuffer m_MainCameraComponentがnullptr");
		return;
	}
	// ビュープロジェクション行列登録
	m_MainCameraComponent->SetCameraData();

	// モデルの描画
	for (auto* const meshComponent : m_DeferredRenderingMeshComponents)
	{
		// nullチェック
		if (meshComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::InputGBuffer meshComponentがnullptr");
			continue;
		}
		meshComponent->Draw();
	}
}

void SceneRenderManager::FowardRenderingExcute() const
{
	// nullチェック
	if (m_MainCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::InputGBuffer m_MainCameraComponentがnullptr");
		return;
	}
	// ビュープロジェクション行列登録
	m_MainCameraComponent->SetCameraData();

	// モデルの描画
	for (auto* const meshComponent : m_ForwardRenderingMeshComponents)
	{
		// nullチェック
		if (meshComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::FowardRenderingExcute meshComponentがnullptr");
			continue;
		}
		meshComponent->Draw();
	}


	// パーティクルの描画
	ParticleManager::GetInstance().Draw();
}