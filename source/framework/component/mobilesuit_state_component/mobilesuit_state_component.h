/**
 * @file mobilesuit_state_component.h
 * @brief ���r���X�[�c�X�e�[�g�R���|�[�l���g
 */

#pragma once

#include"../component.h"

 /**
 * @brief �X�e�[�g�R���|�[�l���g
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
	//! ���r���X�[�c�X�e�[�g�}�b�v
	std::unordered_map<std::string, class MobileSuitState*> m_MobileSuitStateMap;

	//! ���݂̏��
	class MobileSuitState* m_CurrentMobileSuitState;

	//! ���W�b�h�{�f�B�R���|�[�l���g
	class RigidBodyComponent* m_RigidBodyComponent;

	//! ���b�N�I���J�����R���|�[�l���g
	class LockOnCameraComponent* m_LockOnCameraComponent;

	//! �u�[�X�g�R���|�[�l���g
	class BoostComponent* m_BoostComponent;

	//! �A�j���[�V�����R���|�[�l���g
	class AnimationModelComponent* m_AnimationModelComponent;

	//! AI���v���C���[���͂�
	bool m_IsAI;

	//! �r�[���T�[�x��
	class BeamSaber* m_BeamSaber;

	//! NotMoveFlag
	bool m_IsMoveFlag;

	//! �g���K�[���͗p�ϐ�
	bool m_IsTriggerInput[INPUT_MAX];

	//! �v���X���͗p�ϐ�
	bool m_IsPressInput[INPUT_MAX];

	//! ����������
	Conv_XM::Vector2f m_InputDirection;

	//! �X�e�b�v�\��
	bool m_StepReserve;

	//! �X�e�b�v���̓J�E���g
	int m_StepInputCount;

	//! �X�e�b�v���͕���
	StepDirection m_StepDirection;
public:
	MobileSuitStateComponent(class Actor* owner);
	~MobileSuitStateComponent();

	/**
	* @brief ���r���X�[�c�̏�ԕύX
	* @param[in] const std::string&(name) ��Ԗ�
	*/
	void ChangeMobileSuitState(const std::string& name);

	/**
	* @brief �X�e�[�g�N���G�C�g�֐�
	*/
	template <typename T>
	void CreateMobileSuitState()
	{
		//�쐬
		T* state = new T(this);

		if (state == nullptr)
		{
			Logger::GetInstance().SetLog("CreateMobileSuitState state��nullptr");
			return;
		}

		const auto& iter = m_MobileSuitStateMap.find(state->GetName());

		//�d�����Ă��Ȃ����`�F�b�N
		if (iter != m_MobileSuitStateMap.end())
		{
			//�d�����Ă�����폜
			delete state;
			state = nullptr;
		}
		else
		{
			//�o�^
			m_MobileSuitStateMap.emplace(state->GetName(), state);
		}
	}

	//! �Q�b�^�[
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
	* @brief ����������
	*/
	virtual void Init()override;

	/**
	* @brief �I������
	*/
	virtual void Uninit()override;

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput()override;

	/**
	* @brief �X�V����
	*/
	virtual void Update()override;

	/**
	* @brief ���͎擾
	*/
	void GetInput();
};