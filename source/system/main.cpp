
#include"main.h"
#include"mywindows.h"
#include"fps_manager.h"
#include"../directx/renderer.h"
#include"../directx/resource/shader_manager.h"
#include"../framework/scene/scene_manager.h"
#include"../directx/resource/texture_manager.h"
#include"../directx/resource/model_manager.h"
#include"../directx/resource/animation_model_manager.h"
#include"input_manager.h"
#include"../particle/particle_manager.h"
#include"../framework/component/audio_component/audio_component.h"
#include"audio_manager.h"
#include"myimgui.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//メモリリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//参照しないパラメーター
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// 乱数の種を初期化
	srand(static_cast<unsigned int>(time(NULL)));

	//インスタンス作成
	MyWindows& myWindow = MyWindows::GetInstance();
	FPSManager& fpsManager = FPSManager::GetInstance();
	Renderer& renderer = Renderer::GetInstance();
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();
	InputManager& inputManager = InputManager::GetInstance();
	MyImGui& myImgui = MyImGui::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();
	ModelManager& modelManager = ModelManager::GetInstance();
	AnimationModelManager& animationModelManager = AnimationModelManager::GetInstance();

	ParticleManager& particleManager = ParticleManager::GetInstance();

	AudioManager& audioManager = AudioManager::GetInstance();
	audioManager.Init();

	// ウィンドウ生成
	if (myWindow.Create() == false)
	{
		return 0;
	}
	// FPS制御初期化
	fpsManager.Init();
	// レンダラーの初期化
	renderer.Init();
	// シェーダーの初期化
	shaderManager.Init();
	// レンダリングパイプラインの構築
	renderer.CreateRenderingPipline();
	// パーティクルマネージャーの初期化
	particleManager.Init();
	// シーンマネージャーの初期化
	sceneManager.Init();
	// 入力マネージャーの初期化
	inputManager.Init();
	// ImGuiの初期化
	myImgui.Init();

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// FPSの計測開始
			fpsManager.Begin();

			// ImGuiの更新
			myImgui.Update();
			// 入力マネージャーの更新
			inputManager.Update();
			// シーンマネージャーの更新
			sceneManager.Update();
			// FPSの表示
			fpsManager.InputImGui();

			{
				// シーンマネージャーの描画
				sceneManager.Draw();

				// 描画終了
				renderer.DrawEnd();
			}

			// シーン切り替え
			sceneManager.SceneExChange();

			// FPSの計測終了
			fpsManager.End();
		}
	}

	audioManager.Uninit();

	// ImGuiの終了処理
	myImgui.Uninit();
	// 入力マネージャーの終了処理
	inputManager.Uninit();
	// シーンマネージャーの終了処理
	sceneManager.Uninit();
	// アニメーションモデルの全開放
	animationModelManager.AllRelease();
	// モデルの全開放
	modelManager.AllRelease();
	// テクスチャの全開放
	textureManager.AllRelease();
	// シェーダーの終了処理
	shaderManager.Uninit();
	// パーティクルマネージャーの終了処理
	particleManager.Uninit();
	// レンダラーの終了処理
	renderer.Uninit();
	// FPS制御終了処理
	fpsManager.Uninit();
	// ウィンドウ終了処理
	myWindow.Uninit();


	// ログ出力
	Logger::GetInstance().OutPut();

	//シングルトン解放
	SingletonFinalizer::Finalize();

	return (int)msg.wParam;
}