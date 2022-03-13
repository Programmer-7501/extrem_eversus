
#include"../../../system/main.h"
#include "animation_model_component.h"
#include"../../../directx/resource/animation_model.h"
#include"../../actor/actor.h"
#include"../../../directx/renderer.h"
#include"../../../directx/resource/shader_manager.h"


AnimationModelComponent::AnimationModelComponent(Actor * owner)
	: MeshComponent(owner)
	, m_AnimationModel(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_OffsetScale(1.0f, 1.0f, 1.0f)
	, m_Quaternion(DirectX::XMQuaternionIdentity())
	, m_CurrentFrame(0)
	, m_MaxCount(80)
	, m_StopFlag(false)
	, m_Counter(0.0f)
	, m_CounterSpeed(1.0f)
	, m_IsBlendMode(false)
	, m_MaxBlendCount(0)
	, m_BlendCount(0)
	, m_BlendAnime001Count(0)
	, m_BlendAnime001MaxCount(0)
	, m_BlendAnime002Count(0)
	, m_BlendAnime002MaxCount(0)
{
	// �e�`�������
	m_IsShadowInput = true;
}

void AnimationModelComponent::Update()
{
	// �ő�J�E���g���擾
	if (m_AnimationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Update m_AnimationModel��nullptr");
		return;
	}

	// �A�j���[�V�������f���̍X�V
	m_AnimationModel->Update();

	if (m_IsBlendMode == true)
	{
		m_BlendAnime001Count++;
		if (m_BlendAnime001Count > m_BlendAnime001MaxCount)
		{
			m_BlendAnime001Count = 0;
		}


		m_BlendAnime002Count++;
		if (m_BlendAnime002Count > m_BlendAnime002MaxCount)
		{
			m_BlendAnime002Count = 0;
		}


		m_BlendCount++;
		if (m_BlendCount > m_MaxBlendCount)
		{
			m_IsBlendMode = false;


			SetAnimation(m_BlendAnime002Name);
			m_CurrentFrame = m_BlendAnime002Count;
		}
	}
	else
	{
		// ���݂̃t���[���X�V
		if (m_StopFlag == false)
		{
			// �v���p�J�E���^�[���X�V
			m_Counter += m_CounterSpeed;
			// �������𔲂��o��
			int plusframe = static_cast<int>(m_Counter);
			// �t���[���ɒǉ�����
			m_CurrentFrame += plusframe;
			// �ǉ����������炷
			m_Counter -= plusframe;
		}

		if (m_CurrentFrame > m_MaxCount)
		{
			m_CurrentFrame = 0;
		}
	}

}

void AnimationModelComponent::Draw() const
{
	// null�`�F�b�N
	if (m_AnimationModel == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr || m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Draw nullptr");
		return;
	}

	if (m_Owner->GetActive() == false)
	{
		return;
	}

	if (m_IsBlendMode == true)
	{
		float t = m_BlendCount / static_cast<float>(m_MaxBlendCount);
		m_AnimationModel->SetBlendMatrix(m_BlendAnime001Name, m_BlendAnime001Count, m_BlendAnime002Name, m_BlendAnime002Count, t);
	}
	else
	{
		m_AnimationModel->SetMatrix(m_CurrentAnimeName, m_CurrentFrame);
	}


	Renderer& renderer = Renderer::GetInstance();

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
		//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	renderer.SetWorldMatrix(mtxWorld);

	renderer.SetMaterial(m_Material);

	// ���f���`��
	m_AnimationModel->Draw();

	CBufferData::Material material = {};
	material.Metaric = 0.5f;
	material.Smoothness = 0.5f;
	renderer.SetMaterial(material);
}

void AnimationModelComponent::ShadowDraw() const
{
	// null�`�F�b�N
	if (m_AnimationModel == nullptr || m_Owner == nullptr || m_InputLayout == nullptr || m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Draw nullptr");
		return;
	}


	if (m_Owner->GetActive() == false)
	{
		return;
	}

	if (m_IsBlendMode == true)
	{
		float t = m_BlendCount / static_cast<float>(m_MaxBlendCount);
		m_AnimationModel->SetBlendMatrix(m_BlendAnime001Name, m_BlendAnime001Count, m_BlendAnime002Name, m_BlendAnime002Count, t);
	}
	else
	{
		m_AnimationModel->SetMatrix(m_CurrentAnimeName, m_CurrentFrame);
	}

	//���̓��C�A�E�g�ݒ�
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//�V�F�[�_�[�ݒ�
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(NULL, NULL, 0);

	//�}�g���N�X�ݒ�
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	Renderer::GetInstance().SetWorldMatrix(mtxWorld);

	// ���f���`��
	m_AnimationModel->Draw();
}

void AnimationModelComponent::SetCurrentAnimationBlendToNextAnimation(const std::string & blendAnimationName, int time)
{
	if (m_CurrentAnimeName == blendAnimationName)
	{
		// �u�����h���[�h�I�t
		m_IsBlendMode = false;

		m_BlendCount = 0;
		return;
	}


	// �u�����h�A�j���[�V�����̖��O��o�^
	m_BlendAnime001Name = m_CurrentAnimeName;

	// �u�����h�J�E���g������
	m_BlendAnime001Count = m_CurrentFrame;

	// �ő�J�E���g
	m_BlendAnime001MaxCount = m_MaxCount;

	// �u�����h�A�j���[�V�����̖��O��o�^
	m_BlendAnime002Name = blendAnimationName;

	// �u�����h�J�E���g������
	m_BlendAnime002Count = 0;

	// �ő�J�E���g
	m_BlendAnime002MaxCount = m_AnimationModel->GetMaxFrame(m_BlendAnime002Name);

	// �ő�u�����h�J�E���g
	m_MaxBlendCount = time;

	// �u�����h���[�h�I��
	m_IsBlendMode = true;

	// �J�E���g������
	m_BlendCount = 0;
}

void AnimationModelComponent::SetBlendAnimation(const std::string & blendAnimationName001, const std::string & blendAnimationName002, int time)
{

	// �u�����h�A�j���[�V�����̖��O��o�^
	m_BlendAnime001Name = blendAnimationName001;

	// �u�����h�J�E���g������
	m_BlendAnime001Count = 0;

	// �ő�J�E���g
	m_BlendAnime001MaxCount = m_AnimationModel->GetMaxFrame(m_BlendAnime001Name);;

	// �u�����h�A�j���[�V�����̖��O��o�^
	m_BlendAnime002Name = blendAnimationName002;

	// �u�����h�J�E���g������
	m_BlendAnime002Count = 0;

	// �ő�J�E���g
	m_BlendAnime002MaxCount = m_AnimationModel->GetMaxFrame(m_BlendAnime002Name);

	// �ő�u�����h�J�E���g
	m_MaxBlendCount = time;

	// �u�����h���[�h�I��
	m_IsBlendMode = true;

	m_BlendCount = 0;
}

void AnimationModelComponent::SetAnimationModel(AnimationModel * animationModel)
{
	if (animationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::SetModel animationModel��nullptr");
		return;
	}
	m_AnimationModel = animationModel;
}

void AnimationModelComponent::SetAnimation(const std::string & name)
{
	if (m_CurrentAnimeName == name)
	{
		return;
	}

	m_CurrentAnimeName = name;

	// �ő�J�E���g���擾
	if (m_AnimationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::SetAnimation m_AnimationModel��nullptr");
		return;
	}
	m_MaxCount = m_AnimationModel->GetMaxFrame(m_CurrentAnimeName);

	m_Counter = 0.0f;

	m_CurrentFrame = 0;
}

void AnimationModelComponent::ResetAnimation(const std::string & name)
{
	m_IsBlendMode = false;

	m_CurrentAnimeName = name;

	// �ő�J�E���g���擾
	if (m_AnimationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::SetAnimation m_AnimationModel��nullptr");
		return;
	}
	m_MaxCount = m_AnimationModel->GetMaxFrame(m_CurrentAnimeName);

	m_Counter = 0.0f;

	m_CurrentFrame = 0;
}

void AnimationModelComponent::SetIsIdentity(bool flag)
{
	if (m_AnimationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::SetIsIdentity m_AnimationModel��nullptr");
		return;
	}
	m_AnimationModel->SetIsIdentity(flag);
}

void AnimationModelComponent::StopModel(bool flag, int time)
{
	if (flag == true)
	{
		m_StopFlag = flag;
		m_CurrentFrame = time;
	}
	else
	{
		m_StopFlag = flag;
	}
}

bool AnimationModelComponent::LoadData()
{
	// D3D11�̃f�o�C�X�C���^�[�t�F�[�X�̎擾
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Init GetDeviceInterface�����s");
		return false;
	}

	// �V�F�[�_�[�̎擾
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Init GetShader�����s");
		return false;
	}

	return true;
}

bool AnimationModelComponent::GetDeviceContext()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// null�`�F�b�N
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_DeviceContext��nullptr");
		return false;
	}

	return true;
}

bool AnimationModelComponent::GetShader()
{
	// �V�F�[�_�[�}�l�[�W���[�̎擾
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// ���_�V�F�[�_�[�̎擾
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_ANIMATION);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::GetShader m_InputGBufferVertexShader��nullptr");
		return false;
	}

	// �s�N�Z���V�F�[�_�[�̎擾
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_GBUFFERINPUT);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::GetInterface m_InputGBufferPixelShader��nullptr");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̎擾
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_ANIMATION);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::GetInterface m_InputLayout��nullptr");
		return false;
	}

	return true;
}
