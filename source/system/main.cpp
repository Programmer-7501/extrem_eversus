
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
	//���������[�N���m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//�Q�Ƃ��Ȃ��p�����[�^�[
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// �����̎��������
	srand(static_cast<unsigned int>(time(NULL)));

	//�C���X�^���X�쐬
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

	// �E�B���h�E����
	if (myWindow.Create() == false)
	{
		return 0;
	}
	// FPS���䏉����
	fpsManager.Init();
	// �����_���[�̏�����
	renderer.Init();
	// �V�F�[�_�[�̏�����
	shaderManager.Init();
	// �����_�����O�p�C�v���C���̍\�z
	renderer.CreateRenderingPipline();
	// �p�[�e�B�N���}�l�[�W���[�̏�����
	particleManager.Init();
	// �V�[���}�l�[�W���[�̏�����
	sceneManager.Init();
	// ���̓}�l�[�W���[�̏�����
	inputManager.Init();
	// ImGui�̏�����
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
			// FPS�̌v���J�n
			fpsManager.Begin();

			// ImGui�̍X�V
			myImgui.Update();
			// ���̓}�l�[�W���[�̍X�V
			inputManager.Update();
			// �V�[���}�l�[�W���[�̍X�V
			sceneManager.Update();
			// FPS�̕\��
			fpsManager.InputImGui();

			{
				// �V�[���}�l�[�W���[�̕`��
				sceneManager.Draw();

				// �`��I��
				renderer.DrawEnd();
			}

			// �V�[���؂�ւ�
			sceneManager.SceneExChange();

			// FPS�̌v���I��
			fpsManager.End();
		}
	}

	audioManager.Uninit();

	// ImGui�̏I������
	myImgui.Uninit();
	// ���̓}�l�[�W���[�̏I������
	inputManager.Uninit();
	// �V�[���}�l�[�W���[�̏I������
	sceneManager.Uninit();
	// �A�j���[�V�������f���̑S�J��
	animationModelManager.AllRelease();
	// ���f���̑S�J��
	modelManager.AllRelease();
	// �e�N�X�`���̑S�J��
	textureManager.AllRelease();
	// �V�F�[�_�[�̏I������
	shaderManager.Uninit();
	// �p�[�e�B�N���}�l�[�W���[�̏I������
	particleManager.Uninit();
	// �����_���[�̏I������
	renderer.Uninit();
	// FPS����I������
	fpsManager.Uninit();
	// �E�B���h�E�I������
	myWindow.Uninit();


	// ���O�o��
	Logger::GetInstance().OutPut();

	//�V���O���g�����
	SingletonFinalizer::Finalize();

	return (int)msg.wParam;
}