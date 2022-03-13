
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
	// 影描画をする
	m_IsShadowInput = true;
}

void AnimationModelComponent::Update()
{
	// 最大カウントを取得
	if (m_AnimationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Update m_AnimationModelがnullptr");
		return;
	}

	// アニメーションモデルの更新
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
		// 現在のフレーム更新
		if (m_StopFlag == false)
		{
			// 計測用カウンターを更新
			m_Counter += m_CounterSpeed;
			// 整数部を抜き出す
			int plusframe = static_cast<int>(m_Counter);
			// フレームに追加する
			m_CurrentFrame += plusframe;
			// 追加した分減らす
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
	// nullチェック
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

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
		//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	renderer.SetWorldMatrix(mtxWorld);

	renderer.SetMaterial(m_Material);

	// モデル描画
	m_AnimationModel->Draw();

	CBufferData::Material material = {};
	material.Metaric = 0.5f;
	material.Smoothness = 0.5f;
	renderer.SetMaterial(material);
}

void AnimationModelComponent::ShadowDraw() const
{
	// nullチェック
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

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(NULL, NULL, 0);

	//マトリクス設定
	Conv_XM::Matrix4x4f mtxWorld = m_Owner->GetWorldMatrix();

	Conv_XM::Matrix4x4f mtxOffsetWorld, mtxOffsetS, mtxOffsetR;

	mtxOffsetS = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	mtxOffsetR = DirectX::XMMatrixRotationQuaternion(m_Quaternion);

	mtxOffsetWorld = (DirectX::XMMATRIX)mtxOffsetS * (DirectX::XMMATRIX)mtxOffsetR;

	mtxWorld = (DirectX::XMMATRIX)mtxOffsetWorld * (DirectX::XMMATRIX)mtxWorld;

	Renderer::GetInstance().SetWorldMatrix(mtxWorld);

	// モデル描画
	m_AnimationModel->Draw();
}

void AnimationModelComponent::SetCurrentAnimationBlendToNextAnimation(const std::string & blendAnimationName, int time)
{
	if (m_CurrentAnimeName == blendAnimationName)
	{
		// ブレンドモードオフ
		m_IsBlendMode = false;

		m_BlendCount = 0;
		return;
	}


	// ブレンドアニメーションの名前を登録
	m_BlendAnime001Name = m_CurrentAnimeName;

	// ブレンドカウント初期化
	m_BlendAnime001Count = m_CurrentFrame;

	// 最大カウント
	m_BlendAnime001MaxCount = m_MaxCount;

	// ブレンドアニメーションの名前を登録
	m_BlendAnime002Name = blendAnimationName;

	// ブレンドカウント初期化
	m_BlendAnime002Count = 0;

	// 最大カウント
	m_BlendAnime002MaxCount = m_AnimationModel->GetMaxFrame(m_BlendAnime002Name);

	// 最大ブレンドカウント
	m_MaxBlendCount = time;

	// ブレンドモードオン
	m_IsBlendMode = true;

	// カウント初期化
	m_BlendCount = 0;
}

void AnimationModelComponent::SetBlendAnimation(const std::string & blendAnimationName001, const std::string & blendAnimationName002, int time)
{

	// ブレンドアニメーションの名前を登録
	m_BlendAnime001Name = blendAnimationName001;

	// ブレンドカウント初期化
	m_BlendAnime001Count = 0;

	// 最大カウント
	m_BlendAnime001MaxCount = m_AnimationModel->GetMaxFrame(m_BlendAnime001Name);;

	// ブレンドアニメーションの名前を登録
	m_BlendAnime002Name = blendAnimationName002;

	// ブレンドカウント初期化
	m_BlendAnime002Count = 0;

	// 最大カウント
	m_BlendAnime002MaxCount = m_AnimationModel->GetMaxFrame(m_BlendAnime002Name);

	// 最大ブレンドカウント
	m_MaxBlendCount = time;

	// ブレンドモードオン
	m_IsBlendMode = true;

	m_BlendCount = 0;
}

void AnimationModelComponent::SetAnimationModel(AnimationModel * animationModel)
{
	if (animationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::SetModel animationModelがnullptr");
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

	// 最大カウントを取得
	if (m_AnimationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::SetAnimation m_AnimationModelがnullptr");
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

	// 最大カウントを取得
	if (m_AnimationModel == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::SetAnimation m_AnimationModelがnullptr");
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
		Logger::GetInstance().SetLog("AnimationModelComponent::SetIsIdentity m_AnimationModelがnullptr");
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
	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::Init GetShaderが失敗");
		return false;
	}

	return true;
}

bool AnimationModelComponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool AnimationModelComponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_ANIMATION);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_GBUFFERINPUT);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_ANIMATION);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("AnimationModelComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
