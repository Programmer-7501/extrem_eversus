
#include"../../../system/main.h"
#include "mobilesuit_state_component.h"
#include"mobilesuit_state/mobilesuit_state.h"
#include"../../actor/actor.h"
#include"../move_component/rigidbody_component.h"
#include"../cameracomponent/lockon_camera_component.h"
#include"../mobilesuit_component/boost_component.h"
#include"../mesh_component/animation_model_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"

MobileSuitStateComponent::MobileSuitStateComponent(Actor * owner)
	: Component(owner)
	, m_CurrentMobileSuitState(nullptr)
	, m_RigidBodyComponent(nullptr)
	, m_LockOnCameraComponent(nullptr)
	, m_BoostComponent(nullptr)
	, m_AnimationModelComponent(nullptr)
	, m_IsAI(false)
	, m_IsMoveFlag(false)
	, m_IsTriggerInput{}
	, m_IsPressInput{}
	, m_InputDirection(0.0f,0.0f)
	, m_StepReserve(false)
	, m_StepInputCount(0)
	, m_StepDirection(SD_NONE)
{

}

MobileSuitStateComponent::~MobileSuitStateComponent()
{

}

void MobileSuitStateComponent::ChangeMobileSuitState(const std::string & name)
{
	//現在の状態を抜ける
	if (m_CurrentMobileSuitState)
	{
		m_CurrentMobileSuitState->OnExit();
	}

	//新しい状態を連想配列から探す
	const auto& iter = m_MobileSuitStateMap.find(name);
	if (iter == m_MobileSuitStateMap.end())
	{
		// 状態登録
		m_CurrentMobileSuitState = nullptr;
		Logger::GetInstance().SetLog("MobileSuitStateComponent::ChangeMobileSuitState 登録なし");
		return;
	}
	else
	{
		// 状態登録
		m_CurrentMobileSuitState = iter->second;

		//新しい状態に入る
		if (m_CurrentMobileSuitState)
		{
			m_CurrentMobileSuitState->OnEnter();
		}
	}
}

void MobileSuitStateComponent::Init()
{
	// 初期化処理
	for (auto&& state : m_MobileSuitStateMap)
	{
		if (state.second == nullptr)
		{
			continue;
		}

		state.second->Init();
	}
}

void MobileSuitStateComponent::Uninit()
{
	//登録されているステートマップを削除
	for (auto&& iter = m_MobileSuitStateMap.begin(); iter != m_MobileSuitStateMap.end(); iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}
	m_MobileSuitStateMap.clear();
}

void MobileSuitStateComponent::ProcessInput()
{
	// 動かさない
	if (m_IsMoveFlag == false)
	{
		return;
	}

	if (m_CurrentMobileSuitState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateComponent::ProcessInput m_CurrentMobileSuitStateがnullptr");
		return;
	}

	// AIじゃないなら入力取得
	if (m_IsAI == false)
	{
		GetInput();
	}

	m_CurrentMobileSuitState->ProcessInput();
}

void MobileSuitStateComponent::Update()
{
	// 動かさない
	if (m_IsMoveFlag == false)
	{
		return;
	}
	
	if (m_CurrentMobileSuitState == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitStateComponent::Update m_CurrentMobileSuitStateがnullptr");
		return;
	}
	m_CurrentMobileSuitState->Update();
}

void MobileSuitStateComponent::GetInput()
{
	InputManager& inputManager = InputManager::GetInstance();

	// トリガー状態取得
	if (inputManager.GetIsTrigger(InputManager::IN_MAIN) == true)
	{
		m_IsTriggerInput[IN_SHAGEKI] = true;
	}
	else
	{
		m_IsTriggerInput[IN_SHAGEKI] = false;
	}
	if (inputManager.GetIsTrigger(InputManager::IN_KAKUTOU) == true)
	{
		m_IsTriggerInput[IN_KAKUTOU] = true;
	}
	else
	{
		m_IsTriggerInput[IN_KAKUTOU] = false;
	}
	if (inputManager.GetIsTrigger(InputManager::IN_JUMP) == true)
	{
		m_IsTriggerInput[IN_JUMP] = true;
	}
	else
	{
		m_IsTriggerInput[IN_JUMP] = false;
	}
	if (inputManager.GetIsTrigger(InputManager::IN_LEFT_STICK) == true)
	{
		m_IsTriggerInput[IN_LEFT_STICK] = true;
	}
	else
	{
		m_IsTriggerInput[IN_LEFT_STICK] = false;
	}

	// プレス状態取得
	if (inputManager.GetIsPress(InputManager::IN_MAIN) == true)
	{
		m_IsPressInput[IN_SHAGEKI] = true;
	}
	else
	{
		m_IsPressInput[IN_SHAGEKI] = false;
	}
	if (inputManager.GetIsPress(InputManager::IN_KAKUTOU) == true)
	{
		m_IsPressInput[IN_KAKUTOU] = true;
	}
	else
	{
		m_IsPressInput[IN_KAKUTOU] = false;
	}
	if (inputManager.GetIsPress(InputManager::IN_JUMP) == true)
	{
		m_IsPressInput[IN_JUMP] = true;
	}
	else
	{
		m_IsPressInput[IN_JUMP] = false;
	}
	if (inputManager.GetIsPress(InputManager::IN_LEFT_STICK) == true)
	{
		m_IsPressInput[IN_LEFT_STICK] = true;
	}
	else
	{
		m_IsPressInput[IN_LEFT_STICK] = false;
	}

	if (inputManager.GetIsUseGamePad() == true)// ゲームパッドを使っていたら
	{
		// 左ステックの方向ベクトル取得
		m_InputDirection = inputManager.GetLeftStickeVector();
	}
	else
	{
		// 方向ベクトル取得
		if (inputManager.GetIsPress(InputManager::IN_FRONT))
		{
			m_InputDirection = Conv_XM::Vector2f(0.0f, 1.0f);
		}
		else if (inputManager.GetIsPress(InputManager::IN_BACK))
		{
			m_InputDirection = Conv_XM::Vector2f(0.0f, -1.0f);
		}
		else if (inputManager.GetIsPress(InputManager::IN_RIGHT))
		{
			m_InputDirection = Conv_XM::Vector2f(1.0f, 0.0f);
		}
		else if (inputManager.GetIsPress(InputManager::IN_LEFT))
		{
			m_InputDirection = Conv_XM::Vector2f(-1.0f, 0.0f);
		}
		else
		{
			m_InputDirection = Conv_XM::Vector2f(0.0f, 0.0f);
		}
	}
}
