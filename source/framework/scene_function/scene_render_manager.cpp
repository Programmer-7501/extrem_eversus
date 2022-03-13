
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
		Logger::GetInstance().SetLog("SceneRenderManager::Draw m_TimeMeasure��nullptr");
		return;
	}
	// �v���J�n
	m_TimeMeasure->Start();

	// 3D�`��
	Draw3D();

	// 2D�`��
	Draw2D();

	// �v���I��
	m_TimeMeasure->End();
}

void SceneRenderManager::AddSpriteComponent(SpriteComponent * spriteComponent)
{
	// null�`�F�b�N
	if (spriteComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::AddSpriteComponent spriteComponent��nullptr");
		return;
	}
	//�}������ꏊ��������
	int myDrawOrder = spriteComponent->GetDrawOrder();
	auto&& iter = m_SpriteComponents.begin();
	for (; iter != m_SpriteComponents.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	// �C�e���[�^�[�̑O�̃|�W�V�����ɑ}��
	m_SpriteComponents.insert(iter, spriteComponent);
}

void SceneRenderManager::AddMeshComponent(MeshComponent * meshComponent)
{
	// null�`�F�b�N
	if (meshComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::AddMeshComponent meshComponent��nullptr");
		return;
	}

	// �e���͂��邩�ǂ���
	if (meshComponent->GetIsShadowInput() == true)
	{
		m_ShadowInputMeshComponents.emplace_back(meshComponent);
	}

	// �����_�����O���@�𔻕�
	switch (meshComponent->GetRenderingWay())
	{
	case MeshComponent::RENDERING_DEFERRED:
		// ���f���R���|�[�l���g��ɓo�^
		m_DeferredRenderingMeshComponents.emplace_back(meshComponent);
		break;
	case MeshComponent::RENDERING_FORWARD:
		// ���f���R���|�[�l���g��ɓo�^
		m_ForwardRenderingMeshComponents.emplace_back(meshComponent);
		break;
	default:
		break;
	}

}

void SceneRenderManager::Draw3D() const
{
	// �����_���[�̎擾
	Renderer& renderer = Renderer::GetInstance();

	// �V���h�E�}�b�v���͊J�n
	renderer.ShadowInputStart();

	// �V���h�E�}�b�v����
	InputShadow();

	// �V���h�E�}�b�v���͏I��
	renderer.ShadowInputEnd();


	// GBuffer���͊J�n
	renderer.GBufferInputStart();

	// GBuffer����
	InputGBuffer();

	// GBuffer���͏I��
	renderer.GBufferInputEnd();


	// �f�B�t�@�[�h�����_�����O
	renderer.ExcuteDeferredRendering();


	// �t�H���[�h�����_�����O�J�n
	renderer.ForwardRenderingStart();

	// �t�H���[�h�����_�����O
	FowardRenderingExcute();

	// �t�H���[�h�����_�����O�I��
	renderer.ForwardRenderingEnd();


	// �����_�����O���ʏo��
	renderer.ExcuteOutPutRenderingResult();
}

void SceneRenderManager::Draw2D() const
{
	// 2D�p�����[�^�[�ɕύX
	Renderer::GetInstance().SetWorldViewProjection2D();

	// �X�v���C�g�R���|�[�l���g�̕`��
	for (auto* const spriteComponent : m_SpriteComponents)
	{
		// null�`�F�b�N
		if (spriteComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::Draw2D spriteComponent��nullptr");
			continue;
		}
		spriteComponent->Draw();
	}
}

void SceneRenderManager::InputShadow() const
{
	// null�`�F�b�N
	if (m_LightCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::InputShadow m_LightCameraComponent��nullptr");
		return;
	}
	// �r���[�v���W�F�N�V�����s��o�^
	m_LightCameraComponent->SetCameraData();
	
	// ���f���̕`��
	for (auto* const meshComponent : m_ShadowInputMeshComponents)
	{
		// null�`�F�b�N
		if (meshComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::InputShadow meshComponent��nullptr");
			continue;
		}
		meshComponent->ShadowDraw();
	}
}

void SceneRenderManager::InputGBuffer() const
{
	// null�`�F�b�N
	if (m_MainCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::InputGBuffer m_MainCameraComponent��nullptr");
		return;
	}
	// �r���[�v���W�F�N�V�����s��o�^
	m_MainCameraComponent->SetCameraData();

	// ���f���̕`��
	for (auto* const meshComponent : m_DeferredRenderingMeshComponents)
	{
		// null�`�F�b�N
		if (meshComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::InputGBuffer meshComponent��nullptr");
			continue;
		}
		meshComponent->Draw();
	}
}

void SceneRenderManager::FowardRenderingExcute() const
{
	// null�`�F�b�N
	if (m_MainCameraComponent == nullptr)
	{
		Logger::GetInstance().SetLog("SceneRenderManager::InputGBuffer m_MainCameraComponent��nullptr");
		return;
	}
	// �r���[�v���W�F�N�V�����s��o�^
	m_MainCameraComponent->SetCameraData();

	// ���f���̕`��
	for (auto* const meshComponent : m_ForwardRenderingMeshComponents)
	{
		// null�`�F�b�N
		if (meshComponent == nullptr)
		{
			Logger::GetInstance().SetLog("SceneRenderManager::FowardRenderingExcute meshComponent��nullptr");
			continue;
		}
		meshComponent->Draw();
	}


	// �p�[�e�B�N���̕`��
	ParticleManager::GetInstance().Draw();
}