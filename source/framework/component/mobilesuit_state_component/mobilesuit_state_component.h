/**
 * @file mobilesuit_state_component.h
 * @brief モビルスーツステートコンポーネント
 */

#pragma once

#include"../component.h"

 /**
 * @brief ステートコンポーネント
 */
class MobileSuitStateComponent : public Component
{
public:
	enum InputName
	{
		IN_SHAGEKI,
		IN_KAKUTOU,
		IN_JUMP,
		IN_LEFT_STICK,
		INPUT_MAX,
	};

	enum StepDirection
	{
		SD_NONE,
		SD_LEFT,
		SD_RIGHT,
		SD_FORWARD,
		SD_BACK,
	};
protected:
	//! モビルスーツステートマップ
	std::unordered_map<std::string, class MobileSuitState*> m_MobileSuitStateMap;

	//! 現在の状態
	class MobileSuitState* m_CurrentMobileSuitState;

	//! リジッドボディコンポーネント
	class RigidBodyComponent* m_RigidBodyComponent;

	//! ロックオンカメラコンポーネント
	class LockOnCameraComponent* m_LockOnCameraComponent;

	//! ブーストコンポーネント
	class BoostComponent* m_BoostComponent;

	//! アニメーションコンポーネント
	class AnimationModelComponent* m_AnimationModelComponent;

	//! AIかプレイヤー入力か
	bool m_IsAI;

	//! ビームサーベル
	class BeamSaber* m_BeamSaber;

	//! NotMoveFlag
	bool m_IsMoveFlag;

	//! トリガー入力用変数
	bool m_IsTriggerInput[INPUT_MAX];

	//! プレス入力用変数
	bool m_IsPressInput[INPUT_MAX];

	//! 向かう方向
	Conv_XM::Vector2f m_InputDirection;

	//! ステップ予約
	bool m_StepReserve;

	//! ステップ入力カウント
	int m_StepInputCount;

	//! ステップ入力方向
	StepDirection m_StepDirection;
public:
	MobileSuitStateComponent(class Actor* owner);
	~MobileSuitStateComponent();

	/**
	* @brief モビルスーツの状態変更
	* @param[in] const std::string&(name) 状態名
	*/
	void ChangeMobileSuitState(const std::string& name);

	/**
	* @brief ステートクリエイト関数
	*/
	template <typename T>
	void CreateMobileSuitState()
	{
		//作成
		T* state = new T(this);

		if (state == nullptr)
		{
			Logger::GetInstance().SetLog("CreateMobileSuitState stateがnullptr");
			return;
		}

		const auto& iter = m_MobileSuitStateMap.find(state->GetName());

		//重複していないかチェック
		if (iter != m_MobileSuitStateMap.end())
		{
			//重複していたら削除
			delete state;
			state = nullptr;
		}
		else
		{
			//登録
			m_MobileSuitStateMap.emplace(state->GetName(), state);
		}
	}

	//! ゲッター
	inline class RigidBodyComponent* GetRigidBodyComponent() const { return m_RigidBodyComponent; }
	inline class LockOnCameraComponent* GetLockOnCameraComponent() const { return m_LockOnCameraComponent; }
	inline class BoostComponent* GetBoostComponent() const { return m_BoostComponent; }
	inline class AnimationModelComponent* GetAnimationModelComponent() const { return m_AnimationModelComponent; }

	inline void SetComponent(
		class RigidBodyComponent* rigidBody,
		class LockOnCameraComponent* lockOnCamera,
		class BoostComponent* boost,
		class AnimationModelComponent* animationModel
	)
	{
		m_RigidBodyComponent = rigidBody;
		m_LockOnCameraComponent = lockOnCamera;
		m_BoostComponent = boost;
		m_AnimationModelComponent = animationModel;
	}

	inline void SetIsAI(bool flag) { m_IsAI = flag; }

	inline bool GetIsAI() { return m_IsAI; }

	inline void SetBeamSaber(class BeamSaber* beamSaber) { m_BeamSaber = beamSaber; }
	inline class BeamSaber* GetBeamSaber() { return m_BeamSaber; }

	inline void SetIsMoveFlag(bool flag) { m_IsMoveFlag = flag; }

	inline void SetIsTriggerInput(InputName inputName, bool flag) { m_IsTriggerInput[inputName] = flag; }
	inline bool GetIsTriggerInput(InputName inputName) { return m_IsTriggerInput[inputName]; }

	inline void SetIsPressInput(InputName inputName, bool flag) { m_IsPressInput[inputName] = flag; }
	inline bool GetIsPressInput(InputName inputName) { return m_IsPressInput[inputName]; }

	inline Conv_XM::Vector2f GetInputDirection() { return m_InputDirection; }
	inline void SetInputDirection(const Conv_XM::Vector2f& inputDirection) { m_InputDirection = inputDirection; }

	class MobileSuitState* GetCurrentMobileSuitState() { return m_CurrentMobileSuitState; }

	inline void SetStepReserve(StepDirection stepDirection)
	{
		m_StepReserve = true;
		m_StepInputCount = 0;
		m_StepDirection = stepDirection;
	}

	inline StepDirection GetStepDirection(){ return m_StepDirection; }
protected:

	/**
	* @brief 初期化処理
	*/
	virtual void Init()override;

	/**
	* @brief 終了処理
	*/
	virtual void Uninit()override;

	/**
	* @brief 入力処理
	*/
	virtual void ProcessInput()override;

	/**
	* @brief 更新処理
	*/
	virtual void Update()override;

	/**
	* @brief 入力取得
	*/
	void GetInput();
};